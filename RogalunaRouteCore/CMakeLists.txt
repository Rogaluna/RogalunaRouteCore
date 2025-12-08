cmake_minimum_required(VERSION 3.16)
project(RogalunaRouteCore VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OUTPUT_DIR_SUFFIX "debug")
else()
    set(OUTPUT_DIR_SUFFIX "release")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../${OUTPUT_DIR_SUFFIX}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../${OUTPUT_DIR_SUFFIX}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../${OUTPUT_DIR_SUFFIX}/lib)
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../${OUTPUT_DIR_SUFFIX}/include)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

# 自动收集 Public 头文件
file(GLOB_RECURSE PUBLIC_HEADERS
    LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/Public/*.h
)

# 自动收集 Private 源文件
file(GLOB_RECURSE PRIVATE_SOURCES
    LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/Private/*.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(RogalunaRouteCore SHARED)

target_sources(RogalunaRouteCore
    PRIVATE ${PRIVATE_SOURCES}
    PUBLIC  ${PUBLIC_HEADERS}
)

target_include_directories(RogalunaRouteCore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    $<INSTALL_INTERFACE:include>
)

# 导出宏定义（用于符号可见性）
target_compile_definitions(RogalunaRouteCore PRIVATE
    ROGALUNAROUTECORE_LIBRARY
)

# 包含目录配置
target_include_directories(RogalunaRouteCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Interface
)

# 依赖链接配置
target_link_libraries(RogalunaRouteCore PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)
target_link_libraries(RogalunaRouteCore PRIVATE Qt6::Core)

# ===== 安装配置 =====

set(INSTALL_ROOT ${CMAKE_BINARY_DIR}/../${OUTPUT_DIR_SUFFIX})
set(CMAKE_INSTALL_PREFIX ${INSTALL_ROOT} CACHE PATH "Install prefix" FORCE)

install(TARGETS RogalunaRouteCore
    LIBRARY DESTINATION bin     # Linux/macOS .so
    RUNTIME DESTINATION bin     # Windows .dll
    ARCHIVE DESTINATION lib     # Windows .lib / Linux .a
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Public/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
