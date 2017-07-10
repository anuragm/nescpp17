# Create the project name and other required variables.
cmake_minimum_required (VERSION 2.6)

#------------------------------------------
# GLOBAL COMPILER FEATURES

# Do NOT want vendor extensions (e.g. no --std=gnu++14)
set(CMAKE_CXX_EXTENSIONS OFF)

#Include warnings.
add_definitions(-Wall -Wextra -pedantic)

#--------------------------------------------------
# Project specific features.
project (NESCPP17)

#Create a compilation database for use of other tools.
# This should be set after project declaration.
# See http://archive.is/aBhI9
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Bring the include files to the project
include_directories(include)

#Glob the source files to a variable.
file(GLOB SOURCES "src/*.cpp")

#Main executable is nesemu
add_executable (nesemu ${SOURCES})

## --------------------------------------------------------
#And add required complier features
set_property(TARGET nesemu PROPERTY CXX_STANDARD 14)
