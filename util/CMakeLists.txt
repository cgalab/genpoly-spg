# Define library. Only source files here!
project(util VERSION 0.1 LANGUAGES CXX)

add_library(util
    src/basicFunctions.cpp
    src/edge.cpp
    src/point.cpp
    src/pol.cpp
    src/predicates.cpp
    src/simple_pol_check.cpp
    )

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(util PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
    $<INSTALL_INTERFACE:includes>
    PRIVATE src)

# manually inserted..
#add_definitions(-DNDEBUG)

# 'make install' to the correct locations (provided by GNUInstallDirs).
#install(TARGETS util EXPORT MyLibraryConfig
#    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows
#install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# This makes the project importable from the install directory
# Put config file in per-project dir (name MUST match), can also
# just go into 'cmake'.
#install(EXPORT MyLibraryConfig DESTINATION share/MyLibrary/cmake)

# This makes the project importable from the build directory
#export(TARGETS util FILE MyLibraryConfig.cmake)
