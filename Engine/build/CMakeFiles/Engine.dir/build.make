# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.31

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\daves\Projects\plant_game\Engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\daves\Projects\plant_game\Engine\build

# Include any dependencies generated for this target.
include CMakeFiles/Engine.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Engine.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Engine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Engine.dir/flags.make

CMakeFiles/Engine.dir/codegen:
.PHONY : CMakeFiles/Engine.dir/codegen

CMakeFiles/Engine.dir/src/main.cpp.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/src/main.cpp.obj: CMakeFiles/Engine.dir/includes_CXX.rsp
CMakeFiles/Engine.dir/src/main.cpp.obj: C:/Users/daves/Projects/plant_game/Engine/src/main.cpp
CMakeFiles/Engine.dir/src/main.cpp.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\daves\Projects\plant_game\Engine\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Engine.dir/src/main.cpp.obj"
	C:\msys64\ucrt64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Engine.dir/src/main.cpp.obj -MF CMakeFiles\Engine.dir\src\main.cpp.obj.d -o CMakeFiles\Engine.dir\src\main.cpp.obj -c C:\Users\daves\Projects\plant_game\Engine\src\main.cpp

CMakeFiles/Engine.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Engine.dir/src/main.cpp.i"
	C:\msys64\ucrt64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\daves\Projects\plant_game\Engine\src\main.cpp > CMakeFiles\Engine.dir\src\main.cpp.i

CMakeFiles/Engine.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/src/main.cpp.s"
	C:\msys64\ucrt64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\daves\Projects\plant_game\Engine\src\main.cpp -o CMakeFiles\Engine.dir\src\main.cpp.s

# Object files for target Engine
Engine_OBJECTS = \
"CMakeFiles/Engine.dir/src/main.cpp.obj"

# External object files for target Engine
Engine_EXTERNAL_OBJECTS =

Engine.exe: CMakeFiles/Engine.dir/src/main.cpp.obj
Engine.exe: CMakeFiles/Engine.dir/build.make
Engine.exe: libs/glfw/src/libglfw3.a
Engine.exe: libglad.a
Engine.exe: CMakeFiles/Engine.dir/linkLibs.rsp
Engine.exe: CMakeFiles/Engine.dir/objects1.rsp
Engine.exe: CMakeFiles/Engine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\daves\Projects\plant_game\Engine\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Engine.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Engine.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Engine.dir/build: Engine.exe
.PHONY : CMakeFiles/Engine.dir/build

CMakeFiles/Engine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Engine.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Engine.dir/clean

CMakeFiles/Engine.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\daves\Projects\plant_game\Engine C:\Users\daves\Projects\plant_game\Engine C:\Users\daves\Projects\plant_game\Engine\build C:\Users\daves\Projects\plant_game\Engine\build C:\Users\daves\Projects\plant_game\Engine\build\CMakeFiles\Engine.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Engine.dir/depend

