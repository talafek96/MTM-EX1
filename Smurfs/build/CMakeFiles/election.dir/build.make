# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Smurfs"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Smurfs\build"

# Include any dependencies generated for this target.
include CMakeFiles/election.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/election.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/election.dir/flags.make

CMakeFiles/election.dir/election.c.obj: CMakeFiles/election.dir/flags.make
CMakeFiles/election.dir/election.c.obj: ../election.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Smurfs\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/election.dir/election.c.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\X8EAA8~1.EXE $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\election.dir\election.c.obj   -c "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Smurfs\election.c"

CMakeFiles/election.dir/election.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/election.dir/election.c.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\X8EAA8~1.EXE $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Smurfs\election.c" > CMakeFiles\election.dir\election.c.i

CMakeFiles/election.dir/election.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/election.dir/election.c.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\X8EAA8~1.EXE $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Smurfs\election.c" -o CMakeFiles\election.dir\election.c.s

# Object files for target election
election_OBJECTS = \
"CMakeFiles/election.dir/election.c.obj"

# External object files for target election
election_EXTERNAL_OBJECTS =

libelection.a: CMakeFiles/election.dir/election.c.obj
libelection.a: CMakeFiles/election.dir/build.make
libelection.a: CMakeFiles/election.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Smurfs\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libelection.a"
	$(CMAKE_COMMAND) -P CMakeFiles\election.dir\cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\election.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/election.dir/build: libelection.a

.PHONY : CMakeFiles/election.dir/build

CMakeFiles/election.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\election.dir\cmake_clean.cmake
.PHONY : CMakeFiles/election.dir/clean

CMakeFiles/election.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Smurfs" "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Smurfs" "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Smurfs\build" "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Smurfs\build" "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Smurfs\build\CMakeFiles\election.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/election.dir/depend
