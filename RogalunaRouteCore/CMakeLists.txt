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

file(GLOB_RECURSE SOURCES
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/Private/*.cpp"
)

file(GLOB_RECURSE HEADERS
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/Public/*.h"
)

# 构建目标库
add_library(RogalunaRouteCore SHARED
    ${SOURCES}
    ${HEADERS}
)

# 导出宏定义（用于符号可见性）
target_compile_definitions(RogalunaRouteCore PRIVATE
    ROGALUNAROUTECORE_LIBRARY
)

# 包含目录配置
target_include_directories(RogalunaRouteCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/Public/Interface
)

# 依赖链接配置
target_link_libraries(RogalunaRouteCore PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

add_custom_command(TARGET RogalunaRouteCore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Public"
        "${CMAKE_INCLUDE_OUTPUT_DIRECTORY}"
    COMMENT "Copying headers to build directory..."
    VERBATIM
)
