cmake_minimum_required(VERSION 3.18.0)
project(SeamCarving LANGUAGES CXX CUDA VERSION 0.1.0)

include(CheckLanguage)
check_language(CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CUDA_STANDARD_REQUIRED True)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/lib)

# message("CMAKE_HOME_DIRECTORY->${CMAKE_HOME_DIRECTORY}")
# message("OpenCV_INCLUDE_DIRS->${OpenCV_INCLUDE_DIRS}")
# message("OpenCV_LIBS->${OpenCV_LIBS}")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

file(GLOB SRC_FILES 
    "src/*.cpp"
    "src/*.h"
)

set(SOURCES ${SRC_FILES})

add_executable(SeamCarving ${SOURCES})

target_link_libraries( SeamCarving ${OpenCV_LIBS} )
