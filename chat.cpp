#include "chat.hpp"
#include "utility.hpp"

#include <string>
#include <map>

void drag_controller_t::call(ImVec2* pos) {
    if (utility.mouse_in_contents(*pos, ImVec2(content.size.x, 25)) && utility.mouse_clicked()) {
        menu_click_pos = *pos;
        click_pos = ImGui::GetIO().MousePos;
        active = true;
    }

    if (!utility.mouse_held()) {
        active = false;
    }

    if (active) {
        auto display_size = ImGui::GetIO().DisplaySize;
        auto new_pos = menu_click_pos + ImGui::GetIO().MousePos - click_pos;

        *pos = ImClamp(new_pos, ImVec2(10.f, 10.f), display_size - content.size - ImVec2(10.f, 10.f));
    }
}

void resize_controller_t::call() {

}

void chat_t::on_render_start(ImVec2 start_position, ImVec2 default_size) {
    if (render_start_complete) {
        return;
    }

    position = start_position;
    drag_controller.content.position = start_position;
    drag_controller.content.size = default_size;

    render_start_complete = true;
}

void chat_t::think() {
    auto display_size = ImGui::GetIO().DisplaySize;
    auto size = ImVec2(600.f, 400.f);
    auto start_position = display_size / 2 - size / 2;

    on_render_start(start_position, size);

    drag_controller.call(&position);

    auto draw_list = ImGui::GetBackgroundDrawList();

    draw_list->AddRectFilled(position, position + size, ImColor(20, 25, 30), 3.f);

    auto text_input_pos = position + ImVec2(25, size.y - 25 - 35);
    auto text_input_size = ImVec2(size.x - 50, 35);

    auto begin_typing_str = "Click to start typing...";
    auto begin_typing_size = ImGui::CalcTextSize(begin_typing_str);

    if (utility.mouse_in_contents(text_input_pos, text_input_size) && utility.mouse_clicked()) {
        globals.blocking = std::hash<std::string>()("TextInput");
    }

    if (globals.blocking == std::hash<std::string>()("TextInput") && !utility.mouse_in_contents(text_input_pos, text_input_size) && utility.mouse_clicked()) {
        globals.blocking = 0;
    }

    draw_list->AddRectFilled(text_input_pos, text_input_pos + text_input_size, ImColor(15, 20, 25), 3.f);

    if (globals.blocking == 0)
        draw_list->AddText(text_input_pos + ImVec2(text_input_size.y, text_input_size.y) / 2 - ImVec2(begin_typing_size.y, begin_typing_size.y) / 2, ImColor(100, 100, 100), begin_typing_str);
}
