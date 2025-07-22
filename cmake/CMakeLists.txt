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

set(APP_SRCS
    ../src/core/CefPulseApp.cpp
    ../src/core/CefPulseApp.h

    ../src/core/CefPulseHandler.cpp
    ../src/core/CefPulseHandler.h

    ../src/core/CefPulseUtils.cpp
    ../src/core/CefPulseUtils.h
)

append_platform_sources(APP_SRCS)

if(OS_WINDOWS)
    # On Windows include the shared resources
    set(APP_RESOURCES_SRCS ${SHARED_RESOURCES_SRCS})
endif()

# Shared configuration
if(OS_WINDOWS)
    add_logical_target("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
endif()

# Target executable name
set(APP_TARGET "CefPulseApp")

if(OS_WINDOWS)
    # Merge resources into sources
    # set(APP_SRCS ${APP_SRCS} ${APP_RESOURCES_SRCS} ${SHARED_SRCS})
    # set(APP_SRCS ${APP_SRCS})

    # Create source groups for Visual Studio
    # set_app_source_groups("${APP_SRCS}")

    # Executable target
    # add_executable(${APP_TARGET} WIN32 
    #   ${APP_SRCS} 
    #   ../src/core/CefPulseMain.cpp
    # )
    add_library(${APP_TARGET} STATIC ${APP_SRCS})

    # set_app_executable_target_properties(${APP_TARGET})

    add_dependencies(${APP_TARGET} libcef_dll_wrapper)

    target_link_libraries(${APP_TARGET}
        libcef_lib
        libcef_dll_wrapper
        ${CEF_STANDARD_LIBS}
    )

    copy_files("${APP_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${APP_TARGET_OUT_DIR}")
    copy_files("${APP_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${APP_TARGET_OUT_DIR}")

    # Manually copy assets directory
    add_custom_command(
        TARGET ${APP_TARGET}
        POST_BUILD
        COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/${SHARED_ASSETS_SRCS}"
        "${APP_TARGET_OUT_DIR}/assets"
        VERBATIM
    )

    pybind11_add_module(cefpulse ../src/core/bind/Initialize.cpp)
    target_link_libraries(cefpulse PRIVATE 
        ${APP_TARGET} 
        libcef_lib
        libcef_dll_wrapper
        ${CEF_STANDARD_LIBS}
    )
    target_include_directories(cefpulse PRIVATE ..)

    copy_files("cefpulse" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "$<TARGET_FILE_DIR:cefpulse>")
    copy_files("cefpulse" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "$<TARGET_FILE_DIR:cefpulse>")
endif()
