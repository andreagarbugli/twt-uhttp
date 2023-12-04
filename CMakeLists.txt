cmake_minimum_required(VERSION 3.12)
project(uHTTPServer VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

# set output directory, if window to bin/win32, else to bin/linux
if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/win32)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/linux)
endif()

# 1st Example: a simple http server that response with a simple text
add_executable(uhttp_simple src/simple_resp.c)

# 2nd Example: a simple http server that response with a simple html page
add_executable(uhttp_page src/simple_page.c)
