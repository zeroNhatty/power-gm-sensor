cmake_minimum_required(VERSION 3.25)
project(power_gm_sensor CXX)

set(CMAKE_CXX_STANDARD 20)

# diable raylib test builds
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/raylib-6.0)

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui")
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
)

set(RLIMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/rlImGui")
set(RLIMGUI_SOURCES
        ${RLIMGUI_DIR}/rlImGui.cpp
        values.h
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
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)