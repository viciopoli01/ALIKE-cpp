cmake_minimum_required(VERSION 3.14)  # Needed for good FetchContent / target support
project(ALIKE_test LANGUAGES CXX CUDA)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Find ALIKE (assumes ALIKE installs a config file using install(FILES ...))
find_package(ALIKE_cpp REQUIRED)

# Find OpenCV if needed
find_package(OpenCV REQUIRED)

# Add executable
add_executable(demo main.cpp)

# Link executable against ALIKE_cpp
target_link_libraries(demo
        ALIKE_cpp::get_patches_cuda
        ${OpenCV_LIBS}
        ${TORCH_LIBRARIES}
)