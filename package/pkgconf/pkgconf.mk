################################################################################
#
# pkgconf
#
################################################################################

PKGCONF_VERSION = 1.6.3
PKGCONF_SITE = https://distfiles.ariadne.space/pkgconf
PKGCONF_SOURCE = pkgconf-$(PKGCONF_VERSION).tar.xz
PKGCONF_LICENSE = pkgconf license
PKGCONF_LICENSE_FILES = COPYING
PKGCONF_CPE_ID_VENDOR = pkgconf

# We are a ccache dependency, so we can't use ccache
HOST_PKGCONF_CONF_ENV = \
	CC="$(HOSTCC_NOCCACHE)" \
	CXX="$(HOSTCXX_NOCCACHE)"

PKG_CONFIG_HOST_BINARY = $(HOST_DIR)/bin/pkg-config

define PKGCONF_LINK_PKGCONFIG
	ln -sf pkgconf $(TARGET_DIR)/usr/bin/pkg-config
endef

define HOST_PKGCONF_INSTALL_WRAPPER
	$(INSTALL) -m 0755 -D package/pkgconf/pkg-config.in \
		$(HOST_DIR)/bin/pkg-config
	$(SED) 's,@STAGING_SUBDIR@,$(STAGING_SUBDIR),g' \
		$(HOST_DIR)/bin/pkg-config
endef

define HOST_PKGCONF_STATIC
	$(SED) 's,@STATIC@,--static,' $(HOST_DIR)/bin/pkg-config
endef

define HOST_PKGCONF_SHARED
	$(SED) 's,@STATIC@,,' $(HOST_DIR)/bin/pkg-config
endef

PKGCONF_POST_INSTALL_TARGET_HOOKS += PKGCONF_LINK_PKGCONFIG
HOST_PKGCONF_POST_INSTALL_HOOKS += HOST_PKGCONF_INSTALL_WRAPPER

ifeq ($(LINGMO_STATIC_LIBS),y)
HOST_PKGCONF_POST_INSTALL_HOOKS += HOST_PKGCONF_STATIC
else
HOST_PKGCONF_POST_INSTALL_HOOKS += HOST_PKGCONF_SHARED
endif

$(eval $(autotools-package))
$(eval $(host-autotools-package))
