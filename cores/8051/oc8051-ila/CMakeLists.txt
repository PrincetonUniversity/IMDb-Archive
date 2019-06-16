cmake_minimum_required(VERSION 3.8)

# ---------------------------------------------------------------------------- #
# PROJECT
# name version language
# ---------------------------------------------------------------------------- #
project(oc8051 VERSION 1.0 LANGUAGES CXX)

# ---------------------------------------------------------------------------- #
# VARIABLE
# alias and configurations
# ---------------------------------------------------------------------------- #
set(MyTarget ${PROJECT_NAME})

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# ---------------------------------------------------------------------------- #
# External dependencies
# ---------------------------------------------------------------------------- #
##
## ilang
##
find_package(ilang REQUIRED 0.9.1)


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${MyTarget} 
  app/main.cc
)

target_include_directories(${MyTarget} PRIVATE include)

target_link_libraries(${MyTarget} ilang::ilang)

