# override.cmake - 优化后的 Poppler 构建选项配置

# 基础构建配置
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build as shared library (recommended for Qt)" FORCE)
set(ENABLE_RELOCATABLE ON CACHE BOOL "Enable relocatable library on Windows" FORCE)

# 前端接口配置 - 只启用 Qt6
set(ENABLE_QT6 ON CACHE BOOL "Enable Qt6 wrapper (required)" FORCE)
set(ENABLE_QT5 OFF CACHE BOOL "Disable Qt5 wrapper" FORCE)
set(ENABLE_GLIB OFF CACHE BOOL "Disable GLib wrapper" FORCE)
set(ENABLE_CPP OFF CACHE BOOL "Disable C++ wrapper" FORCE)

# 测试程序配置
set(BUILD_QT6_TESTS OF CACHE BOOL "Enable Qt6 tests (recommended for verification)" FORCE)
set(BUILD_GTK_TESTS OFF CACHE BOOL "Disable GTK tests" FORCE)
set(BUILD_QT5_TESTS OFF CACHE BOOL "Disable Qt5 tests" FORCE)
set(BUILD_CPP_TESTS OFF CACHE BOOL "Disable CPP tests" FORCE)
set(BUILD_MANUAL_TESTS OFF CACHE BOOL "Disable manual tests" FORCE)

# 功能模块配置
set(ENABLE_UTILS OFF CACHE BOOL "Disable command line utils" FORCE)
set(ENABLE_BOOST OFF CACHE BOOL "Disable Boost (unless Splash backend needed)" FORCE)
set(ENABLE_LCMS ON CACHE BOOL "Enable color management support" FORCE)

# 文档和自省配置
set(ENABLE_GTK_DOC OFF CACHE BOOL "Disable GTK docs" FORCE)
set(ENABLE_GOBJECT_INTROSPECTION OFF CACHE BOOL "Disable GObject introspection" FORCE)
set(INSTALL_GLIB_DEMO OFF CACHE BOOL "Disable glib demo" FORCE)

# 安全/加密相关
set(ENABLE_NSS3 OFF CACHE BOOL "Disable NSS3 crypto support" FORCE)
set(ENABLE_GPGME OFF CACHE BOOL "Disable GPGME support" FORCE)
set(ENABLE_PGP_SIGNATURES OFF CACHE BOOL "Disable PGP signatures" FORCE)

# 解码器配置
set(ENABLE_LIBOPENJPEG "openjpeg2" CACHE STRING "Use openjpeg2 for JPX streams" FORCE)
set(ENABLE_DCTDECODER "libjpeg" CACHE STRING "Use libjpeg for DCT streams" FORCE)
set(ENABLE_LIBTIFF ON CACHE BOOL "Enable TIFF support" FORCE)
set(ENABLE_ZLIB_UNCOMPRESS OFF CACHE BOOL "Disable zlib uncompress" FORCE)

# 性能选项
set(USE_FLOAT OFF CACHE BOOL "Use double precision arithmetic" FORCE)

# 工具配置
set(RUN_GPERF_IF_PRESENT ON CACHE BOOL "Enable gperf if available" FORCE)

# 不稳定的API
set(ENABLE_UNSTABLE_API_ABI_HEADERS OFF CACHE BOOL "Disable unstable headers" FORCE)

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
set(ENABLE_RELOCATABLE ON CACHE BOOL "Enable relocatable library on Windows" FORCE)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/shared/" CACHE PATH "Installation directory" FORCE)
# 设置安装目录结构
set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "Library directory")
set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "Include directory")
set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "Binary directory")