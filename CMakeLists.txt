cmake_minimum_required (VERSION 2.4)
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



EXEC_PROGRAM(xml2-config ARGS --libs OUTPUT_VARIABLE XML2_LIBS)
EXEC_PROGRAM(xml2-config ARGS --cflags OUTPUT_VARIABLE XML2_CFLAGS)
EXEC_PROGRAM(pkg-config ARGS --libs glib-2.0 OUTPUT_VARIABLE GLIB_LIBS)
EXEC_PROGRAM(pkg-config ARGS --cflags glib-2.0 OUTPUT_VARIABLE GLIB_CFLAGS)
EXEC_PROGRAM(pkg-config ARGS --libs apr-1 OUTPUT_VARIABLE APR_LIBS)
EXEC_PROGRAM(pkg-config ARGS --cflags apr-1 OUTPUT_VARIABLE APR_CFLAGS)
EXEC_PROGRAM(pkg-config ARGS --libs dpgmm OUTPUT_VARIABLE DPGMM_LIBS)
EXEC_PROGRAM(pkg-config ARGS --cflags dpgmm OUTPUT_VARIABLE DPGMM_CFLAGS)
EXEC_PROGRAM(mysql_config ARGS --cflags OUTPUT_VARIABLE MYSQL_CFLAGS)
EXEC_PROGRAM(mysql_config ARGS --libs OUTPUT_VARIABLE MYSQL_LIBS)
SET(CMAKE_C_FLAGS "-Wall -g -O0 ${XML2_CFLAGS} ${GLIB_CFLAGS} ${APR_CFLAGS} ${DPGMM_CFLAGS} ${MYSQL_CFLAGS}")
#SET(CMAKE_BUILD_TYPE Release)

project (standard C)
SET(serial "1.0.0")
SET(soserial "1")
#ADD_LIBRARY(standard SHARED predict.c db.c xpath.c file.c string.c xml.c mathematics.c)
ADD_LIBRARY(standard STATIC xml.c predict.c db.c xpath.c file.c string.c mathematics.c gsl.c lognormal.c)
SET_TARGET_PROPERTIES(standard PROPERTIES VERSION ${serial} SOVERSION ${soserial})
INSTALL(TARGETS standard ARCHIVE DESTINATION lib)
#INSTALL(TARGETS standard LIBRARY DESTINATION lib)
INSTALL(FILES libstandard.h DESTINATION include)

add_executable (example example.c predict.c db.c xpath.c file.c string.c xml.c mathematics.c gsl.c lognormal.c)
add_executable (example2 example2.c predict.c db.c xpath.c file.c string.c xml.c mathematics.c gsl.c lognormal.c)
add_executable (example3 example3.c predict.c db.c xpath.c file.c string.c xml.c mathematics.c gsl.c lognormal.c)
add_executable (example4 example4.c predict.c db.c xpath.c file.c string.c xml.c mathematics.c gsl.c lognormal.c)
add_executable (example5 example5.c predict.c db.c xpath.c file.c string.c xml.c mathematics.c gsl.c lognormal.c)
add_executable (example6 example6.c predict.c db.c xpath.c file.c string.c xml.c mathematics.c gsl.c lognormal.c)
target_link_libraries(example m -L/usr/local/lib ${GLIB_LIBS} ${APR_LIBS} ${DPGMM_LIBS} ${MYSQL_LIBS} ${XML2_LIBS})
target_link_libraries(example2 m -L/usr/local/lib ${GLIB_LIBS} ${APR_LIBS} ${DPGMM_LIBS} ${MYSQL_LIBS} ${XML2_LIBS})
target_link_libraries(example3 m -L/usr/local/lib ${GLIB_LIBS} ${APR_LIBS} ${DPGMM_LIBS} ${MYSQL_LIBS} ${XML2_LIBS})
target_link_libraries(example4 m -L/usr/local/lib ${GLIB_LIBS} ${APR_LIBS} ${DPGMM_LIBS} ${MYSQL_LIBS} ${XML2_LIBS})
target_link_libraries(example5 m -L/usr/local/lib ${GLIB_LIBS} ${APR_LIBS} ${DPGMM_LIBS} ${MYSQL_LIBS} ${XML2_LIBS})
target_link_libraries(example6 m -L/usr/local/lib ${GLIB_LIBS} ${APR_LIBS} ${DPGMM_LIBS} ${MYSQL_LIBS} ${XML2_LIBS})

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
set(LIBS        "-lstandard ${XML2_LIBS} ${GLIB_LIBS} ${APR_LIBS} ${DPGMM_LIBS} ${MYSQL_LIBS}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/standard.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PNGLIB_NAME}.pc @ONLY)
_png_generate_symlink_code(PNG_PC_INSTALL_CODE
  ${CMAKE_CURRENT_BINARY_DIR}/${PNGLIB_NAME}.pc
  ${CMAKE_CURRENT_BINARY_DIR}/standard.pc)
install(CODE ${PNG_PC_INSTALL_CODE})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/standard.pc
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

