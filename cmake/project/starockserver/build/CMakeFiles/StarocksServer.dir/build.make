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
CMAKE_SOURCE_DIR = /home/keyvalue/code/HopeGit/cmake/project/starockserver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/keyvalue/code/HopeGit/cmake/project/starockserver/build

# Include any dependencies generated for this target.
include CMakeFiles/StarocksServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/StarocksServer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/StarocksServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/StarocksServer.dir/flags.make

CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.o: CMakeFiles/StarocksServer.dir/flags.make
CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.o: ../src/starocks_api_face_snaps.cc
CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.o: CMakeFiles/StarocksServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/keyvalue/code/HopeGit/cmake/project/starockserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.o -MF CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.o.d -o CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.o -c /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/starocks_api_face_snaps.cc

CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/starocks_api_face_snaps.cc > CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.i

CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/starocks_api_face_snaps.cc -o CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.s

CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.o: CMakeFiles/StarocksServer.dir/flags.make
CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.o: ../src/starocks_api_person.cc
CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.o: CMakeFiles/StarocksServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/keyvalue/code/HopeGit/cmake/project/starockserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.o -MF CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.o.d -o CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.o -c /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/starocks_api_person.cc

CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/starocks_api_person.cc > CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.i

CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/starocks_api_person.cc -o CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.s

CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.o: CMakeFiles/StarocksServer.dir/flags.make
CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.o: ../src/starocks_api_persons_info.cc
CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.o: CMakeFiles/StarocksServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/keyvalue/code/HopeGit/cmake/project/starockserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.o -MF CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.o.d -o CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.o -c /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/starocks_api_persons_info.cc

CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/starocks_api_persons_info.cc > CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.i

CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/starocks_api_persons_info.cc -o CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.s

CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.o: CMakeFiles/StarocksServer.dir/flags.make
CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.o: ../src/starocks_api_temp_data.cc
CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.o: CMakeFiles/StarocksServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/keyvalue/code/HopeGit/cmake/project/starockserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.o -MF CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.o.d -o CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.o -c /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/starocks_api_temp_data.cc

CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/starocks_api_temp_data.cc > CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.i

CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/starocks_api_temp_data.cc -o CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.s

CMakeFiles/StarocksServer.dir/src/util.cc.o: CMakeFiles/StarocksServer.dir/flags.make
CMakeFiles/StarocksServer.dir/src/util.cc.o: ../src/util.cc
CMakeFiles/StarocksServer.dir/src/util.cc.o: CMakeFiles/StarocksServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/keyvalue/code/HopeGit/cmake/project/starockserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/StarocksServer.dir/src/util.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/StarocksServer.dir/src/util.cc.o -MF CMakeFiles/StarocksServer.dir/src/util.cc.o.d -o CMakeFiles/StarocksServer.dir/src/util.cc.o -c /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/util.cc

CMakeFiles/StarocksServer.dir/src/util.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/StarocksServer.dir/src/util.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/util.cc > CMakeFiles/StarocksServer.dir/src/util.cc.i

CMakeFiles/StarocksServer.dir/src/util.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/StarocksServer.dir/src/util.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/keyvalue/code/HopeGit/cmake/project/starockserver/src/util.cc -o CMakeFiles/StarocksServer.dir/src/util.cc.s

CMakeFiles/StarocksServer.dir/alStarocksServer.cc.o: CMakeFiles/StarocksServer.dir/flags.make
CMakeFiles/StarocksServer.dir/alStarocksServer.cc.o: ../alStarocksServer.cc
CMakeFiles/StarocksServer.dir/alStarocksServer.cc.o: CMakeFiles/StarocksServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/keyvalue/code/HopeGit/cmake/project/starockserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/StarocksServer.dir/alStarocksServer.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/StarocksServer.dir/alStarocksServer.cc.o -MF CMakeFiles/StarocksServer.dir/alStarocksServer.cc.o.d -o CMakeFiles/StarocksServer.dir/alStarocksServer.cc.o -c /home/keyvalue/code/HopeGit/cmake/project/starockserver/alStarocksServer.cc

CMakeFiles/StarocksServer.dir/alStarocksServer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/StarocksServer.dir/alStarocksServer.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/keyvalue/code/HopeGit/cmake/project/starockserver/alStarocksServer.cc > CMakeFiles/StarocksServer.dir/alStarocksServer.cc.i

CMakeFiles/StarocksServer.dir/alStarocksServer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/StarocksServer.dir/alStarocksServer.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/keyvalue/code/HopeGit/cmake/project/starockserver/alStarocksServer.cc -o CMakeFiles/StarocksServer.dir/alStarocksServer.cc.s

# Object files for target StarocksServer
StarocksServer_OBJECTS = \
"CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.o" \
"CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.o" \
"CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.o" \
"CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.o" \
"CMakeFiles/StarocksServer.dir/src/util.cc.o" \
"CMakeFiles/StarocksServer.dir/alStarocksServer.cc.o"

# External object files for target StarocksServer
StarocksServer_EXTERNAL_OBJECTS =

bin/alStarocksServer: CMakeFiles/StarocksServer.dir/src/starocks_api_face_snaps.cc.o
bin/alStarocksServer: CMakeFiles/StarocksServer.dir/src/starocks_api_person.cc.o
bin/alStarocksServer: CMakeFiles/StarocksServer.dir/src/starocks_api_persons_info.cc.o
bin/alStarocksServer: CMakeFiles/StarocksServer.dir/src/starocks_api_temp_data.cc.o
bin/alStarocksServer: CMakeFiles/StarocksServer.dir/src/util.cc.o
bin/alStarocksServer: CMakeFiles/StarocksServer.dir/alStarocksServer.cc.o
bin/alStarocksServer: CMakeFiles/StarocksServer.dir/build.make
bin/alStarocksServer: ../lib/libelog.a
bin/alStarocksServer: ../lib/libcjson.a
bin/alStarocksServer: CMakeFiles/StarocksServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/keyvalue/code/HopeGit/cmake/project/starockserver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable bin/alStarocksServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/StarocksServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/StarocksServer.dir/build: bin/alStarocksServer
.PHONY : CMakeFiles/StarocksServer.dir/build

CMakeFiles/StarocksServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/StarocksServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/StarocksServer.dir/clean

CMakeFiles/StarocksServer.dir/depend:
	cd /home/keyvalue/code/HopeGit/cmake/project/starockserver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/keyvalue/code/HopeGit/cmake/project/starockserver /home/keyvalue/code/HopeGit/cmake/project/starockserver /home/keyvalue/code/HopeGit/cmake/project/starockserver/build /home/keyvalue/code/HopeGit/cmake/project/starockserver/build /home/keyvalue/code/HopeGit/cmake/project/starockserver/build/CMakeFiles/StarocksServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/StarocksServer.dir/depend

