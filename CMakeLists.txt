cmake_minimum_required(VERSION 3.15)
project(orbslam3 LANGUAGES CXX VERSION 0.0.1)

LIST(APPEND CMAKE_MODULE_PATH 
    ${PROJECT_SOURCE_DIR}/cmake
)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module NumPy)
find_package(pybind11 REQUIRED)
find_package(OpenCV 4.4 REQUIRED)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
add_subdirectory(ORB_SLAM3)

# Create the Python module
pybind11_add_module(orbslam3 
    src/ORBSLAM3Wrapper.cpp
    src/NDArrayConverter.cpp
)

# Link against the static ORB_SLAM3 library
target_link_libraries(orbslam3 PRIVATE
    ORB_SLAM3
    ${OpenCV_LIBS}
)

target_compile_definitions(orbslam3 PRIVATE 
    VERSION_INFO=${PROJECT_VERSION}
)

target_include_directories(orbslam3 PRIVATE
    ${Python3_NumPy_INCLUDE_DIRS}
)

target_include_directories(orbslam3 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/ORB_SLAM3>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/ORB_SLAM3/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/ORB_SLAM3/include/CameraModels>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/ORB_SLAM3/Thirdparty/g2o/g2o>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/ORB_SLAM3/Thirdparty/DBoW2/DBoW2>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/ORB_SLAM3/Thirdparty/Sophus/sophus>
)

# Install the main Python module
install(TARGETS orbslam3 DESTINATION orbslam3)
