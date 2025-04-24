cmake_minimum_required(VERSION 3.27)
set(PROJECT_NAME Curvy)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if(WIN32)
    add_compile_definitions(CV_WINDOWS)
elseif(UNIX)
    add_compile_definitions(CV_UNIX)
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
    submodules/glm/glm
    submodules/imgui
    submodules/sdl/include
    submodules/stb

    ./src
    ./src/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 imgui glad)

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:CV_DEBUG>)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Release>:CV_RELEASE>)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Export>:CV_EXPORT>)