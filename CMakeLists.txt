cmake_minimum_required(VERSION 2.6)
project(CHARGE)

# no ppc because it's untested (and do we need it at all?)
set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")

include_directories(/usr/local/include)

find_package(glut)
find_package(opengl)

if(EXISTS ${GLUT_INCLUDE_DIR}/GL/glut.h)
	message("Found at GL/glut.h")
	SET(GLUT_HEADER GL/glut.h)
else(EXISTS ${GLUT_INCLUDE_DIR}/GL/glut.h)
	message("Not found, assuming glut/glut.h")
	SET(GLUT_HEADER glut/glut.h)
endif(EXISTS ${GLUT_INCLUDE_DIR}/GL/glut.h)

configure_file(${CHARGE_SOURCE_DIR}/src/glut_wrap.h.in ${CHARGE_BINARY_DIR}/src/glut_wrap.h)
include_directories(${CHARGE_BINARY_DIR}/src)

file(GLOB_RECURSE CHARGE_SOURCES ${CHARGE_SOURCE_DIR}/src/*.cpp)
add_executable(charge ${CHARGE_SOURCES})
target_link_libraries(charge ${GLUT_LIBRARY} ${OPENGL_LIBRARY} -lBox2D)