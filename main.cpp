#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include "values.h"
#include "httplib.h"
#include "json.hpp"

class Node {
public:
    int64_t node_id;
    std::string location;
    NodeStatus status;

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
    /*
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
*/

    if (!get_node_collection()) {
        return 1;
    }


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

    for (auto node: sensor_nodes) {
        std::cout << node.node_id << std::endl;
    }
    return 0;
}

