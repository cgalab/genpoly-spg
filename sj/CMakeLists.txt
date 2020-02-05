# manually inserted..
add_definitions(-O3 -DNDEBUG -std=c++11 -DCPUTIME_IN_MILLISECONDS)

#set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall" )

#set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -W -Wfloat-equal -Wundef -Wpointer-arith" )
#set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wwrite-strings" )
#set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wredundant-decls -Wchar-subscripts -Wcomment -Wformat=2" )
#set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wwrite-strings -Wmissing-declarations -Wredundant-decls" )
#set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wswitch-enum" )

#set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Winit-self -Wmissing-field-initializers" )
#set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Waddress -Wmissing-noreturn -Wstrict-overflow=1" )
#set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wextra -Warray-bounds" )
#set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -pedantic" )

add_executable(spg
                    src/allsp.cpp
                    src/curve.cpp
                    src/heap.cpp
					          src/inits.cpp
                    src/io.cpp
                    src/main.cpp
                    src/opt2base.cpp
                    src/opt2.cpp
                    src/opt2a.cpp
                    src/opt2b.cpp
                    src/opt2c.cpp
                    src/opt2d.cpp
                    src/opt2e.cpp
                    src/opt2f.cpp
                    src/opt2g.cpp
					          src/opt2h.cpp
                    src/star.cpp
                    src/statistic.cpp
                    src/test.cpp
                    )

target_include_directories(spg PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
  $<INSTALL_INTERFACE:includes>
  PRIVATE src)

target_link_libraries(spg util)
