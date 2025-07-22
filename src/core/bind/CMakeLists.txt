set(BIND_SRCS 
    CefBindInitialize.cpp
)

if(OS_WINDOWS)
    pybind11_add_module(cefpulse ${BIND_SRCS})
        
    target_link_libraries(cefpulse PRIVATE ${CMAKE_PROJECT_NAME})
    target_include_directories(cefpulse PRIVATE ../../..)

    copy_files("cefpulse" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "$<TARGET_FILE_DIR:cefpulse>")
    copy_files("cefpulse" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "$<TARGET_FILE_DIR:cefpulse>")
endif()
