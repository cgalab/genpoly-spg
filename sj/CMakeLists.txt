

add_executable(spg
                    src/main.cpp
                    src/basicFunctions.cpp
					          src/inits.cpp
                    src/io.cpp
                    src/opt2.cpp
                    src/opt2a.cpp
                    src/opt2b.cpp
                    src/pol.cpp
                    src/test.cpp
					src/curve.cpp
                    )

target_include_directories(spg PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
  $<INSTALL_INTERFACE:includes>
  PRIVATE src)


target_link_libraries(spg util)
