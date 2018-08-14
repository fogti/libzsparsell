cmake_minimum_required(VERSION 3.7)
project(zsparsell)

set(INSTALL_LIB_DIR     lib${LIB_SUFFIX}   CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR include/zs/parsell CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR  share/cmake/Modules CACHE PATH "Installation directory for CMake module files")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${PROJECT_SOURCE_DIR}/src")

find_package(LowlevelZS REQUIRED)

add_subdirectory(cmake)
add_subdirectory(src)
