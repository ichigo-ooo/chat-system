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

inline const char* const letters[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, ("0"), ("1"), ("2"), ("3"), ("4"), ("5"), ("6"), ("7"), ("8"), ("9"), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, ("a"), ("b"), ("c"), ("d"), ("e"), ("f"), ("g"), ("h"), ("i"), ("j"), ("k"), ("l"), ("m"), ("n"), ("o"), ("p"), ("q"), ("r"), ("s"),
("t"), ("u"), ("v"), ("w"), ("x"),
("y"), ("z"), nullptr, nullptr, nullptr, nullptr, nullptr, ("0"), ("1"), ("2"), ("3"), ("4"), ("5"), ("6"),
("7"), ("8"), ("9"), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, (";"), ("+"), (")"), ("-"), ("."), ("/?"), ("~"), nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, ("["), ("\\"), ("]"), ("'"), nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
