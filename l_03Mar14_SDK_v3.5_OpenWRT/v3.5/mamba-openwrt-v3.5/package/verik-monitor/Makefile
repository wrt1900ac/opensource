#
# Copyright (C) 2011-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=verik-monitor
PKG_RELEASE:=2

include $(INCLUDE_DIR)/package.mk

define Package/verik-monitor
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Verik utilities
  MAINTAINER:=Tom LV <tom.lv@veriksystems.com>
  DEPENDS:=+libpthread
endef

define Package/verik-monitor/description
 This package contains some small utility apps which use to monitor the 
 key events, status...
endef

define Build/Prepare
	$(INSTALL_DIR) $(PKG_BUILD_DIR)
	$(INSTALL_DATA) ./src/key_monitor.c $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -Wall \
		-o $(PKG_BUILD_DIR)/key_monitor $(PKG_BUILD_DIR)/key_monitor.c -lpthread -v
endef

define Package/verik-monitor/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/key_monitor $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,verik-monitor))
