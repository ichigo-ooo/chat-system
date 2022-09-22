#pragma once
#define IM_VEC2_CLASS_EXTRA
#include "imgui.h"
#include "imgui_internal.h"

struct utility_t {

    ImVec2 mouse_position() {
        return ImGui::GetIO().MousePos;
    }

    bool mouse_in_contents(ImVec2 position, ImVec2 size) {
        auto p = mouse_position();
        return (p.x > position.x && p.y > position.y && p.x < position.x + size.x && p.y < position.y + size.y);
    }

    bool mouse_clicked() {
        return ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    }

    bool mouse_held() {
        return ImGui::IsMouseDown(ImGuiMouseButton_Left);
    }

}; inline utility_t utility;
