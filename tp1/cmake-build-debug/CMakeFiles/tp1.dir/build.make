# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = "/Users/diegosilvapires/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/173.4548.31/CLion.app/Contents/bin/cmake/bin/cmake"

# The command to remove a file.
RM = "/Users/diegosilvapires/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/173.4548.31/CLion.app/Contents/bin/cmake/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/diegosilvapires/Bitbucket/ppd-h18/tp1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/diegosilvapires/Bitbucket/ppd-h18/tp1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/tp1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tp1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tp1.dir/flags.make

CMakeFiles/tp1.dir/premiers.cpp.o: CMakeFiles/tp1.dir/flags.make
CMakeFiles/tp1.dir/premiers.cpp.o: ../premiers.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/diegosilvapires/Bitbucket/ppd-h18/tp1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tp1.dir/premiers.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tp1.dir/premiers.cpp.o -c /Users/diegosilvapires/Bitbucket/ppd-h18/tp1/premiers.cpp

CMakeFiles/tp1.dir/premiers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tp1.dir/premiers.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/diegosilvapires/Bitbucket/ppd-h18/tp1/premiers.cpp > CMakeFiles/tp1.dir/premiers.cpp.i

CMakeFiles/tp1.dir/premiers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tp1.dir/premiers.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/diegosilvapires/Bitbucket/ppd-h18/tp1/premiers.cpp -o CMakeFiles/tp1.dir/premiers.cpp.s

CMakeFiles/tp1.dir/premiers.cpp.o.requires:

.PHONY : CMakeFiles/tp1.dir/premiers.cpp.o.requires

CMakeFiles/tp1.dir/premiers.cpp.o.provides: CMakeFiles/tp1.dir/premiers.cpp.o.requires
	$(MAKE) -f CMakeFiles/tp1.dir/build.make CMakeFiles/tp1.dir/premiers.cpp.o.provides.build
.PHONY : CMakeFiles/tp1.dir/premiers.cpp.o.provides

CMakeFiles/tp1.dir/premiers.cpp.o.provides.build: CMakeFiles/tp1.dir/premiers.cpp.o


# Object files for target tp1
tp1_OBJECTS = \
"CMakeFiles/tp1.dir/premiers.cpp.o"

# External object files for target tp1
tp1_EXTERNAL_OBJECTS =

tp1: CMakeFiles/tp1.dir/premiers.cpp.o
tp1: CMakeFiles/tp1.dir/build.make
tp1: CMakeFiles/tp1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/diegosilvapires/Bitbucket/ppd-h18/tp1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tp1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tp1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tp1.dir/build: tp1

.PHONY : CMakeFiles/tp1.dir/build

CMakeFiles/tp1.dir/requires: CMakeFiles/tp1.dir/premiers.cpp.o.requires

.PHONY : CMakeFiles/tp1.dir/requires

CMakeFiles/tp1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tp1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tp1.dir/clean

CMakeFiles/tp1.dir/depend:
	cd /Users/diegosilvapires/Bitbucket/ppd-h18/tp1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/diegosilvapires/Bitbucket/ppd-h18/tp1 /Users/diegosilvapires/Bitbucket/ppd-h18/tp1 /Users/diegosilvapires/Bitbucket/ppd-h18/tp1/cmake-build-debug /Users/diegosilvapires/Bitbucket/ppd-h18/tp1/cmake-build-debug /Users/diegosilvapires/Bitbucket/ppd-h18/tp1/cmake-build-debug/CMakeFiles/tp1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tp1.dir/depend
