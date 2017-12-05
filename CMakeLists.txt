cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

# Prevent dereference variable in if statement
cmake_policy(SET CMP0054 NEW)

# Force cmake generate C++14 compiler flag instead of -std=gnu++11 in Linux.
set(CMAKE_CXX_STANDARD 14)

# Colorful output for ninja
string(TOLOWER ${CMAKE_GENERATOR} CMakeGenLower)
if (CMakeGenLower STREQUAL "ninja")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=always")
endif()


project("depthToSurface")

set(Boost_USE_STATIC_LIBS FALSE CACHE BOOL "Use Boost statis library")

find_package(OpenMP)
find_package(Eigen3 REQUIRED)
#find_package(OpenCV REQUIRED COMPONENTS "opencv_core;opencv_highgui;opencv_imgcodecs;opencv_vis")
find_package(Boost REQUIRED COMPONENTS "filesystem;serialization;system;timer")
find_package(PCL REQUIRED COMPONENTS "")


## Executable: kittiScanToPly
# Write KITTI velodyne scanned data (*.bin) to binary ply file
add_executable(kittiScanToPly kittiScanToPly.cpp)
target_compile_definitions(kittiScanToPly PRIVATE ${PCL_DEFINITIONS})
if (NOT Boost_USE_STATIC_LIBS)
    target_compile_definitions(kittiScanToPly PRIVATE BOOST_ALL_DYN_LINK)
endif()
target_include_directories(kittiScanToPly PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${PCL_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIR}
#    ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(kittiScanToPly PRIVATE
    ${Boost_LIBRARIES}
    ${PCL_LIBRARIES}
#    ${OpenCV_LIBS}
)

