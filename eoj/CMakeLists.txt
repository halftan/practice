cmake_minimum_required(VERSION 2.8)
project(eoj)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_C_FLAGS_INIT "-Wall -g -std=gnu99")

set(targets 1129 1850)

foreach(t ${targets})
    message("Building ${t}")
    add_executable("${t}" "${t}.c")
endforeach(t)
