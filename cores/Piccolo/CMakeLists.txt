cmake_minimum_required(VERSION 3.8)

# ---------------------------------------------------------------------------- #
# PROJECT
# name version language
# ---------------------------------------------------------------------------- #
project(RiscV_RV32I VERSION 1.0 LANGUAGES CXX)

# ---------------------------------------------------------------------------- #
# VARIABLE
# alias and configurations
# ---------------------------------------------------------------------------- #
set(RISCV_RV32I_TARGET ${PROJECT_NAME})

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
add_library(${RISCV_RV32I_TARGET}
  src/helpers.cc
  src/riscvIla.cc
)

target_include_directories(${RISCV_RV32I_TARGET} PRIVATE include)

target_link_libraries(${RISCV_RV32I_TARGET} PUBLIC ilang::ilang)


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${RISCV_RV32I_TARGET}Exe
  app/main.cc
)

target_include_directories(${RISCV_RV32I_TARGET}Exe PRIVATE include)

target_link_libraries(${RISCV_RV32I_TARGET}Exe ${RISCV_RV32I_TARGET})


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${RISCV_RV32I_TARGET}Exe_noreset
  app/main-noreset.cc
)

target_include_directories(${RISCV_RV32I_TARGET}Exe_noreset PRIVATE include)

target_link_libraries(${RISCV_RV32I_TARGET}Exe_noreset ${RISCV_RV32I_TARGET})



# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${RISCV_RV32I_TARGET}Exe_btor
  app/main-btor.cc
)

target_include_directories(${RISCV_RV32I_TARGET}Exe_btor PRIVATE include)

target_link_libraries(${RISCV_RV32I_TARGET}Exe_btor ${RISCV_RV32I_TARGET})


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${RISCV_RV32I_TARGET}Exe_noreset_btor
  app/main-noreset-btor.cc
)

target_include_directories(${RISCV_RV32I_TARGET}Exe_noreset_btor PRIVATE include)

target_link_libraries(${RISCV_RV32I_TARGET}Exe_noreset_btor ${RISCV_RV32I_TARGET})


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${RISCV_RV32I_TARGET}Exe_invsyn_z3
  app/inv-syn-cegar.cc
)

target_include_directories(${RISCV_RV32I_TARGET}Exe_invsyn_z3 PRIVATE include)

target_link_libraries(${RISCV_RV32I_TARGET}Exe_invsyn_z3 ${RISCV_RV32I_TARGET})


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${RISCV_RV32I_TARGET}Exe_invsyn_abc
  app/inv-syn-cegar-abc.cc
)

target_include_directories(${RISCV_RV32I_TARGET}Exe_invsyn_abc PRIVATE include)

target_link_libraries(${RISCV_RV32I_TARGET}Exe_invsyn_abc ${RISCV_RV32I_TARGET})


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${RISCV_RV32I_TARGET}Exe_relchc
  app/relchc.cc
)

target_include_directories(${RISCV_RV32I_TARGET}Exe_relchc PRIVATE include)

target_link_libraries(${RISCV_RV32I_TARGET}Exe_relchc ${RISCV_RV32I_TARGET})

