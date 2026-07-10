#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include "values.h"
#include "httplib.h"
#include "imgui_internal.h"
#include "json.hpp"

class Node {
public:
    int64_t node_id;
    std::string location;
    NodeStatus status;

    bool static draw_node(int64_t  node_id, NodeStatus status) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        ImVec2 p = ImGui::GetCursorScreenPos();

        bool is_clicked = false;

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
        std::string button_id = "node_click_" + std::to_string(node_id);

        if (ImGui::InvisibleButton(button_id.c_str(), ImVec2(100, 100))) {
            is_clicked = true;
        }

        if (ImGui::IsItemHovered()) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            draw_list->AddCircle(ImVec2(p.x + 50, p.y + 50), 34.0f, ImColor(255, 255, 255, 150), 0, 2.0f);
        }

        return is_clicked;
    }
};

class NodeRelations {

};

std::vector<Node> sensor_nodes;
httplib::Client cli("http://localhost:5050");


NodeStatus resolve_status(const std::string& response_status) {
    if (response_status == "active") {
        return ACTIVE;
    }
    if (response_status == "inactive") {
        return INACTIVE;
    }
    if (response_status == "being_maintained") {
        return BEING_MAINTAINED;
    }
    return INACTIVE;
}

bool get_node_collection() {
    if (auto res = cli.Get("/node_collection")) {
        if (res->status == 200) {
            nlohmann::json j = nlohmann::json::parse(res->body);
            if (j.is_array() && !j.empty()) {
                for (const auto& item : j) {
                    Node n;
                    n.node_id  = item["id"].get<int64_t>();
                    n.location = item["location"].get<std::string>();
                    n.status   = resolve_status(item["status"].get<std::string>());

                    sensor_nodes.push_back(n);
                }
                return true;
            }
            else {
                std::cout << "Empty or invalid response array!" << std::endl;
            }
        }
        else {
            std::cout << "Couldn't get node collection! Status: " << res->status <<std::endl;
        }
    }
    return false;
}


int main() {
    InitWindow(1280, 720, "PGM Node Simulator");
    SetTargetFPS(60);

    rlImGuiSetup(true);

    bool fetched_node_collection = get_node_collection();
    int selected_node_id = -1;
    Node selected_node;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        rlImGuiBegin();

        ImGui::Begin("Sensor Status Dashboard");
        if (!fetched_node_collection) {
            ImGui::TextColored(COLOR_NODE_INACTIVE, "Couldn't Retrieve Nodes!");
        }
        else {
            ImGui::BeginGroup();
            int colo = 1;
            for (const auto& node : sensor_nodes) {
                if (Node::draw_node(node.node_id, node.status)) {
                    selected_node_id = node.node_id;
                    selected_node = node;
                }
                if (colo < 5) {
                    ImGui::SameLine();
                    colo++;
                }
                else
                    colo = 1;
            }
            ImGui::EndGroup();

            ImGui::SameLine();
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
            ImGui::SameLine();

            if (selected_node_id != -1) {
                ImGui::BeginChild("NodeDetailsPanel", ImVec2(300, 200));

                ImGui::Text("--- Node Details ---");

                std::string id_text = "ID: " + std::to_string(selected_node.node_id);
                ImGui::Text("%s", id_text.c_str());

                ImGui::Text("Location: %s", selected_node.location.c_str());;

                ImGui::Text("Change Status:");

                if (ImGui::RadioButton("Active", selected_node.status == ACTIVE)) {
                    selected_node.status = ACTIVE;
                }
                if (ImGui::RadioButton("Inactive", selected_node.status == INACTIVE)) {
                    selected_node.status = INACTIVE;
                }
                if (ImGui::RadioButton("Being Maintained", selected_node.status == BEING_MAINTAINED)) {
                    selected_node.status = BEING_MAINTAINED;
                }

                for (auto& node : sensor_nodes) {
                    if (node.node_id == selected_node_id) {
                        node.status = selected_node.status;
                        break;
                    }
                }
                if (ImGui::Button("Close Details")) {
                    selected_node_id = -1;
                }

                ImGui::EndChild();
            }
        }
        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    std::string json_data = R"({"id":42393,"location":"Musk"})";

    if (auto res = cli.Post("/decode", json_data, "application/json")) {
        if (res->status == 200) {
            std::cout << "Response: " << res->body << std::endl;
        } else {
            std::cout << "HTTP Error: " << res->status << std::endl;
        }
    } else {
        std::cout << "Connection Error: " << res.error() << std::endl;
    }

    return 0;
}

