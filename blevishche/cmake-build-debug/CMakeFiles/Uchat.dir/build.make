# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /private/var/folders/zz/zyxvpxvq6csfxvn_n0003ymh000zn4/T/AppTranslocation/64A5DD63-BC89-48FA-B593-234A9A4F50BE/d/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /private/var/folders/zz/zyxvpxvq6csfxvn_n0003ymh000zn4/T/AppTranslocation/64A5DD63-BC89-48FA-B593-234A9A4F50BE/d/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/blevishche/study/uchat2/Uchat

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/blevishche/study/uchat2/Uchat/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Uchat.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Uchat.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Uchat.dir/flags.make

CMakeFiles/Uchat.dir/client/src/client.c.o: CMakeFiles/Uchat.dir/flags.make
CMakeFiles/Uchat.dir/client/src/client.c.o: ../client/src/client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/blevishche/study/uchat2/Uchat/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Uchat.dir/client/src/client.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Uchat.dir/client/src/client.c.o   -c /Users/blevishche/study/uchat2/Uchat/client/src/client.c

CMakeFiles/Uchat.dir/client/src/client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Uchat.dir/client/src/client.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/blevishche/study/uchat2/Uchat/client/src/client.c > CMakeFiles/Uchat.dir/client/src/client.c.i

CMakeFiles/Uchat.dir/client/src/client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Uchat.dir/client/src/client.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/blevishche/study/uchat2/Uchat/client/src/client.c -o CMakeFiles/Uchat.dir/client/src/client.c.s

CMakeFiles/Uchat.dir/server/src/main.c.o: CMakeFiles/Uchat.dir/flags.make
CMakeFiles/Uchat.dir/server/src/main.c.o: ../server/src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/blevishche/study/uchat2/Uchat/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Uchat.dir/server/src/main.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Uchat.dir/server/src/main.c.o   -c /Users/blevishche/study/uchat2/Uchat/server/src/main.c

CMakeFiles/Uchat.dir/server/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Uchat.dir/server/src/main.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/blevishche/study/uchat2/Uchat/server/src/main.c > CMakeFiles/Uchat.dir/server/src/main.c.i

CMakeFiles/Uchat.dir/server/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Uchat.dir/server/src/main.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/blevishche/study/uchat2/Uchat/server/src/main.c -o CMakeFiles/Uchat.dir/server/src/main.c.s

# Object files for target Uchat
Uchat_OBJECTS = \
"CMakeFiles/Uchat.dir/client/src/client.c.o" \
"CMakeFiles/Uchat.dir/server/src/main.c.o"

# External object files for target Uchat
Uchat_EXTERNAL_OBJECTS =

Uchat: CMakeFiles/Uchat.dir/client/src/client.c.o
Uchat: CMakeFiles/Uchat.dir/server/src/main.c.o
Uchat: CMakeFiles/Uchat.dir/build.make
Uchat: CMakeFiles/Uchat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/blevishche/study/uchat2/Uchat/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable Uchat"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Uchat.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Uchat.dir/build: Uchat

.PHONY : CMakeFiles/Uchat.dir/build

CMakeFiles/Uchat.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Uchat.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Uchat.dir/clean

CMakeFiles/Uchat.dir/depend:
	cd /Users/blevishche/study/uchat2/Uchat/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/blevishche/study/uchat2/Uchat /Users/blevishche/study/uchat2/Uchat /Users/blevishche/study/uchat2/Uchat/cmake-build-debug /Users/blevishche/study/uchat2/Uchat/cmake-build-debug /Users/blevishche/study/uchat2/Uchat/cmake-build-debug/CMakeFiles/Uchat.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Uchat.dir/depend

