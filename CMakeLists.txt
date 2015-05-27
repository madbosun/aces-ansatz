cmake_minimum_required (VERSION 2.8 FATAL_ERROR)

project(aces-ansatz)

#enable_language(Fortran)

set(ANSATZ_SOURCEFILES;
src/core/core.cpp
src/core/global_molecule_data.cpp
src/core/global_state.cpp
src/core/interupt.cpp
src/core/io_utils.cpp
src/include/abort.h
src/include/core.h
src/include/flags.h
src/include/global_state.h
src/include/input_reader.h
src/include/io_utils.h
src/include/molecule_data.h
src/include/setup_handler.h
src/input/asv.cpp
src/input/cartesian_manipulator.cpp
src/input/input_reader.cpp
src/input/keyword_handler.cpp
src/input/keyword_parser.cpp
src/setup/atomic_constants.cpp
src/setup/setup_handler.cpp)

configure_file(${CMAKE_SOURCE_DIR}/test/hf2.zmat ${CMAKE_BINARY_DIR}/ COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/test/g.bas ${CMAKE_BINARY_DIR}/ COPYONLY)

#include(FortranCInterface)
#FortranCInterface_HEADER(fc.h MACRO_NAMESPACE "FC_")
#FortranCInterface_VERIFY()

add_executable(acesansatz;
    ${ANSATZ_SOURCEFILES};
    src/main/main.cpp)

include_directories(
    src/include
    ${CMAKE_BINARY_DIR})

# To deal with the ACES2 part in the initialization executable
# add_definitions(-D__fortran -D__fortran77)
#set (CMAKE_Fortran_FLAGS "-D__fortran -D__fortran77")

