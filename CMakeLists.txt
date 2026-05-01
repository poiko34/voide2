cmake_minimum_required(VERSION 3.10)
project(voide_editor VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES 
    src/main.cpp 
    src/terminal_manager.cpp 
    src/keycode.cpp 
    src/init.cpp
    src/document.cpp
)

include_directories(src)

add_executable(voide ${SOURCES})

if(WIN32)
    target_link_options(voide PRIVATE -static -static-libgcc -static-libstdc++)
    target_compile_definitions(voide PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

if(MSVC)
    target_compile_options(voide PRIVATE /W4)
else()
    target_compile_options(voide PRIVATE -Wall -Wextra)
endif()
