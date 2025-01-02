cmake_minimum_required(VERSION 3.0)
project(ALIKE_test LANGUAGES CXX CUDA)

# Set the C++ standard to C++14
set(CMAKE_CXX_STANDARD 14)

# Set the build type to Release
set(CMAKE_BUILD_TYPE "Release")

# Find the ALIKE_cpp package
find_package(ALIKE_cpp REQUIRED)

# Optional: Build demo executable
add_executable(demo
        main.cpp
)

# Link the CUDA library with the demo executable
target_link_libraries(demo ${TORCH_LIBRARIES} ${OpenCV_LIBS} ALIKE_cpp::get_patches_cuda)
