cmake_minimum_required(VERSION 3.10)

project(
  Skribble
  LANGUAGES CXX
  VERSION 0.1)

include(cmake/CompilerWarnings.cmake)
include(cmake/Coverage.cmake)
include(cmake/StandardProjectSettings.cmake)
include(cmake/Sanitizers.cmake)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

enable_sanitizers(project_options)
enable_coverage(project_options)

add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(
  Qt5
  COMPONENTS Widgets Qml Quick
  REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/)

option(ENABLE_TESTS "Build Skribble's tests" ON)

if(ENABLE_TESTS)
  enable_testing()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/)
endif()
