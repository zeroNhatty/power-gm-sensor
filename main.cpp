#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include "values.h"

class Nodes {
public:
    void static draw_node(NodeStatus status) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        ImVec2 p = ImGui::GetCursorScreenPos();

        ImColor color;
        switch (status) {
            case ACTIVE:
                color = COLOR_NODE_ACTIVE;
                break;
            case INACTIVE:
                color = COLOR_NODE_INACTIVE;
                break;
            case BEING_MAINTAINED:
                color = COLOR_NODE_BEING_MAINTAINED;
                break;
        }

        draw_list->AddCircleFilled(ImVec2(p.x + 50, p.y + 50), 30.0f, color);


        ImGui::Dummy(ImVec2(200, 200));
    }
};

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
        ImGui::Button("test");
        Nodes::draw_node(ACTIVE);
        Nodes::draw_node(INACTIVE);
        Nodes::draw_node(BEING_MAINTAINED);
        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}

