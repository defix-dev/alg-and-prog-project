# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Users/defix/Documents/GitHub/alg-and-prog-project/AppModule

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Users/defix/Documents/GitHub/alg-and-prog-project/AppModule/build

# Include any dependencies generated for this target.
include CMakeFiles/MyExecutable.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MyExecutable.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MyExecutable.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyExecutable.dir/flags.make

CMakeFiles/MyExecutable.dir/codegen:
.PHONY : CMakeFiles/MyExecutable.dir/codegen

CMakeFiles/MyExecutable.dir/src/main.cpp.obj: CMakeFiles/MyExecutable.dir/flags.make
CMakeFiles/MyExecutable.dir/src/main.cpp.obj: CMakeFiles/MyExecutable.dir/includes_CXX.rsp
CMakeFiles/MyExecutable.dir/src/main.cpp.obj: C:/Users/defix/Documents/GitHub/alg-and-prog-project/AppModule/src/main.cpp
CMakeFiles/MyExecutable.dir/src/main.cpp.obj: CMakeFiles/MyExecutable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:/Users/defix/Documents/GitHub/alg-and-prog-project/AppModule/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MyExecutable.dir/src/main.cpp.obj"
	C:/MinGW/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MyExecutable.dir/src/main.cpp.obj -MF CMakeFiles/MyExecutable.dir/src/main.cpp.obj.d -o CMakeFiles/MyExecutable.dir/src/main.cpp.obj -c C:/Users/defix/Documents/GitHub/alg-and-prog-project/AppModule/src/main.cpp

CMakeFiles/MyExecutable.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/MyExecutable.dir/src/main.cpp.i"
	C:/MinGW/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/defix/Documents/GitHub/alg-and-prog-project/AppModule/src/main.cpp > CMakeFiles/MyExecutable.dir/src/main.cpp.i

CMakeFiles/MyExecutable.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/MyExecutable.dir/src/main.cpp.s"
	C:/MinGW/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/defix/Documents/GitHub/alg-and-prog-project/AppModule/src/main.cpp -o CMakeFiles/MyExecutable.dir/src/main.cpp.s

# Object files for target MyExecutable
MyExecutable_OBJECTS = \
"CMakeFiles/MyExecutable.dir/src/main.cpp.obj"

# External object files for target MyExecutable
MyExecutable_EXTERNAL_OBJECTS =

MyExecutable.exe: CMakeFiles/MyExecutable.dir/src/main.cpp.obj
MyExecutable.exe: CMakeFiles/MyExecutable.dir/build.make
MyExecutable.exe: CMakeFiles/MyExecutable.dir/linkLibs.rsp
MyExecutable.exe: CMakeFiles/MyExecutable.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:/Users/defix/Documents/GitHub/alg-and-prog-project/AppModule/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable MyExecutable.exe"
	"C:/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/MyExecutable.dir/objects.a
	C:/MinGW/bin/ar.exe qc CMakeFiles/MyExecutable.dir/objects.a @CMakeFiles/MyExecutable.dir/objects1.rsp
	C:/MinGW/bin/g++.exe -g -Wl,--whole-archive CMakeFiles/MyExecutable.dir/objects.a -Wl,--no-whole-archive -o MyExecutable.exe -Wl,--out-implib,libMyExecutable.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/MyExecutable.dir/linkLibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/MyExecutable.dir/build: MyExecutable.exe
.PHONY : CMakeFiles/MyExecutable.dir/build

CMakeFiles/MyExecutable.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MyExecutable.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MyExecutable.dir/clean

CMakeFiles/MyExecutable.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Users/defix/Documents/GitHub/alg-and-prog-project/AppModule C:/Users/defix/Documents/GitHub/alg-and-prog-project/AppModule C:/Users/defix/Documents/GitHub/alg-and-prog-project/AppModule/build C:/Users/defix/Documents/GitHub/alg-and-prog-project/AppModule/build C:/Users/defix/Documents/GitHub/alg-and-prog-project/AppModule/build/CMakeFiles/MyExecutable.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/MyExecutable.dir/depend

