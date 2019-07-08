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
CMAKE_SOURCE_DIR = /home/nvidia/workspace/ColorFilt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nvidia/workspace/ColorFilt/build

# Include any dependencies generated for this target.
include CMakeFiles/color_filter_proj.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/color_filter_proj.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/color_filter_proj.dir/flags.make

CMakeFiles/color_filter_proj.dir/color_filter.cpp.o: CMakeFiles/color_filter_proj.dir/flags.make
CMakeFiles/color_filter_proj.dir/color_filter.cpp.o: ../color_filter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nvidia/workspace/ColorFilt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/color_filter_proj.dir/color_filter.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/color_filter_proj.dir/color_filter.cpp.o -c /home/nvidia/workspace/ColorFilt/color_filter.cpp

CMakeFiles/color_filter_proj.dir/color_filter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/color_filter_proj.dir/color_filter.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nvidia/workspace/ColorFilt/color_filter.cpp > CMakeFiles/color_filter_proj.dir/color_filter.cpp.i

CMakeFiles/color_filter_proj.dir/color_filter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/color_filter_proj.dir/color_filter.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nvidia/workspace/ColorFilt/color_filter.cpp -o CMakeFiles/color_filter_proj.dir/color_filter.cpp.s

CMakeFiles/color_filter_proj.dir/color_filter.cpp.o.requires:

.PHONY : CMakeFiles/color_filter_proj.dir/color_filter.cpp.o.requires

CMakeFiles/color_filter_proj.dir/color_filter.cpp.o.provides: CMakeFiles/color_filter_proj.dir/color_filter.cpp.o.requires
	$(MAKE) -f CMakeFiles/color_filter_proj.dir/build.make CMakeFiles/color_filter_proj.dir/color_filter.cpp.o.provides.build
.PHONY : CMakeFiles/color_filter_proj.dir/color_filter.cpp.o.provides

CMakeFiles/color_filter_proj.dir/color_filter.cpp.o.provides.build: CMakeFiles/color_filter_proj.dir/color_filter.cpp.o


# Object files for target color_filter_proj
color_filter_proj_OBJECTS = \
"CMakeFiles/color_filter_proj.dir/color_filter.cpp.o"

# External object files for target color_filter_proj
color_filter_proj_EXTERNAL_OBJECTS =

color_filter_proj: CMakeFiles/color_filter_proj.dir/color_filter.cpp.o
color_filter_proj: CMakeFiles/color_filter_proj.dir/build.make
color_filter_proj: /usr/local/lib/libopencv_superres.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_cudaobjdetect.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_ml.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_videostab.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_photo.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_cudabgsegm.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_cudastereo.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_shape.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_stitching.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_cudafeatures2d.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_cudacodec.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_dnn.so.3.4.1
color_filter_proj: /usr/local/cuda-9.0/lib64/libcudart_static.a
color_filter_proj: /usr/lib/aarch64-linux-gnu/librt.so
color_filter_proj: /usr/local/lib/libopencv_cudaoptflow.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_cudalegacy.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_video.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_objdetect.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_calib3d.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_features2d.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_highgui.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_videoio.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_cudawarping.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_cudaimgproc.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_cudafilters.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_cudaarithm.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_flann.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_imgcodecs.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_imgproc.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_core.so.3.4.1
color_filter_proj: /usr/local/lib/libopencv_cudev.so.3.4.1
color_filter_proj: CMakeFiles/color_filter_proj.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nvidia/workspace/ColorFilt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable color_filter_proj"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/color_filter_proj.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/color_filter_proj.dir/build: color_filter_proj

.PHONY : CMakeFiles/color_filter_proj.dir/build

CMakeFiles/color_filter_proj.dir/requires: CMakeFiles/color_filter_proj.dir/color_filter.cpp.o.requires

.PHONY : CMakeFiles/color_filter_proj.dir/requires

CMakeFiles/color_filter_proj.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/color_filter_proj.dir/cmake_clean.cmake
.PHONY : CMakeFiles/color_filter_proj.dir/clean

CMakeFiles/color_filter_proj.dir/depend:
	cd /home/nvidia/workspace/ColorFilt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nvidia/workspace/ColorFilt /home/nvidia/workspace/ColorFilt /home/nvidia/workspace/ColorFilt/build /home/nvidia/workspace/ColorFilt/build /home/nvidia/workspace/ColorFilt/build/CMakeFiles/color_filter_proj.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/color_filter_proj.dir/depend

