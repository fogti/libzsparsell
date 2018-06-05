cmake_minimum_required(VERSION 3.7)

add_library(zsparsell SHARED escape/char.cxx escape/cstr.cxx escape/str.cxx lowlevel.c lowlevel/MMAPguard.cxx random.cxx)
set_target_properties(zsparsell PROPERTIES VERSION "0.0.0" SOVERSION 0)

# === INSTALL SECTION ===
install(TARGETS zsparsell DESTINATION "${INSTALL_LIB_DIR}")

if(WIN32)
  install(FILES lowlevel/mman.h DESTINATION "${INSTALL_INCLUDE_DIR}")
endif()
if(WITH_ZSIG)
  install(FILES lowlevel/zsig.h DESTINATION "${INSTALL_INCLUDE_DIR}")
endif()

install(FILES Backtrack.hpp escape/escape.hpp LexPos.hpp lowlevel/MMAPguard.hpp random.hpp reverse.hpp
        DESTINATION "${INSTALL_INCLUDE_DIR}")
