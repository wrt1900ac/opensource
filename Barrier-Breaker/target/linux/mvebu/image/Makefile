#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

TARGET_DTBS := armada-xp-db armada-370-db armada-xp-openblocks-ax3-4 armada-370-mirabox \
		armada-370-rd armada-xp-gp

ifeq ($(SUBTARGET),mamba)
TARGET_DTBS := armada-xp-mamba
JFFS2OPTS += --no-cleanmarkers
# override add_jffs2_mark
define add_jffs2_mark
	echo "do nothing"
endef

# override Image/mkfs/prepare/default
define Image/mkfs/prepare/default
	# Use symbolic permissions to avoid clobbering SUID/SGID/sticky bits
	- $(FIND) $(TARGET_DIR) -type f -not -perm +0100 -not -name 'ssh_host*' -not -name 'shadow' -print0 | $(XARGS) -0 chmod u+rw,g+r,o+r
	- $(FIND) $(TARGET_DIR) -type f -perm +0100 -print0 | $(XARGS) -0 chmod u+rwx,g+rx,o+rx
	- $(FIND) $(TARGET_DIR) -type d -print0 | $(XARGS) -0 chmod u+rwx,g+rx,o+rx
	$(INSTALL_DIR) $(TARGET_DIR)/tmp
	chmod 1777 $(TARGET_DIR)/tmp
	rm -f $(TARGET_DIR)/sbin/jffs2reset
	rm -f $(TARGET_DIR)/sbin/switch2jffs
	mv $(TARGET_DIR)/sbin/jffs2reset_mamba $(TARGET_DIR)/sbin/jffs2reset
	mv $(TARGET_DIR)/sbin/switch2jffs_mamba $(TARGET_DIR)/sbin/switch2jffs
endef

endif # SUBTARGET == mamba

LOADADDR:=0x00008000

JFFS2_BLOCKSIZE = 128k

UIMAGE:=$(BIN_DIR)/$(IMG_PREFIX)-uImage

define Image/Build/MkuImage
	mkimage -A arm -O linux -T kernel -a $(LOADADDR) -C none -e $(LOADADDR) \
	-n 'ARM OpenWrt Linux-$(LINUX_VERSION)' -d $(1) $(2);
endef

define Image/Build/DTB
	cp $(KDIR)/zImage$(2) $(KDIR)/zImage$(2)-$(1);
	cat $(LINUX_DIR)/arch/$(ARCH)/boot/dts/$(1).dtb >> $(KDIR)/zImage$(2)-$(1);
	$(call Image/Build/MkuImage,$(KDIR)/zImage$(2)-$(1),$(KDIR)/uImage$(2)-$(1))
	cp $(KDIR)/uImage$(2)-$(1) $(UIMAGE)$(2)-$(1);
endef

define Image/BuildKernel
	$(foreach dtb,$(TARGET_DTBS),$(call Image/Build/DTB,$(dtb)))
 ifneq ($(CONFIG_TARGET_ROOTFS_INITRAMFS),)
	$(call Image/Build/Initramfs)
 endif
endef

define Image/Build/squashfs
	$(STAGING_DIR_HOST)/bin/padjffs2 $(KDIR)/root.squashfs 128
endef

define Image/Build/Initramfs
	$(foreach dtb,$(TARGET_DTBS),$(call Image/Build/DTB,$(dtb),-initramfs))
endef

ifeq ($(SUBTARGET),mamba)
define jffs2-128k-internal
	if [ `stat -L -c %s $(KDIR)/uImage-$(2)` -lt 3145728 ]; \
	then \
	( \
		dd if=$(KDIR)/uImage-$(2) bs=3072k conv=sync; \
		dd if=$(KDIR)/root.$(1) bs=128k conv=sync; \
	) > $(BIN_DIR)/$(IMG_PREFIX)-$(1).img ; \
	else \
	( \
		dd if=$(KDIR)/uImage-$(2) bs=128k conv=sync; \
		dd if=$(KDIR)/root.$(1) bs=128k conv=sync; \
	) > $(BIN_DIR)/$(IMG_PREFIX)-$(1).img ; \
	fi	
endef

define Image/Build/jffs2-128k
	$(foreach dtb,$(TARGET_DTBS),$(call jffs2-128k-internal,jffs2-128k,$(dtb)))
endef
endif # SUBTARGET == mamba

define Image/Build
	$(call Image/Build/$(1))
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/$(IMG_PREFIX)-root.$(1) bs=128k conv=sync
ifneq ($(CONFIG_TARGET_ROOTFS_INITRAMFS),)
	$(call Image/Build/Initramfs)
endif
endef

$(eval $(call BuildImage))
