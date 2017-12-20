cmake_minimum_required(VERSION 3.1)

if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE Debug)
ENDIF ()

project(networking)
find_library(PLIBSYS  plibsys ${CMAKE_CURRENT_SOURCE_DIR}/libs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/plibsys)

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PLIBSYS})
