cmake_minimum_required(VERSION 3.0)
project(ALIKE_cpp LANGUAGES CXX CUDA)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")

### CUDA
find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})

### OpenMP
find_package(OpenMP)
include_directories(${OpenMP_INCLUDE_DIRS})

set(ALIKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/lib/cmake/ALIKE_cpp")

### Torchlib you need to copy libtorch in /usr/local/lib
find_package(Torch REQUIRED PATHS /usr/local/lib/libtorch)
message(STATUS "Found Torch include is ${TORCH_INCLUDE_DIRS}")

# Add Torch include directories explicitly if needed
include_directories(${TORCH_INCLUDE_DIRS} ${OPENCV_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -Wno-deprecated-gpu-targets")

### OPENCV
find_package(OpenCV REQUIRED)
message(STATUS "Found OpenCV version is ${OpenCV_VERSION}")
message(STATUS "Found OpenCV include is ${OpenCV_INCLUDE_DIRS}")
include_directories(${OpenCV_INCLUDE_DIRS})

# Create CUDA library
include_directories(include)
add_library(get_patches_cuda SHARED src/get_patches.cu)
set_target_properties(get_patches_cuda PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

# Link CUDA runtime libraries
target_link_libraries(get_patches_cuda ${CUDA_LIBRARIES} ${TORCH_LIBRARIES} ${OpenCV_LIBS})

# Install header files and libraries
install(DIRECTORY include/ DESTINATION include)
install(TARGETS get_patches_cuda
        EXPORT ALIKE_cppTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)

# Export targets for find_package
install(EXPORT ALIKE_cppTargets
        FILE ALIKE_cppTargets.cmake
        NAMESPACE ALIKE_cpp::
        DESTINATION ${ALIKE_INSTALL_PREFIX})

# Generate and install a config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/ALIKE_cppConfigVersion.cmake
        VERSION 1.0
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ALIKEConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/ALIKE_cppConfig.cmake
        INSTALL_DESTINATION ${ALIKE_INSTALL_PREFIX}
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ALIKE_cppConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ALIKE_cppConfigVersion.cmake
        DESTINATION ${ALIKE_INSTALL_PREFIX}
)

# Optional: Build demo executable
#add_executable(demo
#        test/main.cpp
#)

## Link the CUDA library with the demo executable
#target_link_libraries(demo ${TORCH_LIBRARIES} ${OpenCV_LIBS} get_patches_cuda)
