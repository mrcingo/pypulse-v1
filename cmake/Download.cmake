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

function(DownloadCEF platform version download_dir)
    # specify the binary distribution type and download directory
    set(CEF_DISTRIBUTION "cef_binary_${version}_${platform}")
    set(CEF_DOWNLOAD_DIR "${download_dir}")

    # the location where we expect the extracted binary distribution
    set(CEF_ROOT "${CEF_DOWNLOAD_DIR}/${CEF_DISTRIBUTION}" CACHE INTERNAL "CEF_ROOT")

    # download and/or extract the binary distribution if necessary
    if(NOT IS_DIRECTORY "${CEF_ROOT}")
        set(CEF_DOWNLOAD_FILENAME "${CEF_DISTRIBUTION}.tar.bz2")
        set(CEF_DOWNLOAD_PATH "${CEF_DOWNLOAD_DIR}/${CEF_DOWNLOAD_FILENAME}")

        if(NOT EXISTS "${CEF_DOWNLOAD_PATH}")
            set(CEF_DOWNLOAD_URL "https://cef-builds.spotifycdn.com/${CEF_DOWNLOAD_FILENAME}")
            string(REPLACE "+" "%2B" CEF_DOWNLOAD_URL_ESCAPED ${CEF_DOWNLOAD_URL})

            # download the SHA1 hash for the binary distribution
            message(STATUS "Downloading ${CEF_DOWNLOAD_PATH}.sha1 from ${CEF_DOWNLOAD_URL_ESCAPED}...")

            file(DOWNLOAD "${CEF_DOWNLOAD_URL_ESCAPED}.sha1" "${CEF_DOWNLOAD_PATH}.sha1")
            file(READ "${CEF_DOWNLOAD_PATH}.sha1" CEF_SHA1)

            # download the binary distribution and verify the hash
            message(STATUS "Downloading ${CEF_DOWNLOAD_PATH}...")

            file(
                DOWNLOAD "${CEF_DOWNLOAD_URL_ESCAPED}" "${CEF_DOWNLOAD_PATH}"
                EXPECTED_HASH SHA1=${CEF_SHA1}
                SHOW_PROGRESS
            )
        endif()

        # extract the binary distribution
        message(STATUS "Extracting ${CEF_DOWNLOAD_PATH}...")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xzf "${CEF_DOWNLOAD_DIR}/${CEF_DOWNLOAD_FILENAME}"
            WORKING_DIRECTORY ${CEF_DOWNLOAD_DIR}
        )
    endif()
endfunction()
