cmake_minimum_required (VERSION 3.5)

if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

project (spg)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall" )

set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -W -Wfloat-equal -Wundef -Wpointer-arith" )
set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -Wwrite-strings" )
set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -Wredundant-decls -Wchar-subscripts -Wcomment -Wformat=2" )
set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -Wwrite-strings -Wmissing-declarations -Wredundant-decls" )
set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -Wswitch-enum" )

set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -Winit-self -Wmissing-field-initializers" )
set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -Waddress -Wmissing-noreturn -Wstrict-overflow=1" )
set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -Wextra -Warray-bounds" )
set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -pedantic" )

set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -W-unused-parameter" )
set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -W-unused-variable" )
set( CMAKE_CXX_FLAGS  "${CMAKE_C_FLAGS} -Wuninitialized" )

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug CACHE STRING
       "Choose the type of build, options are: Debug Release."
       FORCE )
endif()

string(TOUPPER "${CMAKE_BUILD_TYPE}" UPPERCASE_CMAKE_BUILD_TYPE)

if (NOT UPPERCASE_CMAKE_BUILD_TYPE MATCHES "^(DEBUG|RELEASE)$")
  message(FATAL_ERROR "Invalid value for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}." )

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory (sj)
add_subdirectory (util)
