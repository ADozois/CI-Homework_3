# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = /opt/clion-2016.2.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /opt/clion-2016.2.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gemini/TUM/CI/CI-Homework_3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gemini/TUM/CI/CI-Homework_3/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/CI_Homework_3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CI_Homework_3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CI_Homework_3.dir/flags.make

CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o: CMakeFiles/CI_Homework_3.dir/flags.make
CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o: ../Problem\ 2/Problem_2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gemini/TUM/CI/CI-Homework_3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o   -c "/home/gemini/TUM/CI/CI-Homework_3/Problem 2/Problem_2.c"

CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/gemini/TUM/CI/CI-Homework_3/Problem 2/Problem_2.c" > CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.i

CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/gemini/TUM/CI/CI-Homework_3/Problem 2/Problem_2.c" -o CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.s

CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o.requires:

.PHONY : CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o.requires

CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o.provides: CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o.requires
	$(MAKE) -f CMakeFiles/CI_Homework_3.dir/build.make CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o.provides.build
.PHONY : CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o.provides

CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o.provides.build: CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o


# Object files for target CI_Homework_3
CI_Homework_3_OBJECTS = \
"CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o"

# External object files for target CI_Homework_3
CI_Homework_3_EXTERNAL_OBJECTS =

CI_Homework_3: CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o
CI_Homework_3: CMakeFiles/CI_Homework_3.dir/build.make
CI_Homework_3: CMakeFiles/CI_Homework_3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gemini/TUM/CI/CI-Homework_3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable CI_Homework_3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CI_Homework_3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CI_Homework_3.dir/build: CI_Homework_3

.PHONY : CMakeFiles/CI_Homework_3.dir/build

CMakeFiles/CI_Homework_3.dir/requires: CMakeFiles/CI_Homework_3.dir/Problem_2/Problem_2.c.o.requires

.PHONY : CMakeFiles/CI_Homework_3.dir/requires

CMakeFiles/CI_Homework_3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CI_Homework_3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CI_Homework_3.dir/clean

CMakeFiles/CI_Homework_3.dir/depend:
	cd /home/gemini/TUM/CI/CI-Homework_3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gemini/TUM/CI/CI-Homework_3 /home/gemini/TUM/CI/CI-Homework_3 /home/gemini/TUM/CI/CI-Homework_3/cmake-build-debug /home/gemini/TUM/CI/CI-Homework_3/cmake-build-debug /home/gemini/TUM/CI/CI-Homework_3/cmake-build-debug/CMakeFiles/CI_Homework_3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CI_Homework_3.dir/depend

