# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_SOURCE_DIR = /tmp/tmp.87eUbnXTyU

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.87eUbnXTyU/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/poker_app.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/poker_app.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/poker_app.dir/flags.make

CMakeFiles/poker_app.dir/src/main.cpp.o: CMakeFiles/poker_app.dir/flags.make
CMakeFiles/poker_app.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.87eUbnXTyU/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/poker_app.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/poker_app.dir/src/main.cpp.o -c /tmp/tmp.87eUbnXTyU/src/main.cpp

CMakeFiles/poker_app.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/poker_app.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.87eUbnXTyU/src/main.cpp > CMakeFiles/poker_app.dir/src/main.cpp.i

CMakeFiles/poker_app.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/poker_app.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.87eUbnXTyU/src/main.cpp -o CMakeFiles/poker_app.dir/src/main.cpp.s

CMakeFiles/poker_app.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/poker_app.dir/src/main.cpp.o.requires

CMakeFiles/poker_app.dir/src/main.cpp.o.provides: CMakeFiles/poker_app.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/poker_app.dir/build.make CMakeFiles/poker_app.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/poker_app.dir/src/main.cpp.o.provides

CMakeFiles/poker_app.dir/src/main.cpp.o.provides.build: CMakeFiles/poker_app.dir/src/main.cpp.o


CMakeFiles/poker_app.dir/src/server.cpp.o: CMakeFiles/poker_app.dir/flags.make
CMakeFiles/poker_app.dir/src/server.cpp.o: ../src/server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.87eUbnXTyU/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/poker_app.dir/src/server.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/poker_app.dir/src/server.cpp.o -c /tmp/tmp.87eUbnXTyU/src/server.cpp

CMakeFiles/poker_app.dir/src/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/poker_app.dir/src/server.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.87eUbnXTyU/src/server.cpp > CMakeFiles/poker_app.dir/src/server.cpp.i

CMakeFiles/poker_app.dir/src/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/poker_app.dir/src/server.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.87eUbnXTyU/src/server.cpp -o CMakeFiles/poker_app.dir/src/server.cpp.s

CMakeFiles/poker_app.dir/src/server.cpp.o.requires:

.PHONY : CMakeFiles/poker_app.dir/src/server.cpp.o.requires

CMakeFiles/poker_app.dir/src/server.cpp.o.provides: CMakeFiles/poker_app.dir/src/server.cpp.o.requires
	$(MAKE) -f CMakeFiles/poker_app.dir/build.make CMakeFiles/poker_app.dir/src/server.cpp.o.provides.build
.PHONY : CMakeFiles/poker_app.dir/src/server.cpp.o.provides

CMakeFiles/poker_app.dir/src/server.cpp.o.provides.build: CMakeFiles/poker_app.dir/src/server.cpp.o


CMakeFiles/poker_app.dir/src/client.cpp.o: CMakeFiles/poker_app.dir/flags.make
CMakeFiles/poker_app.dir/src/client.cpp.o: ../src/client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.87eUbnXTyU/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/poker_app.dir/src/client.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/poker_app.dir/src/client.cpp.o -c /tmp/tmp.87eUbnXTyU/src/client.cpp

CMakeFiles/poker_app.dir/src/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/poker_app.dir/src/client.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.87eUbnXTyU/src/client.cpp > CMakeFiles/poker_app.dir/src/client.cpp.i

CMakeFiles/poker_app.dir/src/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/poker_app.dir/src/client.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.87eUbnXTyU/src/client.cpp -o CMakeFiles/poker_app.dir/src/client.cpp.s

CMakeFiles/poker_app.dir/src/client.cpp.o.requires:

.PHONY : CMakeFiles/poker_app.dir/src/client.cpp.o.requires

CMakeFiles/poker_app.dir/src/client.cpp.o.provides: CMakeFiles/poker_app.dir/src/client.cpp.o.requires
	$(MAKE) -f CMakeFiles/poker_app.dir/build.make CMakeFiles/poker_app.dir/src/client.cpp.o.provides.build
.PHONY : CMakeFiles/poker_app.dir/src/client.cpp.o.provides

CMakeFiles/poker_app.dir/src/client.cpp.o.provides.build: CMakeFiles/poker_app.dir/src/client.cpp.o


# Object files for target poker_app
poker_app_OBJECTS = \
"CMakeFiles/poker_app.dir/src/main.cpp.o" \
"CMakeFiles/poker_app.dir/src/server.cpp.o" \
"CMakeFiles/poker_app.dir/src/client.cpp.o"

# External object files for target poker_app
poker_app_EXTERNAL_OBJECTS =

poker_app: CMakeFiles/poker_app.dir/src/main.cpp.o
poker_app: CMakeFiles/poker_app.dir/src/server.cpp.o
poker_app: CMakeFiles/poker_app.dir/src/client.cpp.o
poker_app: CMakeFiles/poker_app.dir/build.make
poker_app: CMakeFiles/poker_app.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.87eUbnXTyU/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable poker_app"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/poker_app.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/poker_app.dir/build: poker_app

.PHONY : CMakeFiles/poker_app.dir/build

CMakeFiles/poker_app.dir/requires: CMakeFiles/poker_app.dir/src/main.cpp.o.requires
CMakeFiles/poker_app.dir/requires: CMakeFiles/poker_app.dir/src/server.cpp.o.requires
CMakeFiles/poker_app.dir/requires: CMakeFiles/poker_app.dir/src/client.cpp.o.requires

.PHONY : CMakeFiles/poker_app.dir/requires

CMakeFiles/poker_app.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/poker_app.dir/cmake_clean.cmake
.PHONY : CMakeFiles/poker_app.dir/clean

CMakeFiles/poker_app.dir/depend:
	cd /tmp/tmp.87eUbnXTyU/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.87eUbnXTyU /tmp/tmp.87eUbnXTyU /tmp/tmp.87eUbnXTyU/cmake-build-debug /tmp/tmp.87eUbnXTyU/cmake-build-debug /tmp/tmp.87eUbnXTyU/cmake-build-debug/CMakeFiles/poker_app.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/poker_app.dir/depend

