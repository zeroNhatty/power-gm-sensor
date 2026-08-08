cmake_minimum_required(VERSION 3.25)
project(power_gm_sensor CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads REQUIRED)

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(RAYLIB_DIR "${LIB_DIR}/raylib-6.0_linux_amd64")

set(IMGUI_DIR "${LIB_DIR}/imgui")
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
)

set(RLIMGUI_DIR "${LIB_DIR}/rlImGui")
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
        ${RAYLIB_DIR}/include
        ${IMGUI_DIR}
        ${RLIMGUI_DIR}
        ${LIB_DIR}
)

find_library(PREBUILT_RAYLIB raylib HINTS "${RAYLIB_DIR}/lib" NO_DEFAULT_PATH)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${PREBUILT_RAYLIB}
        Threads::Threads
        m
        dl
)