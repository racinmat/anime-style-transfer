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
CMAKE_SOURCE_DIR = /home/otaj/files/dip/master-thesis/code/dat-unpacker

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/otaj/files/dip/master-thesis/code/dat-unpacker-build

# Include any dependencies generated for this target.
include CMakeFiles/dat-unpacker.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dat-unpacker.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dat-unpacker.dir/flags.make

CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o: CMakeFiles/dat-unpacker.dir/flags.make
CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o: /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/dat_handler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/otaj/files/dip/master-thesis/code/dat-unpacker-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o -c /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/dat_handler.cpp

CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/dat_handler.cpp > CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.i

CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/dat_handler.cpp -o CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.s

CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o.requires:

.PHONY : CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o.requires

CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o.provides: CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o.requires
	$(MAKE) -f CMakeFiles/dat-unpacker.dir/build.make CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o.provides.build
.PHONY : CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o.provides

CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o.provides.build: CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o


CMakeFiles/dat-unpacker.dir/src/imu.cpp.o: CMakeFiles/dat-unpacker.dir/flags.make
CMakeFiles/dat-unpacker.dir/src/imu.cpp.o: /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/imu.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/otaj/files/dip/master-thesis/code/dat-unpacker-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/dat-unpacker.dir/src/imu.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dat-unpacker.dir/src/imu.cpp.o -c /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/imu.cpp

CMakeFiles/dat-unpacker.dir/src/imu.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dat-unpacker.dir/src/imu.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/imu.cpp > CMakeFiles/dat-unpacker.dir/src/imu.cpp.i

CMakeFiles/dat-unpacker.dir/src/imu.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dat-unpacker.dir/src/imu.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/imu.cpp -o CMakeFiles/dat-unpacker.dir/src/imu.cpp.s

CMakeFiles/dat-unpacker.dir/src/imu.cpp.o.requires:

.PHONY : CMakeFiles/dat-unpacker.dir/src/imu.cpp.o.requires

CMakeFiles/dat-unpacker.dir/src/imu.cpp.o.provides: CMakeFiles/dat-unpacker.dir/src/imu.cpp.o.requires
	$(MAKE) -f CMakeFiles/dat-unpacker.dir/build.make CMakeFiles/dat-unpacker.dir/src/imu.cpp.o.provides.build
.PHONY : CMakeFiles/dat-unpacker.dir/src/imu.cpp.o.provides

CMakeFiles/dat-unpacker.dir/src/imu.cpp.o.provides.build: CMakeFiles/dat-unpacker.dir/src/imu.cpp.o


CMakeFiles/dat-unpacker.dir/src/main.cpp.o: CMakeFiles/dat-unpacker.dir/flags.make
CMakeFiles/dat-unpacker.dir/src/main.cpp.o: /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/otaj/files/dip/master-thesis/code/dat-unpacker-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/dat-unpacker.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dat-unpacker.dir/src/main.cpp.o -c /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/main.cpp

CMakeFiles/dat-unpacker.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dat-unpacker.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/main.cpp > CMakeFiles/dat-unpacker.dir/src/main.cpp.i

CMakeFiles/dat-unpacker.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dat-unpacker.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/main.cpp -o CMakeFiles/dat-unpacker.dir/src/main.cpp.s

CMakeFiles/dat-unpacker.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/dat-unpacker.dir/src/main.cpp.o.requires

CMakeFiles/dat-unpacker.dir/src/main.cpp.o.provides: CMakeFiles/dat-unpacker.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/dat-unpacker.dir/build.make CMakeFiles/dat-unpacker.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/dat-unpacker.dir/src/main.cpp.o.provides

CMakeFiles/dat-unpacker.dir/src/main.cpp.o.provides.build: CMakeFiles/dat-unpacker.dir/src/main.cpp.o


CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o: CMakeFiles/dat-unpacker.dir/flags.make
CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o: /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/parse_input.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/otaj/files/dip/master-thesis/code/dat-unpacker-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o -c /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/parse_input.cpp

CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/parse_input.cpp > CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.i

CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/otaj/files/dip/master-thesis/code/dat-unpacker/src/parse_input.cpp -o CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.s

CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o.requires:

.PHONY : CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o.requires

CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o.provides: CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o.requires
	$(MAKE) -f CMakeFiles/dat-unpacker.dir/build.make CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o.provides.build
.PHONY : CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o.provides

CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o.provides.build: CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o


# Object files for target dat-unpacker
dat__unpacker_OBJECTS = \
"CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o" \
"CMakeFiles/dat-unpacker.dir/src/imu.cpp.o" \
"CMakeFiles/dat-unpacker.dir/src/main.cpp.o" \
"CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o"

# External object files for target dat-unpacker
dat__unpacker_EXTERNAL_OBJECTS =

dat-unpacker: CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o
dat-unpacker: CMakeFiles/dat-unpacker.dir/src/imu.cpp.o
dat-unpacker: CMakeFiles/dat-unpacker.dir/src/main.cpp.o
dat-unpacker: CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o
dat-unpacker: CMakeFiles/dat-unpacker.dir/build.make
dat-unpacker: /home/otaj/files/dip/master-thesis/code/dat-unpacker/../external/adtf-streaminglib/lib/libadtfstreaming_290.so
dat-unpacker: /usr/lib/libboost_program_options.so
dat-unpacker: /usr/lib/libQt5Core.so.5.10.1
dat-unpacker: /usr/lib/libboost_program_options.so
dat-unpacker: /usr/lib/libboost_system.so
dat-unpacker: /usr/lib/libboost_thread.so
dat-unpacker: /usr/lib/libboost_date_time.so
dat-unpacker: /usr/lib/libboost_chrono.so
dat-unpacker: /usr/lib/libboost_atomic.so
dat-unpacker: /usr/lib/libboost_system.so
dat-unpacker: /usr/lib/libboost_thread.so
dat-unpacker: /usr/lib/libboost_date_time.so
dat-unpacker: /usr/lib/libboost_chrono.so
dat-unpacker: /usr/lib/libboost_atomic.so
dat-unpacker: CMakeFiles/dat-unpacker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/otaj/files/dip/master-thesis/code/dat-unpacker-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable dat-unpacker"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dat-unpacker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dat-unpacker.dir/build: dat-unpacker

.PHONY : CMakeFiles/dat-unpacker.dir/build

CMakeFiles/dat-unpacker.dir/requires: CMakeFiles/dat-unpacker.dir/src/dat_handler.cpp.o.requires
CMakeFiles/dat-unpacker.dir/requires: CMakeFiles/dat-unpacker.dir/src/imu.cpp.o.requires
CMakeFiles/dat-unpacker.dir/requires: CMakeFiles/dat-unpacker.dir/src/main.cpp.o.requires
CMakeFiles/dat-unpacker.dir/requires: CMakeFiles/dat-unpacker.dir/src/parse_input.cpp.o.requires

.PHONY : CMakeFiles/dat-unpacker.dir/requires

CMakeFiles/dat-unpacker.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dat-unpacker.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dat-unpacker.dir/clean

CMakeFiles/dat-unpacker.dir/depend:
	cd /home/otaj/files/dip/master-thesis/code/dat-unpacker-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/otaj/files/dip/master-thesis/code/dat-unpacker /home/otaj/files/dip/master-thesis/code/dat-unpacker /home/otaj/files/dip/master-thesis/code/dat-unpacker-build /home/otaj/files/dip/master-thesis/code/dat-unpacker-build /home/otaj/files/dip/master-thesis/code/dat-unpacker-build/CMakeFiles/dat-unpacker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dat-unpacker.dir/depend

