cmake_minimum_required(VERSION 3.25)
project(power_gm_sensor CXX)

set(CMAKE_CXX_STANDARD 20)

# Disable raylib test builds
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/raylib-6.0)

find_package(Threads REQUIRED)

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui")
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
)

set(RLIMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/rlImGui")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(RLIMGUI_SOURCES
        ${RLIMGUI_DIR}/rlImGui.cpp
        values.h
        ${LIB_DIR}/httplib.h
        ${LIB_DIR}/json.hpp
)

add_executable(${PROJECT_NAME}
        main.cpp
        ${IMGUI_SOURCES}
        ${RLIMGUI_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${IMGUI_DIR}
        ${RLIMGUI_DIR}
        ${LIB_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        raylib
        Threads::Threads
)