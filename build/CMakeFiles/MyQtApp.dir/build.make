# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.5/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.5/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/siddharth/Desktop/C++ GoQuant"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/siddharth/Desktop/C++ GoQuant/build"

# Include any dependencies generated for this target.
include CMakeFiles/MyQtApp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MyQtApp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MyQtApp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyQtApp.dir/flags.make

MyQtApp_autogen/timestamp: /opt/homebrew/share/qt/libexec/moc
MyQtApp_autogen/timestamp: CMakeFiles/MyQtApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir="/Users/siddharth/Desktop/C++ GoQuant/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC for target MyQtApp"
	/opt/homebrew/Cellar/cmake/3.30.5/bin/cmake -E cmake_autogen "/Users/siddharth/Desktop/C++ GoQuant/build/CMakeFiles/MyQtApp_autogen.dir/AutogenInfo.json" ""
	/opt/homebrew/Cellar/cmake/3.30.5/bin/cmake -E touch "/Users/siddharth/Desktop/C++ GoQuant/build/MyQtApp_autogen/timestamp"

CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.o: CMakeFiles/MyQtApp.dir/flags.make
CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.o: MyQtApp_autogen/mocs_compilation.cpp
CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.o: CMakeFiles/MyQtApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/siddharth/Desktop/C++ GoQuant/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.o -MF CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.o -c "/Users/siddharth/Desktop/C++ GoQuant/build/MyQtApp_autogen/mocs_compilation.cpp"

CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/siddharth/Desktop/C++ GoQuant/build/MyQtApp_autogen/mocs_compilation.cpp" > CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.i

CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/siddharth/Desktop/C++ GoQuant/build/MyQtApp_autogen/mocs_compilation.cpp" -o CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.s

CMakeFiles/MyQtApp.dir/src/main.cpp.o: CMakeFiles/MyQtApp.dir/flags.make
CMakeFiles/MyQtApp.dir/src/main.cpp.o: /Users/siddharth/Desktop/C++\ GoQuant/src/main.cpp
CMakeFiles/MyQtApp.dir/src/main.cpp.o: CMakeFiles/MyQtApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/siddharth/Desktop/C++ GoQuant/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/MyQtApp.dir/src/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MyQtApp.dir/src/main.cpp.o -MF CMakeFiles/MyQtApp.dir/src/main.cpp.o.d -o CMakeFiles/MyQtApp.dir/src/main.cpp.o -c "/Users/siddharth/Desktop/C++ GoQuant/src/main.cpp"

CMakeFiles/MyQtApp.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/MyQtApp.dir/src/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/siddharth/Desktop/C++ GoQuant/src/main.cpp" > CMakeFiles/MyQtApp.dir/src/main.cpp.i

CMakeFiles/MyQtApp.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/MyQtApp.dir/src/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/siddharth/Desktop/C++ GoQuant/src/main.cpp" -o CMakeFiles/MyQtApp.dir/src/main.cpp.s

CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.o: CMakeFiles/MyQtApp.dir/flags.make
CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.o: /Users/siddharth/Desktop/C++\ GoQuant/src/MainWindow.cpp
CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.o: CMakeFiles/MyQtApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/siddharth/Desktop/C++ GoQuant/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.o -MF CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.o.d -o CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.o -c "/Users/siddharth/Desktop/C++ GoQuant/src/MainWindow.cpp"

CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/siddharth/Desktop/C++ GoQuant/src/MainWindow.cpp" > CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.i

CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/siddharth/Desktop/C++ GoQuant/src/MainWindow.cpp" -o CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.s

# Object files for target MyQtApp
MyQtApp_OBJECTS = \
"CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/MyQtApp.dir/src/main.cpp.o" \
"CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.o"

# External object files for target MyQtApp
MyQtApp_EXTERNAL_OBJECTS =

MyQtApp: CMakeFiles/MyQtApp.dir/MyQtApp_autogen/mocs_compilation.cpp.o
MyQtApp: CMakeFiles/MyQtApp.dir/src/main.cpp.o
MyQtApp: CMakeFiles/MyQtApp.dir/src/MainWindow.cpp.o
MyQtApp: CMakeFiles/MyQtApp.dir/build.make
MyQtApp: /opt/homebrew/lib/QtWidgets.framework/Versions/A/QtWidgets
MyQtApp: /opt/homebrew/lib/QtWebSockets.framework/Versions/A/QtWebSockets
MyQtApp: /opt/homebrew/lib/QtGui.framework/Versions/A/QtGui
MyQtApp: /opt/homebrew/lib/QtNetwork.framework/Versions/A/QtNetwork
MyQtApp: /opt/homebrew/lib/QtCore.framework/Versions/A/QtCore
MyQtApp: CMakeFiles/MyQtApp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/siddharth/Desktop/C++ GoQuant/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable MyQtApp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MyQtApp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MyQtApp.dir/build: MyQtApp
.PHONY : CMakeFiles/MyQtApp.dir/build

CMakeFiles/MyQtApp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MyQtApp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MyQtApp.dir/clean

CMakeFiles/MyQtApp.dir/depend: MyQtApp_autogen/timestamp
	cd "/Users/siddharth/Desktop/C++ GoQuant/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/siddharth/Desktop/C++ GoQuant" "/Users/siddharth/Desktop/C++ GoQuant" "/Users/siddharth/Desktop/C++ GoQuant/build" "/Users/siddharth/Desktop/C++ GoQuant/build" "/Users/siddharth/Desktop/C++ GoQuant/build/CMakeFiles/MyQtApp.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/MyQtApp.dir/depend

