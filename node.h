//
// Created by zero on 8/8/26.
//

#ifndef POWER_GM_SENSOR_NODE_H
#define POWER_GM_SENSOR_NODE_H

#include <string>
#include <vector>

#include "values.h"



class Node {
public:
    int64_t node_id;
    std::string location;
    NodeStatus status;

    bool static draw_node(int64_t  node_id, NodeStatus status);
};

class NodeRelations {
public:
    int64_t relation_id;
    Node* child_node = nullptr;
    Node* parent_node = nullptr;
};

NodeStatus resolve_status(const std::string& response_status);
Node* findNode(int64_t target_node);

inline std::vector<Node> sensor_nodes;
inline std::vector<NodeRelations> sensor_nodes_relationship;
#endif //POWER_GM_SENSOR_NODE_H
