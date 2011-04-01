cmake_minimum_required(VERSION 2.6)
project(CHARGE)

find_file(Box2D_CONFIG_FILE Box2DConfig.cmake)
include(${Box2D_CONFIG_FILE})

# no ppc because it's untested (and do we need it at all?)
set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")

include_directories(/usr/local/include)

find_package(glut)
find_package(opengl)

file(GLOB_RECURSE CHARGE_SOURCES ${CHARGE_SOURCE_DIR}/src/*.cpp)
add_executable(charge ${CHARGE_SOURCES})
target_link_libraries(charge ${GLUT_LIBRARY} ${OPENGL_LIBRARY} -lBox2D)