add_library(zsparsell SHARED escape/char.cxx escape/cstr.cxx escape/str.cxx
                             lexer/get_line.cxx random.cxx)

set_target_properties (zsparsell PROPERTIES VERSION "1.0.0" SOVERSION 1)
target_compile_options(zsparsell PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
target_link_libraries (zsparsell LowlevelZS::lowlevelzs)

set_source_files_properties(escape/cstr.cxx escape/char.cxx lexer/get_line.cxx
  PROPERTIES COMPILE_FLAGS "-fno-exceptions -fno-unwind-tables")

# === INSTALL SECTION ===
install(TARGETS zsparsell DESTINATION "${INSTALL_LIB_DIR}")

install(FILES escape/escape.hpp hash.hpp
    lexer/Backtrack.hpp lexer/InputSlice.hpp lexer/LexPos.hpp lexer/lexer_get_line.hpp
    lowlevel/zsig.h lowlevel/mman.h lowlevel/MMAPguard.hpp
    random.hpp reverse.hpp
  DESTINATION "${INSTALL_INCLUDE_DIR}")
