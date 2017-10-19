#
# Copyright (C) 2006-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ar750-sta-watch
PKG_VERSION:=1
PKG_RELEASE:=1



include $(INCLUDE_DIR)/package.mk



define Package/ar750-sta-watch
  SECTION:=base
  CATEGORY:=gl-inet
  TITLE+= ar750-sta-watch
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
	$(CP) ./files/* $(PKG_BUILD_DIR)
endef

define Build/Configure
endef


define Package/ar750-sta-watch/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sta-watch $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ar750-sta-watch $(1)/etc/init.d/
	
endef



$(eval $(call BuildPackage,ar750-sta-watch))