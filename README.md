openwrt
=======
The main build tree can be found at https://github.com/jimmychungbelkin/Mamba

This repository contains the set of patches, and prebuilt images based on the Linksys WRT1900AC.


Barrier Breaker
=======
Barrier-Breaker/openwrt-armadaxp--jffs2-128k.img - prebuilt image based on OpenWrt Barrier-Breaker

Barrier-Breaker/patch_submission_1.6.tar.gz - patch files for standard OpenWrt Package

OpenWRT git base revision: e97be7a104e5c809ae4638cf169823249a505698

OpenWRT svn base revision: 40006


Attitude Adjustment
=======
Attitude-Adjustment/openwrt-armadaxp--jffs2-128k.img - prebuilt image based on OpenWrt Attitude-Adjustment

=======

After applying the patches in this directory to the OpenWrt Build you can generate an image by doing the following:

Use "make menuconfig" to configure your appreciated
configuration for the toolchain and firmware.

For WRT1900AC:
 The "Target System" in make menuconfig should be set to "Marvell Armada XP/370"

 The "Subtarget" in make menuconfig should be set to "Mamba"

 In "Target Images" in make menuconfig you should enable "jffs2"

You need to have installed gcc, binutils, bzip2, flex, python, perl
make, find, grep, diff, unzip, gawk, getopt, libz-dev and libc headers.

Run "./scripts/feeds update -a" to get all the latest package definitions
defined in feeds.conf / feeds.conf.default respectively

Run "./scripts/feeds install -a" to install symlinks of all of them into
package/feeds/.

Use "make menuconfig" to configure your image.

Simply running "make" will build your firmware.
It will download all sources, build the cross-compile toolchain, 
the kernel and all choosen applications.

After the build completes the firmware image for the WRT1900AC will be 
located at:
./bin/mvebu/openwrt-mvebu-mamba-jffs2-128k.img

How to update from Belkin/Linksys UI

Login into WRT1900AC local UI

Navigate to the Connectivity tab

Select Manual firmware update

Select image to load (e.g., openwrt-armadaxp--jffs2-128k.img)

Select Update firmware

After the firmware is updated, the unit will reboot, and the default ip address will be 192.168.200.1

The OpenWrt system is documented in docs/. You will need a LaTeX distribution
and the tex4ht package to build the documentation. Type "make -C docs/" to build it.

To build your own firmware you need to have access to a Linux, BSD or MacOSX system
(case-sensitive filesystem required). Cygwin will not be supported because of
the lack of case sensitiveness in the file system.


Sunshine!
	Your OpenWrt Project
	http://openwrt.org


