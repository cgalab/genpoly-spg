# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pma/sjas/projects/spg

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pma/sjas/projects/spg/sj/build

# Include any dependencies generated for this target.
include sj/CMakeFiles/spg.dir/depend.make

# Include the progress variables for this target.
include sj/CMakeFiles/spg.dir/progress.make

# Include the compile flags for this target's objects.
include sj/CMakeFiles/spg.dir/flags.make

sj/CMakeFiles/spg.dir/src/main.cpp.o: sj/CMakeFiles/spg.dir/flags.make
sj/CMakeFiles/spg.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pma/sjas/projects/spg/sj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object sj/CMakeFiles/spg.dir/src/main.cpp.o"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spg.dir/src/main.cpp.o -c /home/pma/sjas/projects/spg/sj/src/main.cpp

sj/CMakeFiles/spg.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spg.dir/src/main.cpp.i"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pma/sjas/projects/spg/sj/src/main.cpp > CMakeFiles/spg.dir/src/main.cpp.i

sj/CMakeFiles/spg.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spg.dir/src/main.cpp.s"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pma/sjas/projects/spg/sj/src/main.cpp -o CMakeFiles/spg.dir/src/main.cpp.s

sj/CMakeFiles/spg.dir/src/main.cpp.o.requires:

.PHONY : sj/CMakeFiles/spg.dir/src/main.cpp.o.requires

sj/CMakeFiles/spg.dir/src/main.cpp.o.provides: sj/CMakeFiles/spg.dir/src/main.cpp.o.requires
	$(MAKE) -f sj/CMakeFiles/spg.dir/build.make sj/CMakeFiles/spg.dir/src/main.cpp.o.provides.build
.PHONY : sj/CMakeFiles/spg.dir/src/main.cpp.o.provides

sj/CMakeFiles/spg.dir/src/main.cpp.o.provides.build: sj/CMakeFiles/spg.dir/src/main.cpp.o


sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.o: sj/CMakeFiles/spg.dir/flags.make
sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.o: ../src/basicFunctions.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pma/sjas/projects/spg/sj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.o"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spg.dir/src/basicFunctions.cpp.o -c /home/pma/sjas/projects/spg/sj/src/basicFunctions.cpp

sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spg.dir/src/basicFunctions.cpp.i"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pma/sjas/projects/spg/sj/src/basicFunctions.cpp > CMakeFiles/spg.dir/src/basicFunctions.cpp.i

sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spg.dir/src/basicFunctions.cpp.s"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pma/sjas/projects/spg/sj/src/basicFunctions.cpp -o CMakeFiles/spg.dir/src/basicFunctions.cpp.s

sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.o.requires:

.PHONY : sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.o.requires

sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.o.provides: sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.o.requires
	$(MAKE) -f sj/CMakeFiles/spg.dir/build.make sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.o.provides.build
.PHONY : sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.o.provides

sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.o.provides.build: sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.o


sj/CMakeFiles/spg.dir/src/inits.cpp.o: sj/CMakeFiles/spg.dir/flags.make
sj/CMakeFiles/spg.dir/src/inits.cpp.o: ../src/inits.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pma/sjas/projects/spg/sj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object sj/CMakeFiles/spg.dir/src/inits.cpp.o"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spg.dir/src/inits.cpp.o -c /home/pma/sjas/projects/spg/sj/src/inits.cpp

sj/CMakeFiles/spg.dir/src/inits.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spg.dir/src/inits.cpp.i"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pma/sjas/projects/spg/sj/src/inits.cpp > CMakeFiles/spg.dir/src/inits.cpp.i

sj/CMakeFiles/spg.dir/src/inits.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spg.dir/src/inits.cpp.s"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pma/sjas/projects/spg/sj/src/inits.cpp -o CMakeFiles/spg.dir/src/inits.cpp.s

sj/CMakeFiles/spg.dir/src/inits.cpp.o.requires:

.PHONY : sj/CMakeFiles/spg.dir/src/inits.cpp.o.requires

sj/CMakeFiles/spg.dir/src/inits.cpp.o.provides: sj/CMakeFiles/spg.dir/src/inits.cpp.o.requires
	$(MAKE) -f sj/CMakeFiles/spg.dir/build.make sj/CMakeFiles/spg.dir/src/inits.cpp.o.provides.build
.PHONY : sj/CMakeFiles/spg.dir/src/inits.cpp.o.provides

sj/CMakeFiles/spg.dir/src/inits.cpp.o.provides.build: sj/CMakeFiles/spg.dir/src/inits.cpp.o


sj/CMakeFiles/spg.dir/src/io.cpp.o: sj/CMakeFiles/spg.dir/flags.make
sj/CMakeFiles/spg.dir/src/io.cpp.o: ../src/io.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pma/sjas/projects/spg/sj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object sj/CMakeFiles/spg.dir/src/io.cpp.o"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spg.dir/src/io.cpp.o -c /home/pma/sjas/projects/spg/sj/src/io.cpp

sj/CMakeFiles/spg.dir/src/io.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spg.dir/src/io.cpp.i"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pma/sjas/projects/spg/sj/src/io.cpp > CMakeFiles/spg.dir/src/io.cpp.i

sj/CMakeFiles/spg.dir/src/io.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spg.dir/src/io.cpp.s"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pma/sjas/projects/spg/sj/src/io.cpp -o CMakeFiles/spg.dir/src/io.cpp.s

sj/CMakeFiles/spg.dir/src/io.cpp.o.requires:

.PHONY : sj/CMakeFiles/spg.dir/src/io.cpp.o.requires

sj/CMakeFiles/spg.dir/src/io.cpp.o.provides: sj/CMakeFiles/spg.dir/src/io.cpp.o.requires
	$(MAKE) -f sj/CMakeFiles/spg.dir/build.make sj/CMakeFiles/spg.dir/src/io.cpp.o.provides.build
.PHONY : sj/CMakeFiles/spg.dir/src/io.cpp.o.provides

sj/CMakeFiles/spg.dir/src/io.cpp.o.provides.build: sj/CMakeFiles/spg.dir/src/io.cpp.o


sj/CMakeFiles/spg.dir/src/opt2.cpp.o: sj/CMakeFiles/spg.dir/flags.make
sj/CMakeFiles/spg.dir/src/opt2.cpp.o: ../src/opt2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pma/sjas/projects/spg/sj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object sj/CMakeFiles/spg.dir/src/opt2.cpp.o"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spg.dir/src/opt2.cpp.o -c /home/pma/sjas/projects/spg/sj/src/opt2.cpp

sj/CMakeFiles/spg.dir/src/opt2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spg.dir/src/opt2.cpp.i"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pma/sjas/projects/spg/sj/src/opt2.cpp > CMakeFiles/spg.dir/src/opt2.cpp.i

sj/CMakeFiles/spg.dir/src/opt2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spg.dir/src/opt2.cpp.s"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pma/sjas/projects/spg/sj/src/opt2.cpp -o CMakeFiles/spg.dir/src/opt2.cpp.s

sj/CMakeFiles/spg.dir/src/opt2.cpp.o.requires:

.PHONY : sj/CMakeFiles/spg.dir/src/opt2.cpp.o.requires

sj/CMakeFiles/spg.dir/src/opt2.cpp.o.provides: sj/CMakeFiles/spg.dir/src/opt2.cpp.o.requires
	$(MAKE) -f sj/CMakeFiles/spg.dir/build.make sj/CMakeFiles/spg.dir/src/opt2.cpp.o.provides.build
.PHONY : sj/CMakeFiles/spg.dir/src/opt2.cpp.o.provides

sj/CMakeFiles/spg.dir/src/opt2.cpp.o.provides.build: sj/CMakeFiles/spg.dir/src/opt2.cpp.o


sj/CMakeFiles/spg.dir/src/pol.cpp.o: sj/CMakeFiles/spg.dir/flags.make
sj/CMakeFiles/spg.dir/src/pol.cpp.o: ../src/pol.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pma/sjas/projects/spg/sj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object sj/CMakeFiles/spg.dir/src/pol.cpp.o"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spg.dir/src/pol.cpp.o -c /home/pma/sjas/projects/spg/sj/src/pol.cpp

sj/CMakeFiles/spg.dir/src/pol.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spg.dir/src/pol.cpp.i"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pma/sjas/projects/spg/sj/src/pol.cpp > CMakeFiles/spg.dir/src/pol.cpp.i

sj/CMakeFiles/spg.dir/src/pol.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spg.dir/src/pol.cpp.s"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pma/sjas/projects/spg/sj/src/pol.cpp -o CMakeFiles/spg.dir/src/pol.cpp.s

sj/CMakeFiles/spg.dir/src/pol.cpp.o.requires:

.PHONY : sj/CMakeFiles/spg.dir/src/pol.cpp.o.requires

sj/CMakeFiles/spg.dir/src/pol.cpp.o.provides: sj/CMakeFiles/spg.dir/src/pol.cpp.o.requires
	$(MAKE) -f sj/CMakeFiles/spg.dir/build.make sj/CMakeFiles/spg.dir/src/pol.cpp.o.provides.build
.PHONY : sj/CMakeFiles/spg.dir/src/pol.cpp.o.provides

sj/CMakeFiles/spg.dir/src/pol.cpp.o.provides.build: sj/CMakeFiles/spg.dir/src/pol.cpp.o


sj/CMakeFiles/spg.dir/src/test.cpp.o: sj/CMakeFiles/spg.dir/flags.make
sj/CMakeFiles/spg.dir/src/test.cpp.o: ../src/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pma/sjas/projects/spg/sj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object sj/CMakeFiles/spg.dir/src/test.cpp.o"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spg.dir/src/test.cpp.o -c /home/pma/sjas/projects/spg/sj/src/test.cpp

sj/CMakeFiles/spg.dir/src/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spg.dir/src/test.cpp.i"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pma/sjas/projects/spg/sj/src/test.cpp > CMakeFiles/spg.dir/src/test.cpp.i

sj/CMakeFiles/spg.dir/src/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spg.dir/src/test.cpp.s"
	cd /home/pma/sjas/projects/spg/sj/build/sj && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pma/sjas/projects/spg/sj/src/test.cpp -o CMakeFiles/spg.dir/src/test.cpp.s

sj/CMakeFiles/spg.dir/src/test.cpp.o.requires:

.PHONY : sj/CMakeFiles/spg.dir/src/test.cpp.o.requires

sj/CMakeFiles/spg.dir/src/test.cpp.o.provides: sj/CMakeFiles/spg.dir/src/test.cpp.o.requires
	$(MAKE) -f sj/CMakeFiles/spg.dir/build.make sj/CMakeFiles/spg.dir/src/test.cpp.o.provides.build
.PHONY : sj/CMakeFiles/spg.dir/src/test.cpp.o.provides

sj/CMakeFiles/spg.dir/src/test.cpp.o.provides.build: sj/CMakeFiles/spg.dir/src/test.cpp.o


# Object files for target spg
spg_OBJECTS = \
"CMakeFiles/spg.dir/src/main.cpp.o" \
"CMakeFiles/spg.dir/src/basicFunctions.cpp.o" \
"CMakeFiles/spg.dir/src/inits.cpp.o" \
"CMakeFiles/spg.dir/src/io.cpp.o" \
"CMakeFiles/spg.dir/src/opt2.cpp.o" \
"CMakeFiles/spg.dir/src/pol.cpp.o" \
"CMakeFiles/spg.dir/src/test.cpp.o"

# External object files for target spg
spg_EXTERNAL_OBJECTS =

spg: sj/CMakeFiles/spg.dir/src/main.cpp.o
spg: sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.o
spg: sj/CMakeFiles/spg.dir/src/inits.cpp.o
spg: sj/CMakeFiles/spg.dir/src/io.cpp.o
spg: sj/CMakeFiles/spg.dir/src/opt2.cpp.o
spg: sj/CMakeFiles/spg.dir/src/pol.cpp.o
spg: sj/CMakeFiles/spg.dir/src/test.cpp.o
spg: sj/CMakeFiles/spg.dir/build.make
spg: util/libutil.a
spg: sj/CMakeFiles/spg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pma/sjas/projects/spg/sj/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable ../spg"
	cd /home/pma/sjas/projects/spg/sj/build/sj && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/spg.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sj/CMakeFiles/spg.dir/build: spg

.PHONY : sj/CMakeFiles/spg.dir/build

sj/CMakeFiles/spg.dir/requires: sj/CMakeFiles/spg.dir/src/main.cpp.o.requires
sj/CMakeFiles/spg.dir/requires: sj/CMakeFiles/spg.dir/src/basicFunctions.cpp.o.requires
sj/CMakeFiles/spg.dir/requires: sj/CMakeFiles/spg.dir/src/inits.cpp.o.requires
sj/CMakeFiles/spg.dir/requires: sj/CMakeFiles/spg.dir/src/io.cpp.o.requires
sj/CMakeFiles/spg.dir/requires: sj/CMakeFiles/spg.dir/src/opt2.cpp.o.requires
sj/CMakeFiles/spg.dir/requires: sj/CMakeFiles/spg.dir/src/pol.cpp.o.requires
sj/CMakeFiles/spg.dir/requires: sj/CMakeFiles/spg.dir/src/test.cpp.o.requires

.PHONY : sj/CMakeFiles/spg.dir/requires

sj/CMakeFiles/spg.dir/clean:
	cd /home/pma/sjas/projects/spg/sj/build/sj && $(CMAKE_COMMAND) -P CMakeFiles/spg.dir/cmake_clean.cmake
.PHONY : sj/CMakeFiles/spg.dir/clean

sj/CMakeFiles/spg.dir/depend:
	cd /home/pma/sjas/projects/spg/sj/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pma/sjas/projects/spg /home/pma/sjas/projects/spg/sj /home/pma/sjas/projects/spg/sj/build /home/pma/sjas/projects/spg/sj/build/sj /home/pma/sjas/projects/spg/sj/build/sj/CMakeFiles/spg.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sj/CMakeFiles/spg.dir/depend

