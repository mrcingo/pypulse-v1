# MIT License
#
# Copyright (c) 2025 PyPulse
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.13)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# CEF library will start to give errors unless the C++ standard
# is set to 17.
set(CMAKE_CXX_STANDARD 17)

# 
# Project configuration
# 

project("CefPulseApp")

# CEF
set_property(GLOBAL PROPERTY OS_FOLDERS ON)
set(CEF_VERSION "138.0.26+g84f2d27+chromium-138.0.7204.158")

# Platform
if(NOT DEFINED CEF_PLATFORM)

    # Currently PyPulse has no support for other platforms.
    # TODO: MacOS (Darwin) & Linux support
    if (NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
        message(FATAL_ERROR "PyPulse does not support non-Windows platforms.")
    endif()

    # Check if the system is 64 bits.
    if(CMAKE_SIZEOF_VOID_P MATCHES 8)
        set(CEF_PLATFORM "windows64")
    else()
        set(CEF_PLATFORM "windows32")
    endif()

endif()

# Add this project's cmake/ directory to the module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Download and extract the CEF binary distribution (see cmake/Download.cmake)
include(Download)
downloadcef("${CEF_PLATFORM}" "${CEF_VERSION}" "${CMAKE_SOURCE_DIR}/build/cef")

# Add the CEF binary distribution's cmake/ directory to the module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_ROOT}/cmake")

# Load the CEF configuration (executes FindCEF.cmake)
find_package(CEF REQUIRED)

#
# Target configuration
#

# Add pybind11 to the project (this library generates the binds between Python and C++) 
add_subdirectory(pybind11)

# Include the libcef_dll_wrapper target (executes libcef_dll/CMakeLists.txt)
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

# See src/core/CMakeLists.txt
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/core)

# Display configuration settings
print_cef_config()
