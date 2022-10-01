#################################################################################
# Copyright (c) 2022, Savely Pototsky (SavaLione)
# All rights reserved.
#################################################################################
# Checking the minimum version
cmake_minimum_required (VERSION 3.13.1)

# Add module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Project Lyssa
set(LYSSA_PROJECT "lyssa")
set(LYSSA_PROJECT_VERSION "0.0.1")

project(${LYSSA_PROJECT}
    VERSION ${LYSSA_PROJECT_VERSION}
    LANGUAGES CXX
)

# Set default build to release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -fopenmp")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fopenmp")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -fopenmp")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -DNDEBUG -fopenmp")
else()
    message(STATUS "Unknown build type: " ${CMAKE_BUILD_TYPE})
endif()

message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

# Export commands
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#################################################################################
#                               Macros and functions                            #
#################################################################################
# Output directory
function(function_output_directory arg_project)
    set_target_properties(${arg_project}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" # lib
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" # lib
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endfunction(function_output_directory)

#################################################################################
#                                    Libraries                                  #
#################################################################################
# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

set(GLFW_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/glfw/include" CACHE PATH "GLFW lib include")
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw)

# imgui
set(IMGUI_BUILD_BACKEND_OPENGL3 ON CACHE BOOL "" FORCE)
set(IMGUI_BUILD_BACKEND_GLFW ON CACHE BOOL "" FORCE)

set(IMGUI_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/imgui-cmake/include" CACHE PATH "imgui lib include")
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/imgui-cmake)

# OpenCV
find_package(OpenCV REQUIRED)

# spdlog
find_package(spdlog REQUIRED)

#################################################################################
#                                     Sources                                   #
#################################################################################
# core
set(LYSSA_CORE_SRC
    src/core/lyssa.h
    src/core/lyssa.cpp
    src/core/variables.h
    src/core/variables.cpp
)

# Lyssa application
set(LYSSA_SRC
    ${LYSSA_CORE_SRC}
)

#################################################################################
#                                     Executable                                #
#################################################################################
add_executable(${LYSSA_PROJECT} ${LYSSA_SRC})
target_include_directories(${LYSSA_PROJECT} PUBLIC ${PROJECT_SOURCE_DIR}/src)

# C++ features
target_compile_features(${LYSSA_PROJECT} PRIVATE cxx_auto_type)

#################################################################################
#                                Libraries include                              #
#################################################################################

# OpenGL
target_link_libraries(${LYSSA_PROJECT} ${OPENGL_gl_LIBRARY})
target_include_directories(${LYSSA_PROJECT} PUBLIC ${OPENGL_INCLUDE_DIR})

# GLFW
target_link_libraries(${LYSSA_PROJECT} glfw)
target_include_directories(${LYSSA_PROJECT} PUBLIC ${GLFW_INCLUDE_DIR})

# imgui
target_link_libraries(${LYSSA_PROJECT} imgui imgui_glfw imgui_opengl3)
target_include_directories(${LYSSA_PROJECT} PUBLIC ${IMGUI_INCLUDE_DIR})

# OpenCV
target_link_libraries(${LYSSA_PROJECT} ${OpenCV_LIBS})
target_include_directories(${LYSSA_PROJECT} PUBLIC ${OpenCV_INCLUDE_DIRS})

# spdlog
target_link_libraries(${LYSSA_PROJECT} ${SPDLOG_LIBRARY})
target_include_directories(${LYSSA_PROJECT} PUBLIC ${SPDLOG_INCLUDE_DIR})

#################################################################################
#                                 Output directory                              #
#################################################################################
function_output_directory(glfw)                                     # glfw library
function_output_directory(imgui)                                    # imgui library
function_output_directory(imgui_glfw)                               # imgui_glfw library
function_output_directory(imgui_opengl3)                            # imgui_opengl3 library
function_output_directory(${LYSSA_PROJECT})                         # Lyssa application
