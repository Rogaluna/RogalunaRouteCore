cmake_minimum_required(VERSION 3.16)

project(Test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
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

set(LIB_ROGALUNA_ROUTE_CORE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../RogalunaRouteCore/build/${OUTPUT_DIR_SUFFIX})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 查找库文件
find_library(ROGALUNA_ROUTE_CORE_LIBRARY
    NAMES RogalunaRouteCore RogalunaRouteCored
    PATHS ${LIB_ROGALUNA_ROUTE_CORE_ROOT}/lib
          ${LIB_ROGALUNA_ROUTE_CORE_ROOT}/bin
    NO_DEFAULT_PATH
)

# 查找头文件路径
find_path(ROGALUNA_ROUTE_CORE_INCLUDE
    NAMES RogalunaRouteCore_Global.h
    PATHS ${LIB_ROGALUNA_ROUTE_CORE_ROOT}/include
    NO_DEFAULT_PATH
)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Test
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Page/HomePage.h Page/HomePage.cpp Page/HomePage.ui

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Test APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Test SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Test
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Test PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# 检查是否找到
if(ROGALUNA_ROUTE_CORE_LIBRARY AND ROGALUNA_ROUTE_CORE_INCLUDE)
    message(STATUS "Found library: ${ROGALUNA_ROUTE_CORE_LIBRARY}")
    message(STATUS "Found include dir: ${ROGALUNA_ROUTE_CORE_INCLUDE}")

    # 链接到目标
    target_link_libraries(Test PUBLIC ${ROGALUNA_ROUTE_CORE_LIBRARY})
    target_include_directories(Test PUBLIC ${ROGALUNA_ROUTE_CORE_INCLUDE})
else()
    message(FATAL_ERROR "Could not find required library")
endif()

# 包含目录配置
target_include_directories(Test PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Test)
endif()
set_target_properties(Test PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Test
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Test)
endif()
