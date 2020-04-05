cmake_minimum_required(VERSION 3.9.6)

# ---------------------------------------------------------------------------- #
# PROJECT
# name version language
# ---------------------------------------------------------------------------- #
project(rbm_verify VERSION 1.0 LANGUAGES CXX)

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
find_package(ilang REQUIRED)




# ---------------------------------------------------------------------------- #
# TARGET
# library
# ---------------------------------------------------------------------------- #
add_library(${MyTarget}
  src/rbm_compute.cc
  src/rbm_util.cc
  src/rbm_ila.cc
)

target_include_directories(${MyTarget} PRIVATE include)

target_link_libraries(${MyTarget} PUBLIC ilang::ilang)



# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${MyTarget}Exe
  app/main.cc
)

target_include_directories(${MyTarget}Exe PRIVATE include)

target_link_libraries(${MyTarget}Exe ${MyTarget})


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${MyTarget}AQed
  app/aqed-support.cc
)

target_include_directories(${MyTarget}AQed PRIVATE include)

target_link_libraries(${MyTarget}AQed ${MyTarget})
