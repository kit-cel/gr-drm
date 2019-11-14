INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_DRM drm)

FIND_PATH(
    DRM_INCLUDE_DIRS
    NAMES drm/api.h
    HINTS $ENV{DRM_DIR}/include
        ${PC_DRM_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    DRM_LIBRARIES
    NAMES gnuradio-drm
    HINTS $ENV{DRM_DIR}/lib
        ${PC_DRM_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/drmTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DRM DEFAULT_MSG DRM_LIBRARIES DRM_INCLUDE_DIRS)
MARK_AS_ADVANCED(DRM_LIBRARIES DRM_INCLUDE_DIRS)
