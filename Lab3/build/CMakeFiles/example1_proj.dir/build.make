# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/nvidia/workspace/Lab3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nvidia/workspace/Lab3/build

# Include any dependencies generated for this target.
include CMakeFiles/example1_proj.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/example1_proj.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/example1_proj.dir/flags.make

CMakeFiles/example1_proj.dir/example1L3.cpp.o: CMakeFiles/example1_proj.dir/flags.make
CMakeFiles/example1_proj.dir/example1L3.cpp.o: ../example1L3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nvidia/workspace/Lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/example1_proj.dir/example1L3.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example1_proj.dir/example1L3.cpp.o -c /home/nvidia/workspace/Lab3/example1L3.cpp

CMakeFiles/example1_proj.dir/example1L3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example1_proj.dir/example1L3.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nvidia/workspace/Lab3/example1L3.cpp > CMakeFiles/example1_proj.dir/example1L3.cpp.i

CMakeFiles/example1_proj.dir/example1L3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example1_proj.dir/example1L3.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nvidia/workspace/Lab3/example1L3.cpp -o CMakeFiles/example1_proj.dir/example1L3.cpp.s

CMakeFiles/example1_proj.dir/example1L3.cpp.o.requires:

.PHONY : CMakeFiles/example1_proj.dir/example1L3.cpp.o.requires

CMakeFiles/example1_proj.dir/example1L3.cpp.o.provides: CMakeFiles/example1_proj.dir/example1L3.cpp.o.requires
	$(MAKE) -f CMakeFiles/example1_proj.dir/build.make CMakeFiles/example1_proj.dir/example1L3.cpp.o.provides.build
.PHONY : CMakeFiles/example1_proj.dir/example1L3.cpp.o.provides

CMakeFiles/example1_proj.dir/example1L3.cpp.o.provides.build: CMakeFiles/example1_proj.dir/example1L3.cpp.o


# Object files for target example1_proj
example1_proj_OBJECTS = \
"CMakeFiles/example1_proj.dir/example1L3.cpp.o"

# External object files for target example1_proj
example1_proj_EXTERNAL_OBJECTS =

example1_proj: CMakeFiles/example1_proj.dir/example1L3.cpp.o
example1_proj: CMakeFiles/example1_proj.dir/build.make
example1_proj: /usr/local/lib/libopencv_superres.so.3.4.1
example1_proj: /usr/local/lib/libopencv_cudaobjdetect.so.3.4.1
example1_proj: /usr/local/lib/libopencv_ml.so.3.4.1
example1_proj: /usr/local/lib/libopencv_videostab.so.3.4.1
example1_proj: /usr/local/lib/libopencv_photo.so.3.4.1
example1_proj: /usr/local/lib/libopencv_cudabgsegm.so.3.4.1
example1_proj: /usr/local/lib/libopencv_cudastereo.so.3.4.1
example1_proj: /usr/local/lib/libopencv_shape.so.3.4.1
example1_proj: /usr/local/lib/libopencv_stitching.so.3.4.1
example1_proj: /usr/local/lib/libopencv_cudafeatures2d.so.3.4.1
example1_proj: /usr/local/lib/libopencv_cudacodec.so.3.4.1
example1_proj: /usr/local/lib/libopencv_dnn.so.3.4.1
example1_proj: /usr/local/lib/libopencv_cudaoptflow.so.3.4.1
example1_proj: /usr/local/lib/libopencv_cudalegacy.so.3.4.1
example1_proj: /usr/local/lib/libopencv_video.so.3.4.1
example1_proj: /usr/local/lib/libopencv_objdetect.so.3.4.1
example1_proj: /usr/local/lib/libopencv_calib3d.so.3.4.1
example1_proj: /usr/local/lib/libopencv_features2d.so.3.4.1
example1_proj: /usr/local/lib/libopencv_highgui.so.3.4.1
example1_proj: /usr/local/lib/libopencv_videoio.so.3.4.1
example1_proj: /usr/local/lib/libopencv_cudawarping.so.3.4.1
example1_proj: /usr/local/lib/libopencv_cudaimgproc.so.3.4.1
example1_proj: /usr/local/lib/libopencv_cudafilters.so.3.4.1
example1_proj: /usr/local/lib/libopencv_cudaarithm.so.3.4.1
example1_proj: /usr/local/lib/libopencv_flann.so.3.4.1
example1_proj: /usr/local/lib/libopencv_imgcodecs.so.3.4.1
example1_proj: /usr/local/lib/libopencv_imgproc.so.3.4.1
example1_proj: /usr/local/lib/libopencv_core.so.3.4.1
example1_proj: /usr/local/lib/libopencv_cudev.so.3.4.1
example1_proj: CMakeFiles/example1_proj.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nvidia/workspace/Lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable example1_proj"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/example1_proj.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/example1_proj.dir/build: example1_proj

.PHONY : CMakeFiles/example1_proj.dir/build

CMakeFiles/example1_proj.dir/requires: CMakeFiles/example1_proj.dir/example1L3.cpp.o.requires

.PHONY : CMakeFiles/example1_proj.dir/requires

CMakeFiles/example1_proj.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/example1_proj.dir/cmake_clean.cmake
.PHONY : CMakeFiles/example1_proj.dir/clean

CMakeFiles/example1_proj.dir/depend:
	cd /home/nvidia/workspace/Lab3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nvidia/workspace/Lab3 /home/nvidia/workspace/Lab3 /home/nvidia/workspace/Lab3/build /home/nvidia/workspace/Lab3/build /home/nvidia/workspace/Lab3/build/CMakeFiles/example1_proj.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/example1_proj.dir/depend

