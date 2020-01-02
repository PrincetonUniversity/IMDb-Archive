cmake_minimum_required(VERSION 3.8)

# ---------------------------------------------------------------------------- #
# PROJECT
# name version language
# ---------------------------------------------------------------------------- #
project(PMESHL15 VERSION 1.0 LANGUAGES CXX)

# ---------------------------------------------------------------------------- #
# VARIABLE
# alias and configurations
# ---------------------------------------------------------------------------- #
set(PMESHL15_TARGET ${PROJECT_NAME})

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# ---------------------------------------------------------------------------- #
# External dependencies
# ---------------------------------------------------------------------------- #
##
## ilang
##
find_package(ilang REQUIRED)

##
## Python (required if CMake version < 3.12)
##
if(${CMAKE_VERSION} VERSION_LESS 3.12)

  if(NOT TARGET Python::Python)
    add_library(Python::Python IMPORTED INTERFACE)

    set_property(TARGET Python::Python 
      PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${PYTHON_INCLUDE_DIRECTORIES}
    )

    set_property(TARGET Python::Python
      PROPERTY INTERFACE_LINK_LIBRARIES ${PYTHON_LIBRARIES}
    )
    
  endif()

endif()


# ---------------------------------------------------------------------------- #
# TARGET
# library
# ---------------------------------------------------------------------------- #
add_library(${PMESHL15_TARGET}
  src/pmesh_l15_pcx_ila.cc
  src/pmesh_l15_noc2_ila.cc
  src/pmesh_util.cc
)

target_include_directories(${PMESHL15_TARGET} PRIVATE include)

target_link_libraries(${PMESHL15_TARGET} PUBLIC ilang::ilang)


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${PMESHL15_TARGET}-pcx-verify
  app/main-pcx-verify.cc
)

target_include_directories(${PMESHL15_TARGET}-pcx-verify PRIVATE include)

target_link_libraries(${PMESHL15_TARGET}-pcx-verify ${PMESHL15_TARGET})


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${PMESHL15_TARGET}-noc2-verify
  app/main-noc2-verify.cc
)

target_include_directories(${PMESHL15_TARGET}-noc2-verify PRIVATE include)

target_link_libraries(${PMESHL15_TARGET}-noc2-verify ${PMESHL15_TARGET})