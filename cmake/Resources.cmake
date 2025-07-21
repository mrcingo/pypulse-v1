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

if(OS_WINDOWS)
    # Shared Windows resources (version information, icons)
    set(SHARED_RESOURCES_SRCS ../resources/windows/big.ico ../resources/windows/resource.h ../resources/windows/app.rc ../resources/windows/small.ico)

    # Manifest files used for executables
    set(SHARED_EXE_MANIFEST ../resources/windows/app.exe.manifest)
    set(SHARED_COMPATIBILITY_MANIFEST ../resources/windows/compatibility.manifest)
endif()

#
# Shared configuration
#

# Support nice project/target organization in Visual Studio and Xcode
if(OS_WINDOWS)
    # Enable the creation of project folders
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Group target contents by directory
    macro(set_app_source_groups srcs)
        foreach(FILE ${srcs})
            # Get the absolute directory path
            get_filename_component(ABS_FILE "${FILE}" ABSOLUTE)
            get_filename_component(PARENT_DIR "${ABS_FILE}" DIRECTORY)

            # Remove the common directory prefix, what remains in the group
            string(REPLACE "${CMAKE_SOURCE_DIR}/app/" "" GROUP "${PARENT_DIR}")

            # Convert to Windows slashes
            string(REPLACE "/" "\\" GROUP "${GROUP}")

            source_group("${GROUP}" FILES "${FILE}")
        endforeach()
    endmacro()
endif()

# Set the configuration-specific binary output directory
if(GEN_NINJA OR GEN_MAKEFILES)
    # Force Ninja and Make to create a subdirectory named after the configuration
    set(APP_TARGET_OUT_DIR "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
else()
    set(APP_TARGET_OUT_DIR "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
endif()

# Set properties common to all example targets
macro(set_app_properties target)
    # Output all binaries to the configuration-specific build directory
    set_target_properties(
        ${target}
        PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${APP_TARGET_OUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${APP_TARGET_OUT_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${APP_TARGET_OUT_DIR}")

    if(OS_WINDOWS)
        # Place the target in the "app" folder in Visual Studio and Xcode
        set_property(TARGET ${target} PROPERTY FOLDER "app")
    endif()
endmacro()

# Set properties on an example library target
macro(set_app_library_target_properties target)
    set_library_target_properties(${target})
    set_app_properties(${target})
endmacro()

# Set properties on an example executable target
macro(set_app_executable_target_properties target)
    set_executable_target_properties(${target})
    set_app_properties(${target})

    if(OS_WINDOWS)
        # Add the custom Windows manifest files to the executable
        add_custom_command(
            TARGET ${target}
            POST_BUILD
            COMMAND
            "mt.exe" -nologo -manifest
            \"${CMAKE_CURRENT_SOURCE_DIR}/${SHARED_EXE_MANIFEST}\"
            \"${CMAKE_CURRENT_SOURCE_DIR}/${SHARED_COMPATIBILITY_MANIFEST}\"
            -outputresource:"${APP_TARGET_OUT_DIR}/${target}.exe"\;\#1
            COMMENT "Adding manifest...")
    endif()
endmacro()
