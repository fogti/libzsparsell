cmake_minimum_required(VERSION 3.7)

add_library(zsparsell SHARED escape/char.cxx escape/cstr.cxx escape/str.cxx hash.c
                             lexer/get_line.cxx lowlevel.c lowlevel/MMAPguard.cxx random.cxx)

set_target_properties (zsparsell PROPERTIES VERSION "1.0.1" SOVERSION 1)
target_compile_options(zsparsell PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)

set_source_files_properties(escape/cstr.cxx escape/char.cxx lexer/get_line.cxx
  PROPERTIES COMPILE_FLAGS "-fno-exceptions -fno-unwind-tables")

# === INSTALL SECTION ===
install(TARGETS zsparsell DESTINATION "${INSTALL_LIB_DIR}")

if(WITH_ZSIG)
  install(FILES lowlevel/zsig.h DESTINATION "${INSTALL_INCLUDE_DIR}")
endif()

install(FILES escape/escape.hpp hash.hpp
    lexer/Backtrack.hpp lexer/InputSlice.hpp lexer/LexPos.hpp lexer/lexer_get_line.hpp
    lowlevel/mman.h lowlevel/MMAPguard.hpp
    random.hpp reverse.hpp
  DESTINATION "${INSTALL_INCLUDE_DIR}")
