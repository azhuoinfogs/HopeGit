# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/keyvalue/code/tools/cmake-3.21.1-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/keyvalue/code/tools/cmake-3.21.1-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/keyvalue/code/HopeGit/cmake/CmakeTest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/keyvalue/code/HopeGit/cmake/CmakeTest

# Include any dependencies generated for this target.
include test/CMakeFiles/test_demo.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/test_demo.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_demo.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_demo.dir/flags.make

test/CMakeFiles/test_demo.dir/test.cpp.o: test/CMakeFiles/test_demo.dir/flags.make
test/CMakeFiles/test_demo.dir/test.cpp.o: test/test.cpp
test/CMakeFiles/test_demo.dir/test.cpp.o: test/CMakeFiles/test_demo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/keyvalue/code/HopeGit/cmake/CmakeTest/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_demo.dir/test.cpp.o"
	cd /home/keyvalue/code/HopeGit/cmake/CmakeTest/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/test_demo.dir/test.cpp.o -MF CMakeFiles/test_demo.dir/test.cpp.o.d -o CMakeFiles/test_demo.dir/test.cpp.o -c /home/keyvalue/code/HopeGit/cmake/CmakeTest/test/test.cpp

test/CMakeFiles/test_demo.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_demo.dir/test.cpp.i"
	cd /home/keyvalue/code/HopeGit/cmake/CmakeTest/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/keyvalue/code/HopeGit/cmake/CmakeTest/test/test.cpp > CMakeFiles/test_demo.dir/test.cpp.i

test/CMakeFiles/test_demo.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_demo.dir/test.cpp.s"
	cd /home/keyvalue/code/HopeGit/cmake/CmakeTest/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/keyvalue/code/HopeGit/cmake/CmakeTest/test/test.cpp -o CMakeFiles/test_demo.dir/test.cpp.s

# Object files for target test_demo
test_demo_OBJECTS = \
"CMakeFiles/test_demo.dir/test.cpp.o"

# External object files for target test_demo
test_demo_EXTERNAL_OBJECTS =

test/test_demo: test/CMakeFiles/test_demo.dir/test.cpp.o
test/test_demo: test/CMakeFiles/test_demo.dir/build.make
test/test_demo: /usr/local/lib/libgtest.a
test/test_demo: /usr/local/lib/libgtest_main.a
test/test_demo: src/libadd.so
test/test_demo: test/CMakeFiles/test_demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/keyvalue/code/HopeGit/cmake/CmakeTest/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_demo"
	cd /home/keyvalue/code/HopeGit/cmake/CmakeTest/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_demo.dir/build: test/test_demo
.PHONY : test/CMakeFiles/test_demo.dir/build

test/CMakeFiles/test_demo.dir/clean:
	cd /home/keyvalue/code/HopeGit/cmake/CmakeTest/test && $(CMAKE_COMMAND) -P CMakeFiles/test_demo.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_demo.dir/clean

test/CMakeFiles/test_demo.dir/depend:
	cd /home/keyvalue/code/HopeGit/cmake/CmakeTest && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/keyvalue/code/HopeGit/cmake/CmakeTest /home/keyvalue/code/HopeGit/cmake/CmakeTest/test /home/keyvalue/code/HopeGit/cmake/CmakeTest /home/keyvalue/code/HopeGit/cmake/CmakeTest/test /home/keyvalue/code/HopeGit/cmake/CmakeTest/test/CMakeFiles/test_demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test_demo.dir/depend
