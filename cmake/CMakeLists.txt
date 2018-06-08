cmake_minimum_required(VERSION 3.0)
configure_file("${PROJECT_SOURCE_DIR}/cmake/FindZSparsell.cmake.in" "${PROJECT_BINARY_DIR}/cmake/FindZSparsell.cmake" @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/cmake/FindZSparsell.cmake" DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
