cmake_minimum_required(VERSION 3.0)
project(ALIKE_cpp LANGUAGES CXX CUDA)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE "Release")

### CUDA
find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})

### OpenMP
find_package(OpenMP)
include_directories(${OpenMP_INCLUDE_DIRS})

### Torchlib
find_package(Torch REQUIRED PATHS ${PROJECT_SOURCE_DIR}/libtorch)
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
target_link_libraries(get_patches_cuda ${CUDA_LIBRARIES})

# Install header files and libraries
install(DIRECTORY include/ DESTINATION include)
install(TARGETS get_patches_cuda
        EXPORT ALIKE_cppTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)

# Copy libtorch to the install location (if needed for external use)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/libtorch
        DESTINATION lib/cmake/ALIKE_cpp
        PATTERN "*.so*" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# Export targets for find_package
install(EXPORT ALIKE_cppTargets
        FILE ALIKE_cppTargets.cmake
        NAMESPACE ALIKE_cpp::
        DESTINATION lib/cmake/ALIKE_cpp)

# Generate and install a config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/ALIKE_cppConfigVersion.cmake
        VERSION 1.0
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ALIKE_cppConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/ALIKE_cppConfig.cmake
        INSTALL_DESTINATION lib/cmake/ALIKE_cpp
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ALIKE_cppConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ALIKE_cppConfigVersion.cmake
        DESTINATION lib/cmake/ALIKE_cpp
)

# Optional: Build demo executable
add_executable(demo
        test/main.cpp
)

# Link the CUDA library with the demo executable
target_link_libraries(demo ${TORCH_LIBRARIES} ${OpenCV_LIBS} get_patches_cuda)
