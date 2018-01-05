cmake_minimum_required(VERSION 3.8)
project(fluidsim)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp DataExport/DataExport.cpp DataExport/DataExport.h Constants.h Core/RK.cpp Core/RK.h Core/Universe.cpp Core/Universe.h DataImport/DataImport.cpp DataImport/DataImport.h Core/Interpolators.cpp Core/Interpolators.h)
add_executable(fluidsim ${SOURCE_FILES})