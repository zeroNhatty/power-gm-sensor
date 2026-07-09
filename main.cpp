#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

int main() {
    InitWindow(1280, 720, "Power Grid Monitor Node Simulator");
    SetTargetFPS(60);

    rlImGuiSetup(true);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        rlImGuiBegin();

        ImGui::Begin("Sensor Status Dashboard");
        ImGui::Text("Testing worked");
        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}