################################################################################
#
# ogre
#
################################################################################

OGRE_VERSION = v1.12.12
OGRE_SITE = https://github.com/OGRECave/ogre
OGRE_SITE_METHOD = git
OGRE_LICENSE = MIT (main library, DeferredShadingMedia samples), Public Domain (samples and plugins)
OGRE_LICENSE_FILES = LICENSE
OGRE_INSTALL_STAGING = YES

# Download with imgui submodule (https://github.com/ocornut/imgui
OGRE_GIT_SUBMODULES = YES

OGRE_DEPENDENCIES = host-pkgconf \
	freetype \
	libfreeimage \
	libgl \
	pugixml \
	sdl2 \
	xlib_libX11 \
	xlib_libXaw \
	xlib_libXext \
	xlib_libXrandr \
	zziplib

OGRE_CFLAGS = $(TARGET_CFLAGS) -DGLEW_NO_GLU
OGRE_CXXFLAGS = $(TARGET_CXXFLAGS) -DGLEW_NO_GLU

# Unbundle freetype and zziplib.
# Disable java and nvidia cg support.
# Disable imgui overlay to avoid extra download from CMake.
OGRE_CONF_OPTS = -DOGRE_BUILD_DEPENDENCIES=OFF \
	-DOGRE_BUILD_COMPONENT_CSHARP=OFF \
	-DOGRE_BUILD_COMPONENT_JAVA=OFF \
	-DOGRE_BUILD_PLUGIN_CG=OFF \
	-DOGRE_BUILD_COMPONENT_OVERLAY_IMGUI=OFF \
	-DOGRE_INSTALL_DOCS=OFF \
	-DCMAKE_C_FLAGS="$(OGRE_CFLAGS)" \
	-DCMAKE_CXX_FLAGS="$(OGRE_CXXFLAGS)"

# Enable optional python component if python interpreter is present on the target.
ifeq ($(LINGMO_PACKAGE_PYTHON3),y)
OGRE_DEPENDENCIES += host-swig host-python3
OGRE_CONF_OPTS += -DOGRE_BUILD_COMPONENT_PYTHON=ON
else
OGRE_CONF_OPTS += -DOGRE_BUILD_COMPONENT_PYTHON=OFF
endif

# Uses __atomic_fetch_add_8
ifeq ($(LINGMO_TOOLCHAIN_HAS_LIBATOMIC),y)
OGRE_CXXFLAGS += -latomic
endif

$(eval $(cmake-package))
