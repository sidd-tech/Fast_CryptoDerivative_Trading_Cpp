# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.14)

# Set the project name and version
project(MyQtProject VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find and link Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets WebSockets)

set(CMAKE_AUTOMOC ON)  # Automatically run moc for headers with Q_OBJECT

add_executable(MyQtApp src/main.cpp src/MainWindow.cpp src/MainWindow.h)

# Link Qt libraries to the project
target_link_libraries(MyQtApp PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::WebSockets)

# Include directories for Qt
target_include_directories(MyQtApp PRIVATE ${Qt6Core_INCLUDE_DIRS})