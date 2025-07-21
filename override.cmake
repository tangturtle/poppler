# override.cmake - 覆盖 Poppler 构建选项

set(CMAKE_MODULE_PATH OFF CACHE BOOL "Override: Install unstable headers" FORCE)
# 覆盖布尔选项
set(ENABLE_UNSTABLE_API_ABI_HEADERS OFF CACHE BOOL "Override: Install unstable headers" FORCE)
set(BUILD_GTK_TESTS OFF CACHE BOOL "Override: Disable GTK tests" FORCE)
set(BUILD_QT5_TESTS OFF CACHE BOOL "Override: Disable Qt5 tests" FORCE)
set(BUILD_QT6_TESTS OFF CACHE BOOL "Override: Disable Qt6 tests" FORCE)
set(BUILD_CPP_TESTS OFF CACHE BOOL "Override: Disable CPP tests" FORCE)
set(BUILD_MANUAL_TESTS OFF CACHE BOOL "Override: Disable manual tests" FORCE)
set(ENABLE_BOOST ON CACHE BOOL "Override: Enable Boost" FORCE)
set(ENABLE_UTILS OFF CACHE BOOL "Override: Disable command line utils" FORCE)
set(ENABLE_CPP ON CACHE BOOL "Override: Enable C++ wrapper" FORCE)
set(ENABLE_GLIB OFF CACHE BOOL "Override: Disable GLib wrapper" FORCE)
set(ENABLE_GOBJECT_INTROSPECTION ON CACHE BOOL "Override: Enable GObject introspection" FORCE)
set(ENABLE_GTK_DOC OFF CACHE BOOL "Override: Disable GTK docs" FORCE)
set(ENABLE_QT5 OFF CACHE BOOL "Override: Disable Qt5 wrapper" FORCE)
set(ENABLE_QT6 ON CACHE BOOL "Override: Enable Qt6 wrapper" FORCE)
set(ENABLE_LCMS ON CACHE BOOL "Override: Enable LCMS" FORCE)
set(ENABLE_LIBCURL OFF CACHE BOOL "Override: Disable libcurl" FORCE)
set(ENABLE_LIBTIFF OFF CACHE BOOL "Override: Disable libtiff" FORCE)
set(ENABLE_NSS3 OFF CACHE BOOL "Override: Disable NSS3" FORCE)
set(ENABLE_GPGME OFF CACHE BOOL "Override: Disable GPGME" FORCE)
set(ENABLE_PGP_SIGNATURES OFF CACHE BOOL "Override: Disable PGP signatures" FORCE)
set(ENABLE_ZLIB_UNCOMPRESS OFF CACHE BOOL "Override: Disable zlib uncompress" FORCE)
set(USE_FLOAT OFF CACHE BOOL "Override: Disable float arithmetic" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Override: Build static library" FORCE)
set(RUN_GPERF_IF_PRESENT OFF CACHE BOOL "Override: Disable gperf" FORCE)
set(INSTALL_GLIB_DEMO OFF CACHE BOOL "Override: Disable glib demo" FORCE)

# 覆盖字符串选项
# set(ENABLE_LIBOPENJPEG "openjpeg2" CACHE STRING "Override: Use openjpeg2 for JPX streams" FORCE)
# set(ENABLE_DCTDECODER "libjpeg" CACHE STRING "Override: Use libjpeg for DCT streams" FORCE)

# 验证消息
message(STATUS "Loaded override.cmake - Poppler build options overridden")

