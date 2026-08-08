//
// Created by zero on 8/8/26.
//

#include "node.h"

bool Node::draw_node(int64_t  node_id, NodeStatus status) {
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
}\

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

Node* findNode(int64_t target_node) {
    for (auto& node: sensor_nodes) {
        if (target_node == node.node_id)
            return &node;
    }
    return nullptr;
}
