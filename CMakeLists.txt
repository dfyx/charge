cmake_minimum_required(VERSION 2.6)
project(CHARGE)

# no ppc because it's untested (and do we need it at all?)
set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CHARGE_SOURCE_DIR})

find_package(glut)
find_package(opengl)
find_package(Box2D)
include_directories(${BOX2D_INCLUDE_DIRS})

if(EXISTS ${GLUT_INCLUDE_DIR}/GL/glut.h)
	message("Found at GL/glut.h")
	SET(GLUT_HEADER GL/glut.h)
else(EXISTS ${GLUT_INCLUDE_DIR}/GL/glut.h)
	message("Not found, assuming glut/glut.h")
	SET(GLUT_HEADER glut/glut.h)
endif(EXISTS ${GLUT_INCLUDE_DIR}/GL/glut.h)

if(MSVC)
	set(CHARGE_DEFINES ${CHARGE_DEFINES} _USE_MATH_DEFINES)
endif(MSVC)

configure_file(${CHARGE_SOURCE_DIR}/src/glut_wrap.h.in ${CHARGE_BINARY_DIR}/src/glut_wrap.h)
include_directories(${CHARGE_BINARY_DIR}/src)

file(GLOB_RECURSE CHARGE_SOURCES ${CHARGE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE CHARGE_HEADERS ${CHARGE_SOURCE_DIR}/src/*.h)
add_executable(charge ${CHARGE_SOURCES} ${CHARGE_HEADERS})
target_link_libraries(charge ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${BOX2D_LIBRARIES})
set_property(TARGET charge PROPERTY COMPILE_DEFINITIONS ${CHARGE_DEFINES})
