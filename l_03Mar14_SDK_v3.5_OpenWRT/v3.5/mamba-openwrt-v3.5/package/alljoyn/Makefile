# Copyright 2012-2013, Qualcomm Innovation Center, Inc.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=alljoyn
PKG_VERSION:=3.3.2
PKG_RELEASE:=3
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)-src
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-src.tgz
PKG_SOURCE_URL:=https://www.alljoyn.org/sites/default/files/resources/
PKG_MD5SUM:=c5711af5ef8a07470c2ae2e24a252b81

# SCons supports parallel builds but does not support make's jobserver
PKG_BUILD_PARALLEL:=$(if $(CONFIG_PKG_BUILD_USE_JOBSERVER),0,1)

include $(INCLUDE_DIR)/package.mk

define Package/alljoyn
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libopenssl +libpthread +librt +libstdcpp
  TITLE:=AllJoyn Peer-to-Peer networking
  URL:=http://www.alljoyn.org
  MAINTAINER:=AllJoyn Open Source Project (Qualcomm Innovation Center, Inc.) <maintainers@alljoyn.org>
endef

define Package/alljoyn/description
AllJoyn is a daemon and application framework for developing
ad hoc peer-to-peer applications.  This includes the run-time
daemon and build-time C++ library.
endef


define Package/alljoyn-c
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+alljoyn
  TITLE:=AllJoyn Peer-to-Peer networking C binding
  URL:=http://www.alljoyn.org
  MAINTAINER:=AllJoyn Open Source Project (Qualcomm Innovation Center, Inc.) <maintainers@alljoyn.org>
endef

define Package/alljoyn-c/description
AllJoyn C binding.  This provides a C library for building
AllJoyn programs in C.  Selecting this will create a package
containing the dynamic library for the C binding.
endef



ifeq ($(CONFIG_DEBUG),y)
  ALLJOYN_BUILD_VARIANT:=debug
else
  ALLJOYN_BUILD_VARIANT:=release
endif

# Only the core C++ binding is supported for this release of AllJoyn for OpenWrt.
ALLJOYN_BINDINGS=core,c

PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/build/openwrt/openwrt/$(ALLJOYN_BUILD_VARIANT)/dist

define Build/Configure/Default
# Override default to do nothing
endef

TARGET_CFLAGS+=-fPIC
TARGET_CPPFLAGS+=-fPIC

define Build/Compile
	rm -f $(PKG_BUILD_DIR)/alljoyn_core/inc/alljoyn/Status.h
	scons -C $(PKG_BUILD_DIR) \
		$(PKG_JOBS) \
		WS=off \
		CPU=openwrt \
		OS=openwrt \
		BINDINGS=$(ALLJOYN_BINDINGS) \
		VARIANT=$(ALLJOYN_BUILD_VARIANT) \
		BD=off \
		"TARGET_CC=$$(TARGET_CC)" \
		"TARGET_CXX=$$(TARGET_CXX)" \
		"TARGET_CFLAGS=$$(TARGET_CFLAGS)" \
		"TARGET_CPPFLAGS=$$(TARGET_CPPFLAGS)" \
		"TARGET_PATH=$$(TARGET_PATH)" \
		"TARGET_LINKFLAGS=$$(TARGET_LDFLAGS)" \
		"TARGET_LINK=$$(TARGET_CC)" \
		"TARGET_AR=$$(TARGET_AR)" \
		"TARGET_RANLIB=$$(TARGET_RANLIB)" \
		"STAGING_DIR=$$(STAGING_DIR)"
	# build the "chat" sample
	make -C $(PKG_BUILD_DIR)/alljoyn_core/samples/chat/linux \
		"ALLJOYN_DIST=$(PKG_BUILD_DIR)/build/openwrt/openwrt/release/dist/cpp/" \
		"CXX=$$(TARGET_CXX)"

endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/include/qcc
	$(CP) $(PKG_INSTALL_DIR)/cpp/inc/qcc/*.h $(1)/usr/include/qcc/
	$(INSTALL_DIR) $(1)/usr/include/qcc/posix
	$(CP) $(PKG_INSTALL_DIR)/cpp/inc/qcc/posix/*.h $(1)/usr/include/qcc/posix/
	$(INSTALL_DIR) $(1)/usr/include/alljoyn
	$(CP) $(PKG_INSTALL_DIR)/cpp/inc/alljoyn/*.h $(1)/usr/include/alljoyn/
	$(INSTALL_DIR) $(1)/usr/include/alljoyn_c
	$(CP) $(PKG_INSTALL_DIR)/c/inc/alljoyn_c/*.h $(1)/usr/include/alljoyn_c/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/cpp/lib/liballjoyn.a $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/c/lib/liballjoyn_c.a $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/c/lib/liballjoyn_c.so $(1)/usr/lib/
endef

define Package/alljoyn/conffiles
/etc/config/alljoyn
endef

define Package/alljoyn/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/cpp/bin/alljoyn-daemon $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/alljoyn_core/samples/chat/linux/chat $(1)/usr/bin/alljoyn-chat
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/alljoyn.init $(1)/etc/init.d/alljoyn
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./files/alljoyn.defaults $(1)/etc/uci-defaults/alljoyn
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/alljoyn.config $(1)/etc/config/alljoyn
endef

define Package/alljoyn-c/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/c/lib/liballjoyn_c.so $(1)/usr/lib/
endef

define Package/alljoyn/postinst
#!/bin/sh
if [ -z "$${IPKG_INSTROOT}" ]; then
    . /etc/uci-defaults/alljoyn
    rm -f /etc/uci-defaults/alljoyn
fi
exit 0
endef

define Package/alljoyn/prerm
#!/bin/sh
if [ -z "$${IPKG_INSTROOT}" ]; then
    /etc/init.d/alljoyn stop
    /etc/init.d/alljoyn disable
    uci batch <<EOF
del_list ucitrack.@network[-1].affects=alljoyn
delete ucitrack.@alljoyn[-1]
commit ucitrack
EOF
fi
exit 0
endef


$(eval $(call BuildPackage,alljoyn))
$(eval $(call BuildPackage,alljoyn-c))

