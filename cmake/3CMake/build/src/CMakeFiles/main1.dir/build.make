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
CMAKE_SOURCE_DIR = /root/code/HopeGit/cmake/3CMake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/code/HopeGit/cmake/3CMake/build

# Include any dependencies generated for this target.
include src/CMakeFiles/main1.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/main1.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/main1.dir/flags.make

src/CMakeFiles/main1.dir/main1.c.o: src/CMakeFiles/main1.dir/flags.make
src/CMakeFiles/main1.dir/main1.c.o: ../src/main1.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/code/HopeGit/cmake/3CMake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/main1.dir/main1.c.o"
	cd /root/code/HopeGit/cmake/3CMake/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/main1.dir/main1.c.o   -c /root/code/HopeGit/cmake/3CMake/src/main1.c

src/CMakeFiles/main1.dir/main1.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main1.dir/main1.c.i"
	cd /root/code/HopeGit/cmake/3CMake/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/code/HopeGit/cmake/3CMake/src/main1.c > CMakeFiles/main1.dir/main1.c.i

src/CMakeFiles/main1.dir/main1.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main1.dir/main1.c.s"
	cd /root/code/HopeGit/cmake/3CMake/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/code/HopeGit/cmake/3CMake/src/main1.c -o CMakeFiles/main1.dir/main1.c.s

src/CMakeFiles/main1.dir/main1.c.o.requires:

.PHONY : src/CMakeFiles/main1.dir/main1.c.o.requires

src/CMakeFiles/main1.dir/main1.c.o.provides: src/CMakeFiles/main1.dir/main1.c.o.requires
	$(MAKE) -f src/CMakeFiles/main1.dir/build.make src/CMakeFiles/main1.dir/main1.c.o.provides.build
.PHONY : src/CMakeFiles/main1.dir/main1.c.o.provides

src/CMakeFiles/main1.dir/main1.c.o.provides.build: src/CMakeFiles/main1.dir/main1.c.o


src/CMakeFiles/main1.dir/testFunc1.c.o: src/CMakeFiles/main1.dir/flags.make
src/CMakeFiles/main1.dir/testFunc1.c.o: ../src/testFunc1.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/code/HopeGit/cmake/3CMake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/CMakeFiles/main1.dir/testFunc1.c.o"
	cd /root/code/HopeGit/cmake/3CMake/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/main1.dir/testFunc1.c.o   -c /root/code/HopeGit/cmake/3CMake/src/testFunc1.c

src/CMakeFiles/main1.dir/testFunc1.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main1.dir/testFunc1.c.i"
	cd /root/code/HopeGit/cmake/3CMake/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/code/HopeGit/cmake/3CMake/src/testFunc1.c > CMakeFiles/main1.dir/testFunc1.c.i

src/CMakeFiles/main1.dir/testFunc1.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main1.dir/testFunc1.c.s"
	cd /root/code/HopeGit/cmake/3CMake/build/src && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/code/HopeGit/cmake/3CMake/src/testFunc1.c -o CMakeFiles/main1.dir/testFunc1.c.s

src/CMakeFiles/main1.dir/testFunc1.c.o.requires:

.PHONY : src/CMakeFiles/main1.dir/testFunc1.c.o.requires

src/CMakeFiles/main1.dir/testFunc1.c.o.provides: src/CMakeFiles/main1.dir/testFunc1.c.o.requires
	$(MAKE) -f src/CMakeFiles/main1.dir/build.make src/CMakeFiles/main1.dir/testFunc1.c.o.provides.build
.PHONY : src/CMakeFiles/main1.dir/testFunc1.c.o.provides

src/CMakeFiles/main1.dir/testFunc1.c.o.provides.build: src/CMakeFiles/main1.dir/testFunc1.c.o


# Object files for target main1
main1_OBJECTS = \
"CMakeFiles/main1.dir/main1.c.o" \
"CMakeFiles/main1.dir/testFunc1.c.o"

# External object files for target main1
main1_EXTERNAL_OBJECTS =

../bin/main1: src/CMakeFiles/main1.dir/main1.c.o
../bin/main1: src/CMakeFiles/main1.dir/testFunc1.c.o
../bin/main1: src/CMakeFiles/main1.dir/build.make
../bin/main1: src/CMakeFiles/main1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/code/HopeGit/cmake/3CMake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ../../bin/main1"
	cd /root/code/HopeGit/cmake/3CMake/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/main1.dir/build: ../bin/main1

.PHONY : src/CMakeFiles/main1.dir/build

src/CMakeFiles/main1.dir/requires: src/CMakeFiles/main1.dir/main1.c.o.requires
src/CMakeFiles/main1.dir/requires: src/CMakeFiles/main1.dir/testFunc1.c.o.requires

.PHONY : src/CMakeFiles/main1.dir/requires

src/CMakeFiles/main1.dir/clean:
	cd /root/code/HopeGit/cmake/3CMake/build/src && $(CMAKE_COMMAND) -P CMakeFiles/main1.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/main1.dir/clean

src/CMakeFiles/main1.dir/depend:
	cd /root/code/HopeGit/cmake/3CMake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/code/HopeGit/cmake/3CMake /root/code/HopeGit/cmake/3CMake/src /root/code/HopeGit/cmake/3CMake/build /root/code/HopeGit/cmake/3CMake/build/src /root/code/HopeGit/cmake/3CMake/build/src/CMakeFiles/main1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/main1.dir/depend

