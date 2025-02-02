################################################################################
#
# lua-messagepack
#
################################################################################

LUA_MESSAGEPACK_VERSION_UPSTREAM = 0.5.4
LUA_MESSAGEPACK_VERSION = $(LUA_MESSAGEPACK_VERSION_UPSTREAM)-1
ifeq ($(LINGMO_PACKAGE_LUA_5_3)$(LINGMO_PACKAGE_LUA_5_4),y)
LUA_MESSAGEPACK_NAME_UPSTREAM = lua-MessagePack-lua53
else
LUA_MESSAGEPACK_NAME_UPSTREAM = lua-MessagePack
endif
LUA_MESSAGEPACK_SUBDIR = lua-MessagePack-$(LUA_MESSAGEPACK_VERSION_UPSTREAM)
LUA_MESSAGEPACK_LICENSE = MIT
LUA_MESSAGEPACK_LICENSE_FILES = $(LUA_MESSAGEPACK_SUBDIR)/COPYRIGHT

$(eval $(luarocks-package))
