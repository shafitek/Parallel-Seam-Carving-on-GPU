cmake_minimum_required(VERSION 3.18)
project(SeamCarving LANGUAGES CUDA CXX VERSION 0.1.0)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CUDA_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_CUDA_STANDARD_REQUIRED True)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(CUDAToolkit)

find_library(CUDART_LIBRARY cudart ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})

set( CPU_SRC_FILES 
src/CPUCarver.cpp 
src/CPUCarver.h 
src/GPUCarver.cpp 
src/GPUCarver.h 
src/SeamCarver.cpp 
src/SeamCarver.h 
src/utils.cpp 
src/utils.h 
src/main.cpp )

add_library(CarvingKernels STATIC src/EnergyKernels.cu src/EnergyKernels.cuh)
set_target_properties( CarvingKernels PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_include_directories( CarvingKernels PRIVATE ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} )
target_link_libraries( CarvingKernels ${CUDART_LIBRARY})
# Would need at least the pascal architecture
target_compile_options(
    CarvingKernels PRIVATE 
    $<$<COMPILE_LANGUAGE:CUDA>:-gencode=arch=compute_61,code=\"sm_61,compute_61\">
) 

set(SOURCES ${CPU_SRC_FILES})
add_executable(SeamCarving ${SOURCES})
target_link_libraries( SeamCarving PUBLIC CarvingKernels )
target_link_libraries( SeamCarving PUBLIC ${OpenCV_LIBS} )
