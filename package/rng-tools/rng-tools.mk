################################################################################
#
# rng-tools
#
################################################################################

RNG_TOOLS_VERSION = 6.16
RNG_TOOLS_SITE = $(call github,nhorman,rng-tools,v$(RNG_TOOLS_VERSION))
RNG_TOOLS_LICENSE = GPL-2.0
RNG_TOOLS_LICENSE_FILES = COPYING
RNG_TOOLS_CPE_ID_VALID = YES
RNG_TOOLS_SELINUX_MODULES = rngd

RNG_TOOLS_DEPENDENCIES = host-pkgconf libcap openssl
# From git
RNG_TOOLS_AUTORECONF = YES

RNG_TOOLS_CONF_OPTS = --without-pkcs11

# Work around for uClibc or musl toolchains which lack argp_*()
# functions.
ifeq ($(LINGMO_PACKAGE_ARGP_STANDALONE),y)
RNG_TOOLS_CONF_ENV += LIBS="-largp $(TARGET_NLS_LIBS)"
RNG_TOOLS_DEPENDENCIES += argp-standalone $(TARGET_NLS_DEPENDENCIES)
endif

ifeq ($(LINGMO_PACKAGE_LIBRTLSDR),y)
RNG_TOOLS_DEPENDENCIES += librtlsdr
RNG_TOOLS_CONF_OPTS += --with-rtlsdr
else
RNG_TOOLS_CONF_OPTS += --without-rtlsdr
endif

ifeq ($(LINGMO_PACKAGE_RNG_TOOLS_JITTERENTROPY_LIBRARY),y)
RNG_TOOLS_DEPENDENCIES += jitterentropy-library
RNG_TOOLS_CONF_OPTS += --enable-jitterentropy
else
RNG_TOOLS_CONF_OPTS += --disable-jitterentropy
endif

ifeq ($(LINGMO_PACKAGE_RNG_TOOLS_NISTBEACON),y)
RNG_TOOLS_DEPENDENCIES += jansson libcurl libxml2
RNG_TOOLS_CONF_OPTS += --with-nistbeacon
else
RNG_TOOLS_CONF_OPTS += --without-nistbeacon
endif

ifeq ($(LINGMO_PACKAGE_RNG_TOOLS_QRYPT),y)
RNG_TOOLS_DEPENDENCIES += jansson libcurl
RNG_TOOLS_CONF_OPTS += --with-qrypt
else
RNG_TOOLS_CONF_OPTS += --without-qrypt
endif

define RNG_TOOLS_INSTALL_INIT_SYSV
	$(INSTALL) -D -m 755 package/rng-tools/S21rngd \
		$(TARGET_DIR)/etc/init.d/S21rngd
endef

define RNG_TOOLS_INSTALL_INIT_SYSTEMD
	$(INSTALL) -D -m 644 package/rng-tools/rngd.service \
		$(TARGET_DIR)/usr/lib/systemd/system/rngd.service
endef

$(eval $(autotools-package))
