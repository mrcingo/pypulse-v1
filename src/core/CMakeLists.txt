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

#
# Core
# -----
# Here is where all the bindings and C++ code is compiled using pybind11. 
#

set(APP_SRCS
    CefPulseApp.cpp
    CefPulseApp.h

    CefPulseHandler.cpp
    CefPulseHandler.h

    CefPulseUtils.cpp
    CefPulseUtils.h
)

append_platform_sources(APP_SRCS)

# Shared configuration
if(OS_WINDOWS)
    add_logical_target("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
endif()

#
# Windows support configuration
#

if(OS_WINDOWS)
    add_library(${CMAKE_PROJECT_NAME} STATIC ${APP_SRCS})

    add_dependencies(${CMAKE_PROJECT_NAME} libcef_dll_wrapper)

    target_link_libraries(${CMAKE_PROJECT_NAME}
        libcef_lib
        libcef_dll_wrapper
        ${CEF_STANDARD_LIBS}
    )
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ../..)

    # Subprocess executable path
    add_executable(cefpulsesub CefPulseMain.cpp)

    set_target_properties(cefpulsesub PROPERTIES WIN32_EXECUTABLE TRUE)

    target_compile_definitions(cefpulsesub PRIVATE UNICODE _UNICODE)

    add_dependencies(cefpulsesub libcef_dll_wrapper)

    target_link_libraries(cefpulsesub PRIVATE 
        ${CMAKE_PROJECT_NAME}
        libcef_lib
        libcef_dll_wrapper
        ${CEF_STANDARD_LIBS}
    )
    
    target_include_directories(cefpulsesub PRIVATE ../..)

    set_target_properties(cefpulsesub PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "$<TARGET_FILE_DIR:cefpulse>"
    )
endif()

#
# Bindings
#

# Add all C++ bindings and transforming them into a Python 
# using pybind11
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/bind)
