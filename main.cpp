#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"
#include "values.h"
#include "httplib.h"
#include "imgui_internal.h"
#include "json.hpp"
#include "node.h"

httplib::Client cli("http://localhost:5050");
httplib::Server svr;

bool get_node_collection() {
    if (auto res = cli.Get("/node_collection")) {
        if (res->status == httplib::StatusCode::OK_200) {
            nlohmann::json j = nlohmann::json::parse(res->body);
            if (j.is_array() && !j.empty()) {
                for (const auto& item : j) {
                    Node n;
                    n.node_id= item["id"].get<int64_t>();
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

bool get_node_relationship_collection() {
    if (auto res = cli.Get("/node_relation_collection")) {
        if (res->status == httplib::StatusCode::OK_200) {
            nlohmann::json j = nlohmann::json::parse(res->body);
            if (j.is_array() && !j.empty()) {
                for (const auto& item : j) {
                    NodeRelations n;
                    n.relation_id = item["id"].get<int64_t>();
                    n.child_node = findNode(item["node_id"].get<int64_t>());
                    n.parent_node = findNode(item["parent_node_id"].get<int64_t>());

                    sensor_nodes_relationship.push_back(n);
                }
                return true;
            }
            else {
                std::cout << "Empty or invalid response array!" << std::endl;
            }
        }
        else {
            std::cout << "Couldn't get node relationship collection! Status: " << res->status <<std::endl;
        }
    }
    return false;
}

void ping(Node* node) {
    nlohmann::json json_payload;
    json_payload["id"] = node->node_id;
    json_payload["location"] = node->location;

    if (auto res = cli.Post("/ping", json_payload.dump(), "application/json")) {
        if (res->status == 200) {
            std::cout << "Node " << node->node_id << " boink (Status 200)" << std::endl;
        } else {
            std::cout << "Ping failed status: " << res->status << std::endl;
        }
    } else {
        std::cout << "Ping execution network error" << std::endl;
    }
}

void handle_relational_kill(Node* inactiveNode) {
    for (auto& node : sensor_nodes_relationship) {
        if (inactiveNode->node_id == node.parent_node->node_id) {
            for (auto& singular_node : sensor_nodes) {
                if (singular_node.node_id == node.child_node->node_id) {
                    singular_node.status = INACTIVE;
                }
            }
        }
    }
}

int main() {
    InitWindow(1280, 720, "PGM Node Simulator");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    bool fetched_node_collection = get_node_collection();
    int selected_node_id = -1;
    Node selected_node;

    // timer
    auto last_ping_time = std::chrono::steady_clock::now();

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
                } else {
                    colo = 1;
                }
            }
            ImGui::EndGroup();

            // Check if 3 seconds have passed before looping through active elements
            auto current_time = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(current_time - last_ping_time).count() >= 3) {

                for (auto& node : sensor_nodes) {
                    if (node.status == ACTIVE) {
                        ping(&node);
                    }
                }
                last_ping_time = current_time;
            }

            ImGui::SameLine();
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
            ImGui::SameLine();

            if (selected_node_id != -1) {
                ImGui::BeginChild("NodeDetailsPanel", ImVec2(300, 250), ImGuiChildFlags_Borders);
                ImGui::Text("--- Node Details ---");

                std::string id_text = "ID: " + std::to_string(selected_node.node_id);
                ImGui::Text("%s", id_text.c_str());
                ImGui::Text("Location: %s", selected_node.location.c_str());

                ImGui::Text("Change Status:");
                if (ImGui::RadioButton("Active", selected_node.status == ACTIVE)) { selected_node.status = ACTIVE; }
                if (ImGui::RadioButton("Inactive", selected_node.status == INACTIVE)) {
                    selected_node.status = INACTIVE;
                    handle_relational_kill(&selected_node);
                }
                /* if (ImGui::RadioButton("Being Maintained", selected_node.status == BEING_MAINTAINED)) {
                    selected_node.status = BEING_MAINTAINED;
                    notify_being_maintained_status(&selected_node);
                }*/

                // Synchronize selection state edits back down into our array cache container
                for (auto& node : sensor_nodes) {
                    if (node.node_id == selected_node_id) {
                        node.status = selected_node.status;
                        break;
                    }
                }

                if (ImGui::Button("Close Details")) { selected_node_id = -1; }
                ImGui::EndChild();
            }
        }
        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
