cmake_minimum_required(VERSION 3.19)

file(READ "${CMAKE_SOURCE_DIR}/VERSION" VER_RAW)
string(STRIP ${VER_RAW} HYPRPWCENTER_VERSION)

add_compile_definitions(HYPRPWCENTER_VERSION="${HYPRPWCENTER_VERSION}")

project(
  hyprpwcenter
  VERSION ${HYPRPWCENTER_VERSION}
  DESCRIPTION "Volume management center for Hyprland")

include(CTest)
include(CheckIncludeFile)
include(GNUInstallDirs)

set(PREFIX ${CMAKE_INSTALL_PREFIX})
set(INCLUDE ${CMAKE_INSTALL_FULL_INCLUDEDIR})
set(LIBDIR ${CMAKE_INSTALL_FULL_LIBDIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(
  deps
  REQUIRED
  IMPORTED_TARGET
  libpipewire-0.3
  hyprtoolkit
  pixman-1
  libdrm
  hyprutils>=0.10.4
)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(
  -Wall
  -Wextra
  -Wno-unused-parameter
  -Wno-unused-value
  -Wno-missing-field-initializers
  -Wpedantic)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES DEBUG)
  message(STATUS "Configuring hyprpwcenter in Debug")
  add_compile_definitions(hyprpwcenter_DEBUG)
else()
  add_compile_options(-O3)
  message(STATUS "Configuring hyprpwcenter in Release")
endif()

file(GLOB_RECURSE SRCFILES CONFIGURE_DEPENDS "src/*.cpp" "include/*.hpp")

add_executable(hyprpwcenter ${SRCFILES})

target_link_libraries(hyprpwcenter PkgConfig::deps)

install(
  FILES contrib/hyprpwcenter.desktop
  DESTINATION "share/applications")

install(TARGETS hyprpwcenter)