cmake_minimum_required(VERSION 3.27)
set(PROJECT_NAME Chonk)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if(WIN32)
    add_compile_definitions(CHK_WINDOWS)
elseif(UNIX)
    add_compile_definitions(CHK_UNIX)
endif()


set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Choose a build type (Debug | Release)" FORCE)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose a build type (Debug | Release)" FORCE)
endif()

foreach(CONFIG Debug Release Exprot)
    string(TOUPPER ${CONFIG} CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER}
        "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER}
        "${CMAKE_SOURCE_DIR}/bin-lib/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER}
        "${CMAKE_SOURCE_DIR}/bin-objs/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}")
endforeach()

set(CMAKE_BUILD_RPATH)

add_subdirectory(./submodules/sdl)

add_library(glad STATIC submodules/glad/src/glad.c)
target_include_directories(glad PUBLIC submodules/glad/include)
target_compile_options(glad PUBLIC -w -Wno-language-extension-token)

add_library(imgui STATIC
    submodules/imgui/imconfig.h
    submodules/imgui/imgui.h
    submodules/imgui/imgui.cpp
    submodules/imgui/imgui_draw.cpp
    submodules/imgui/imgui_internal.h
    submodules/imgui/imgui_tables.cpp
    submodules/imgui/imgui_widgets.cpp
    submodules/imgui/imstb_rectpack.h
    submodules/imgui/imstb_textedit.h
    submodules/imgui/imstb_truetype.h
    submodules/imgui/imgui_demo.cpp

    submodules/imgui/backends/imgui_impl_sdl3.cpp
    submodules/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    submodules/imgui
    submodules/sdl/include
    submodules/glad/include
)

file(GLOB_RECURSE MAIN_SOURCES "./src/*.cpp")
file(GLOB_RECURSE STB_SOURCES "submodules/stb/*.cpp")

add_executable(${PROJECT_NAME} ${MAIN_SOURCES} ${STB_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    submodules/glm
    submodules/imgui
    submodules/sdl/include
    submodules/stb

    ./src
    ./src/include
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>: -g -O0 -Wall -Wextra -Wpedantic -Wno-unused-parameter>
        $<$<CONFIG:Release>: -O2 -Wall -Wextra -Wpedantic -Wno-unused-parameter>
    )
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>: /Zi /Od /W4>
        $<$<CONFIG:Release>: /O2 /W4>
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 imgui glad)

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:CHK_DEBUG>)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Release>:CHK_RELEASE>)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Export>:CHK_EXPORT>)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER}}/assets"
)