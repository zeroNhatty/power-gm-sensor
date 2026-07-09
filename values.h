//
// Created by zero on 7/9/26.
//

#ifndef POWER_GM_SENSOR_VALUES_H
#define POWER_GM_SENSOR_VALUES_H

#include <imgui.h>

constexpr ImColor COLOR_NODE_BEING_MAINTAINED = IM_COL32(200, 180, 0, 255);
constexpr ImColor COLOR_NODE_ACTIVE = IM_COL32(0, 200, 50, 255);
constexpr ImColor COLOR_NODE_INACTIVE = IM_COL32(200, 50, 50, 255);

enum NodeStatus {
    ACTIVE,
    INACTIVE,
    BEING_MAINTAINED,
};
#endif //POWER_GM_SENSOR_VALUES_H
