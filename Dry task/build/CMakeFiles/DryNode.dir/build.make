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
CMAKE_SOURCE_DIR = "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Dry\Nodes"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Dry\Nodes\build"

# Include any dependencies generated for this target.
include CMakeFiles/DryNode.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/DryNode.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DryNode.dir/flags.make

CMakeFiles/DryNode.dir/node.c.obj: CMakeFiles/DryNode.dir/flags.make
CMakeFiles/DryNode.dir/node.c.obj: ../node.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Dry\Nodes\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/DryNode.dir/node.c.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\DryNode.dir\node.c.obj   -c "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Dry\Nodes\node.c"

CMakeFiles/DryNode.dir/node.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DryNode.dir/node.c.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Dry\Nodes\node.c" > CMakeFiles\DryNode.dir\node.c.i

CMakeFiles/DryNode.dir/node.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DryNode.dir/node.c.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Dry\Nodes\node.c" -o CMakeFiles\DryNode.dir\node.c.s

# Object files for target DryNode
DryNode_OBJECTS = \
"CMakeFiles/DryNode.dir/node.c.obj"

# External object files for target DryNode
DryNode_EXTERNAL_OBJECTS =

DryNode.exe: CMakeFiles/DryNode.dir/node.c.obj
DryNode.exe: CMakeFiles/DryNode.dir/build.make
DryNode.exe: CMakeFiles/DryNode.dir/linklibs.rsp
DryNode.exe: CMakeFiles/DryNode.dir/objects1.rsp
DryNode.exe: CMakeFiles/DryNode.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Dry\Nodes\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable DryNode.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\DryNode.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DryNode.dir/build: DryNode.exe

.PHONY : CMakeFiles/DryNode.dir/build

CMakeFiles/DryNode.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\DryNode.dir\cmake_clean.cmake
.PHONY : CMakeFiles/DryNode.dir/clean

CMakeFiles/DryNode.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Dry\Nodes" "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Dry\Nodes" "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Dry\Nodes\build" "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Dry\Nodes\build" "C:\Users\Tal Afek\Documents\Technion\Semester2\Matam\HW1\Dry\Nodes\build\CMakeFiles\DryNode.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/DryNode.dir/depend

