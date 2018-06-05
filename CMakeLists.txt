cmake_minimum_required(VERSION 3.7)
project(zsparsell)

set(INSTALL_LIB_DIR     lib${LIB_SUFFIX}   CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR include/zs/parsell CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR  share/cmake/Modules CACHE PATH "Installation directory for CMake module files")

option(WITH_ZSIG "zsparsell provide zsig:my_signal function" ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

foreach(i config.h)
  configure_file("${i}.in" "${PROJECT_BINARY_DIR}/${i}")
endforeach(i)

include_directories("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}")

add_subdirectory(cmake)
add_subdirectory(src)

install(FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION "${INSTALL_INCLUDE_DIR}")
