#
# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=gl-inet
PKG_REFV:=2
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/mt7628-binary
  SECTION:=MTK Properties
  CATEGORY:=MTK Properties
  TITLE:=MTK 7628 Wifi driver binary for LEDE
   DEPENDS:= \
  	+kmod-rt-flash +kmod-mac80211 @PCI_SUPPORT @!LINUX_3_18 \
  	+@DRIVER_11AC_SUPPORT +@DRIVER_11N_SUPPORT +@DRIVER_11W_SUPPORT
endef

define Build/Prepare
endef

define Build/Configure
endef


define Package/mt7628-binary/install
	$(CP) -fr ./files/* $(1)/
endef

$(eval $(call BuildPackage,mt7628-binary))
