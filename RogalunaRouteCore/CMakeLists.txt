cmake_minimum_required(VERSION 3.16)
project(RogalunaRouteCore VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

file(GLOB_RECURSE SOURCES
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE HEADERS
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# 依赖链接配置
target_link_libraries(RogalunaRouteCore PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

# 安装规则（可选）
install(TARGETS RogalunaRouteCore
    EXPORT RogalunaRouteCoreTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${HEADERS}
    DESTINATION include/RogalunaRouteCore
)
