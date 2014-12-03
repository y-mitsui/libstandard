# Copyright (C) 2007-2011 Glenn Randers-Pehrson

# This code is released under the libpng license.
# For conditions of distribution and use, see the disclaimer
# and license in png.h
macro(_png_generate_symlink_code CODE SRC DEST)
  if(WIN32 AND NOT CYGWIN)
    set(_png_gsc_message "Copying ${SRC} to ${DEST} if needed")
    set(_png_gsc_operation "copy_if_different")
  else()
    set(_png_gsc_message "Symlinking ${SRC} to ${DEST}")
    set(_png_gsc_operation "create_symlink")
  endif()

  set(${CODE} "
    message(STATUS \"${_png_gsc_message}\")
    execute_process(COMMAND \${CMAKE_COMMAND} -E ${_png_gsc_operation}
      \"${SRC}\" \"${DEST}\")
    ")
endmacro()
#find_package(LibXml2 REQUIRED)
#${CMAKE_INSTALL_PREFIX}

cmake_minimum_required (VERSION 2.6)
project (libstandard)

EXEC_PROGRAM(gsl-config ARGS --libs OUTPUT_VARIABLE GSL_LIBS)
EXEC_PROGRAM(gsl-config ARGS --cflags OUTPUT_VARIABLE GSL_CFLAGS)
SET(CMAKE_C_FLAGS "-Wall -g ${GSL_CFLAGS}")
ADD_LIBRARY(standard STATIC db.c)

INSTALL(TARGETS standard ARCHIVE DESTINATION lib)
INSTALL(FILES libstandard.h DESTINATION include)

add_executable (example example.c)
target_link_libraries(example standard m -L/usr/local/lib  ${GSL_LIBS})

set(PNGLIB_NAME standard)

# CREATE PKGCONFIG FILES
# we use the same files like ./configure, so we have to set its vars
if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR lib)
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)
set(prefix      ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
set(libdir      ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(includedir  ${CMAKE_INSTALL_PREFIX}/include)
set(LIBS        "${GSL_LIBS} -lstandard")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/standard.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PNGLIB_NAME}.pc @ONLY)
_png_generate_symlink_code(PNG_PC_INSTALL_CODE
  ${CMAKE_CURRENT_BINARY_DIR}/${PNGLIB_NAME}.pc
  ${CMAKE_CURRENT_BINARY_DIR}/standard.pc)
install(CODE ${PNG_PC_INSTALL_CODE})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/standard.pc
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

