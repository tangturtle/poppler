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
set(ENABLE_LIBOPENJPEG "openjpeg2" CACHE STRING "Override: Use openjpeg2 for JPX streams" FORCE)
set(ENABLE_DCTDECODER "unmaintained" CACHE STRING "Override: Use libjpeg for DCT streams" FORCE)

# 验证消息
message(STATUS "1***********************************************************")
message(STATUS "Loaded override.cmake - Poppler build options overridden")

message(STATUS "2***********************************************************")
add_subdirectory(deps/zlib)
set(ZLIB_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/deps/zlib ${CMAKE_CURRENT_BINARY_DIR}/deps/zlib)
set(ZLIB_LIBRARIES zlibstatic zlib)  # zlibstatic 是静态库目标名
# 如果其他模块使用 find_package(ZLIB)
if(NOT TARGET ZLIB::ZLIB)
    add_library(ZLIB::ZLIB ALIAS zlibstatic)
endif()
set(ZLIB_FOUND TRUE)
set(ZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIRS})
set(ZLIB_LIBRARY ${ZLIB_LIBRARIES})

message(STATUS "3***********************************************************")
add_subdirectory(deps/libpng)
set(PNG_LIBRARIES png_static)  # 静态库目标名
set(PNG_INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/libpng
    ${CMAKE_CURRENT_BINARY_DIR}/deps/libpng
)
# 创建别名目标供其他模块使用
if(NOT TARGET PNG::PNG)
    add_library(PNG::PNG ALIAS png_static)
endif()
# 导出变量（如果需要）
set(PNG_FOUND TRUE)
set(PNG_LIBRARY ${PNG_LIBRARIES})
set(PNG_PNG_INCLUDE_DIR ${PNG_INCLUDE_DIRS})

message(STATUS "4***********************************************************")
set(LIBJPEG_TURBO_PREFIX "${CMAKE_BINARY_DIR}/libjpeg-turbo-install")
include(ExternalProject)
ExternalProject_Add(
    libjpeg-turbo-ep
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/libjpeg-turbo"
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${LIBJPEG_TURBO_PREFIX}
        -DENABLE_SHARED=OFF
        -DENABLE_STATIC=ON
        -DWITH_JPEG8=ON
    BUILD_BYPRODUCTS
        "${LIBJPEG_TURBO_PREFIX}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}jpeg${CMAKE_STATIC_LIBRARY_SUFFIX}"
)
add_library(jpeg-static STATIC IMPORTED)
set_target_properties(jpeg-static PROPERTIES
    IMPORTED_LOCATION "${LIBJPEG_TURBO_PREFIX}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}jpeg${CMAKE_STATIC_LIBRARY_SUFFIX}"
    INTERFACE_INCLUDE_DIRECTORIES "${LIBJPEG_TURBO_PREFIX}/include"
)
add_dependencies(jpeg-static libjpeg-turbo-ep)
# 5. 设置兼容变量
set(JPEG_FOUND TRUE)
set(JPEG_INCLUDE_DIRS "${LIBJPEG_TURBO_PREFIX}/include")
set(JPEG_LIBRARIES jpeg-static)
if(NOT TARGET JPEG::JPEG)
    add_library(JPEG::JPEG ALIAS jpeg-static)
endif()

message(STATUS "5***********************************************************")
add_subdirectory(deps/bzip2)
set(BZIP2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/bzip2)
set(BZIP2_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/deps/bzip2/libbz2.a)

message(STATUS "6***********************************************************")
# 添加 Brotli 子目录（禁用其自带的测试和工具）
add_subdirectory(deps/brotli EXCLUDE_FROM_ALL)
# 手动设置 Brotli 的头文件和库路径
set(BROTLIDEC_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/deps/brotli/c/include)
set(BROTLIDEC_LIBRARIES brotlidec)  # 库名通常为 brotlidec（静态库）或 brotlidec-static

message(STATUS "7***********************************************************")
option(FT_DISABLE_TESTS ON)
option(FT_DISABLE_EXAMPLES ON)
add_subdirectory(deps/freetype)
# 手动设置 Freetype 的头文件和库路径
set(FREETYPE_INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/freetype/include
)
set(FREETYPE_LIBRARY freetype)  # 库名通常为 freetype 或 freetype-static
