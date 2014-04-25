/*
 *  Copyright (c) 2014
 */

#define _GNU_SOURCE /* for asprintf */
#include <stdio.h>
#include <stdint.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <linux/version.h>
#include <linux/input.h>

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <ctype.h>
#include <pthread.h>

#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

#ifndef EV_SYN
#define EV_SYN 0
#endif

//#define DEBUG
#ifdef DEBUG
#define leds_dbg(fmt, arg...) printf ("%s " fmt "\n", __func__ , ## arg)
#else
#define leds_dbg(fmt, arg...)
#endif

/*
 * Variables
 * */
static int key = 0;
static int is_start = 0;
static int is_power_blink = 0, is_wps_blink = 0;
static pthread_t pid;

/*
 *
 * */
static int is_event_device(const struct dirent *dir) {
	return strncmp(EVENT_DEV_NAME, dir->d_name, 5) == 0;
}

static char* scan_devices(void)
{
	struct dirent **namelist;
	int i, ndev, devnum;
	char *filename;

	ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);
	if (ndev <= 0)
		return NULL;

	fprintf(stderr, "Available devices:\n");

	for (i = 0; i < ndev; i++)
	{
		char fname[64];
		int fd = -1;
		char name[256] = "???";

		snprintf(fname, sizeof(fname),
			 "%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);
		fd = open(fname, O_RDONLY);
		if (fd < 0)
			continue;
		ioctl(fd, EVIOCGNAME(sizeof(name)), name);

		fprintf(stderr, "%s:	%s\n", fname, name);
		close(fd);
		free(namelist[i]);
	}

	fprintf(stderr, "Select the device event number [0-%d]: ", ndev - 1);
	scanf("%d", &devnum);

	if (devnum >= ndev || devnum < 0)
		return NULL;

	asprintf(&filename, "%s/%s%d",
		 DEV_INPUT_EVENT, EVENT_DEV_NAME,
		 devnum);

	return filename;
}

static int usage(void)
{
	printf("USAGE:\n");
	printf("   %s /dev/input/eventX\n", program_invocation_short_name);
	printf("\n");

	return EXIT_FAILURE;
}

void *thread_handler_led(void *arg)
{	
	struct timeval start, end;
	long mtime, secs, usecs;    

	while (1) {
		if (key != 0) {
			if (is_start) {
				is_start = 0;
				gettimeofday(&start, NULL);
				if (key == KEY_WPS_BUTTON)
					is_wps_blink = 0;
				else if (key == KEY_RESTART)
					is_power_blink = 0;
			}
			gettimeofday(&end, NULL);
			secs  = end.tv_sec  - start.tv_sec;
			usecs = end.tv_usec - start.tv_usec;
			mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
			mtime = mtime / 1000;

			if (key == KEY_WPS_BUTTON) {
				if (mtime >= 2) {
					/* 
 					- Start slow blink WPS_WHITE, is_wps_blink = 1.
					1. If successful pairing -> WPS_WHITE -> ON, is_wps_blink = 2
					- Successuful pairing indicator timer expires -> IDLE state is_wps_blink = 0
					OF WPS_WHITE, WPS_AMBER.
					2. Else if session overlap no response or other 
					WPS-related error within walk time of 2min -> Begin blink WPS_AMBER
					is_wps_blink = 3
					- Pairing failure blink timer expires -> IDLE state is_wps_blink = 0
					OFF WPS_WHITE, WPS_AMBER.
					NOTE: Successful/Failure pairing timer expires is 1 min (!!?????)
 					*/
				}
			}
			else if (key == KEY_RESTART) { /* key RESET is pressed */
				if (mtime >= 1 && is_power_blink == 0) { /* time >= 1s */
					is_power_blink = 1;
					system("ledctrl power 255 700 700");
				}else if (mtime >= 10 && is_power_blink == 1) { /* time >= 10s  */
					is_power_blink = 2;
					system("ledctrl power 255 500 500");
				}
			}	
		} else {
			if (is_power_blink != 0) {
				system("ledctrl power off");
				is_power_blink = 0;
			}	
		} /* End if */
		//sleep(1);
		usleep(1000);

	} /* End while */
}

static int print_events(int fd)
{
	struct input_event ev[64];
	int i, rd;
	struct timeval start, end;
	long mtime, secs, usecs;    

	while (1) {
		rd = read(fd, ev, sizeof(struct input_event) * 64);

		if (rd < (int) sizeof(struct input_event)) {
			printf("expected %d bytes, got %d\n", (int) sizeof(struct input_event), rd);
			perror("\nevtest: error reading");
			return 1;
		}

		for (i = 0; i < rd / sizeof(struct input_event); i++) {
			if (ev[i].type != EV_SYN) {
				leds_dbg ("Event: time %ld.%06ld, ", ev[i].time.tv_sec, ev[i].time.tv_usec);
				leds_dbg ("code = %d (0x%x)\n", ev[i].code, ev[i].code);

				if (ev[i].value == 1){
					start.tv_sec = ev[i].time.tv_sec;
					start.tv_usec = ev[i].time.tv_usec;
					key = ev[i].code;
					is_start = 1;
				}else {
					secs  = ev[1].time.tv_sec  - start.tv_sec;
					usecs = ev[1].time.tv_usec - start.tv_usec;
					mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
					leds_dbg ("mtime = %d (%d), secs = %d\n", mtime, mtime / 1000,  secs);
					key = 0;
					is_start = 0;
				}
			}
		} /* End for */

	} /* End while */
}

static int test_grab(int fd)
{
	int rc;

	rc = ioctl(fd, EVIOCGRAB, (void*)1);

	if (!rc)
		ioctl(fd, EVIOCGRAB, (void*)0);

	return rc;
}

static int do_capture(const char *device)
{
	int fd;
	char *filename;

	if (!device) {
		fprintf(stderr, "No device specified, trying to scan all of %s/%s*\n",
			DEV_INPUT_EVENT, EVENT_DEV_NAME);

		if (getuid() != 0)
			fprintf(stderr, "Not running as root, no devices may be available.\n");

		filename = scan_devices();
		if (!filename)
			return usage();
	} else
		filename = strdup(device);

	if (!filename)
		return EXIT_FAILURE;

	if ((fd = open(filename, O_RDONLY)) < 0) {
		perror("evtest");
		if (errno == EACCES && getuid() != 0)
			fprintf(stderr, "You do not have access to %s. Try "
					"running as root instead.\n",
					filename);
		return EXIT_FAILURE;
	}

	free(filename);

	if (!isatty(fileno(stdout)))
		setbuf(stdout, NULL);

	if (test_grab(fd))
	{
		return 0;
	}

	return print_events(fd);
}

int main (int argc, char **argv)
{
	const char *device = NULL;
	int err;

	if (argc < 2) {
		return usage();
	}
	device = argv[1];
		
	err = pthread_create(&pid, NULL, &thread_handler_led, NULL);
	if (err != 0) {
		leds_dbg ("\nCan't create thread :[%s]", strerror(err));
		return 0;
	}
	else
		leds_dbg ("\nThread created successfully\n");
	
	return do_capture(device);
}
/*
 * END
 * */

