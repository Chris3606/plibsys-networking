cmake_minimum_required(VERSION 3.1)

if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE Debug)
ENDIF ()

project(plibsys-networking)
find_library(PLIBSYS  plibsys ${CMAKE_CURRENT_SOURCE_DIR}/libs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/plibsys)

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${PLIBSYS})

IF (MSVC) # Copy dynamic dll as necessary
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
					   "${CMAKE_CURRENT_SOURCE_DIR}/libs/plibsys.dll"
					   $<TARGET_FILE_DIR:${PROJECT_NAME}>)
ENDIF (MSVC)
