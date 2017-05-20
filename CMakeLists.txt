# Create the project name and other required variables.
cmake_minimum_required (VERSION 2.6)
project (NESCPP17)

#Bring the include files to the project
include_directories(include)

#Glob the source files to a variable.
file(GLOB SOURCES "src/*.cpp")


#Main executable is nesemu
add_executable (nesemu ${SOURCES})
