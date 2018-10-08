add_library(zsparsell SHARED escape/char.cxx escape/cstr.cxx escape/str.cxx
                             lexer/get_line.cxx lexer/hexcode.cxx lexer/Lexer.cxx random.cxx)

set_target_properties (zsparsell PROPERTIES VERSION "2.0.0" SOVERSION 2)
target_compile_options(zsparsell PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
target_link_libraries (zsparsell LowlevelZS::lowlevelzs)

set_source_files_properties(escape/cstr.cxx escape/char.cxx lexer/get_line.cxx
  PROPERTIES COMPILE_FLAGS "-fno-exceptions -fno-unwind-tables")

# === INSTALL SECTION ===
install(TARGETS zsparsell DESTINATION "${INSTALL_LIB_DIR}" EXPORT ZSparsellTargets)
install(EXPORT ZSparsellTargets DESTINATION "${INSTALL_CMAKE_DIR}" NAMESPACE ZSparsell:: COMPONENT dev)

install(FILES escape/escape.hpp
    lexer/Backtrack.hpp lexer/InputSlice.hpp lexer/hexcode.hpp lexer/LexPos.hpp lexer/Lexer.hpp
    random.hpp reverse.hpp
  DESTINATION "${INSTALL_INCLUDE_DIR}")
