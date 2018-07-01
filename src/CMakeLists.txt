cmake_minimum_required(VERSION 3.7)

add_library(zsparsell SHARED escape/char.cxx escape/cstr.cxx escape/str.cxx
                             hash.c lowlevel.c lowlevel/MMAPguard.cxx random.cxx)
set_target_properties (zsparsell PROPERTIES VERSION "1.0.1" SOVERSION 1)

target_compile_options(zsparsell PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)

set_source_files_properties(hash.c PROPERTIES COMPILE_FLAGS -Os)

# === INSTALL SECTION ===
install(TARGETS zsparsell DESTINATION "${INSTALL_LIB_DIR}")

if(WITH_ZSIG)
  install(FILES lowlevel/zsig.h DESTINATION "${INSTALL_INCLUDE_DIR}")
endif()

install(FILES Backtrack.hpp escape/escape.hpp hash.hpp InputSlice.hpp LexPos.hpp
              lowlevel/mman.h lowlevel/MMAPguard.hpp random.hpp reverse.hpp
        DESTINATION "${INSTALL_INCLUDE_DIR}")
