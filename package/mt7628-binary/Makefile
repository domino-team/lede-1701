#
# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=gl-inet
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/mt7628-binary
  SECTION:=MTK Properties
  CATEGORY:=MTK Properties
  DEPENDS:=+libuci
  TITLE:=MTK 7628 Wifi driver binary for LEDE
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./files/* $(PKG_BUILD_DIR)/
endef

define Build/Compile/Default

endef
Build/Compile = $(Build/Compile/Default)

define Package/mt7628-binary/install
	$(CP) ./files/* $(1)/
endef

$(eval $(call BuildPackage,mt7628-binary))
