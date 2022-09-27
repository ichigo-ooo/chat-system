#include "chat.hpp"
#include "utility.hpp"

#include <string>
#include <map>
#include <Windows.h>
#include <WinUser.h>
#include <winuser.rh>
#include <algorithm>

std::string to_upper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return toupper(c); });
    return s;
}

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

void chat_t::on_render_start(std::string client_username, ImVec2 start_position, ImVec2 default_size) {
    if (render_start_complete) {
        return;
    }

    username = client_username;
    message_to_send = "";
    position = start_position;
    drag_controller.content.position = start_position;
    drag_controller.content.size = default_size;

    render_start_complete = true;
}

void chat_t::think(std::vector<user_message_t>& messages) {
    auto display_size = ImGui::GetIO().DisplaySize;
    auto size = ImVec2(600.f, 400.f);
    auto start_position = display_size / 2 - size / 2;

    drag_controller.call(&position);

    auto hash = std::hash<std::string>()(username + "TextInput");

    auto draw_list = ImGui::GetBackgroundDrawList();

    draw_list->AddRectFilled(position, position + size, ImColor(20, 25, 30), 3.f);

    auto is_typing = globals.blocking == hash || !message_to_send.empty() ? true : false;
    if (can_send_message_inerp != is_typing) {
        can_send_message_inerp += (is_typing - can_send_message_inerp) * 0.2f;
    }

    auto text_input_pos = position + ImVec2(25, size.y - 25 - 35);
    auto text_input_size = ImVec2(size.x - 50 - (125 * can_send_message_inerp), 35);

    auto begin_typing_str = "Click to start typing...";
    auto begin_typing_size = ImGui::CalcTextSize(begin_typing_str);

    if (utility.mouse_in_contents(text_input_pos, text_input_size) && utility.mouse_clicked()) {
        globals.blocking = hash;
        globals.skip = true;
    }

    if (globals.blocking == hash && !globals.skip && !utility.mouse_in_contents(text_input_pos, text_input_size) && utility.mouse_clicked()) {
        globals.blocking = 0;
    }

    if (globals.blocking == hash) {
        auto str = message_to_send;
        for (size_t i = 0; i <= 256; i++) {
            if (ImGui::IsKeyPressed(i)) {
                if (i == VK_ESCAPE || i == VK_DELETE) {
                    globals.blocking = 0;
                    continue;
                }

                if (i == VK_RETURN && !message_to_send.empty()) {
                    messages.push_back({ username, message_to_send });
                    str.clear();
                    continue;
                }

                if (i == VK_SPACE) {
                    str.append(" ");
                    continue;
                }

                if (i == VK_BACK) {
                    str = str.substr(0, strlen(str.data()) - 1);
                    continue;
                }

                if (letters[i] != nullptr) {
                    if (ImGui::IsKeyDown(VK_SHIFT)) {
                        str.append(to_upper(letters[i]));
                    }
                    else {
                        str.append(letters[i]);
                    }
                }
            }
        }
        message_to_send = str;
    }

    draw_list->AddRectFilled(text_input_pos, text_input_pos + text_input_size, ImColor(15, 20, 25), 3.f);

    auto button_pos = position + ImVec2(size.x - 25 - (100 * can_send_message_inerp), size.y - 25 - 35);
    auto button_size = ImVec2(100 * can_send_message_inerp, 35);
   
    bool send_button_hovered = utility.mouse_in_contents(button_pos, button_size);

    button_hover_a = ImClamp(button_hover_a + (3.f * (1.f / 0.5f * ImGui::GetIO().DeltaTime) * (send_button_hovered && !utility.mouse_held() && !message_to_send.empty() ? 1.f : -1.f)), 0.0f, 1.f);

    draw_list->AddRectFilled(button_pos, button_pos + button_size, ImColor(65, 69, 88, int(can_send_message_inerp * 255)), 3.f);
    draw_list->AddRectFilled(button_pos, button_pos + button_size, ImColor(256, 44, 164, int(can_send_message_inerp * button_hover_a * 255)), 3.f);

    auto send_size = ImGui::CalcTextSize("Send message");

    draw_list->PushClipRect(button_pos, button_pos + button_size);
    draw_list->AddText(button_pos + button_size / 2 - send_size / 2, ImColor(215, 215, 215, int(can_send_message_inerp * 255)), "Send message");
    draw_list->PopClipRect();

    if (send_button_hovered && utility.mouse_clicked() && !message_to_send.empty()) {
        messages.push_back({ username, message_to_send });
        message_to_send.clear();

       //messages.push_back({ "Other user", "This is a crazy message that was automated." });
    }

    if (globals.blocking != hash && message_to_send.empty()) {
        draw_list->AddText(text_input_pos + ImVec2(text_input_size.y, text_input_size.y) / 2 - ImVec2(begin_typing_size.y, begin_typing_size.y) / 2,
            ImColor(100, 100, 100), begin_typing_str);
    }

    draw_list->AddText(text_input_pos + ImVec2(text_input_size.y, text_input_size.y) / 2 - ImVec2(begin_typing_size.y, begin_typing_size.y) / 2,
        ImColor(215, 215, 215), message_to_send.data());

    auto v = messages;
    std::reverse(v.begin(), v.end());
    int offset = 0;

    draw_list->PushClipRect(position, position + size);

    for (size_t i = 0; i < messages.size(); i++) {
        auto invoke_message = [&](user_message_t message) -> void {
            bool is_self = message.sender == username;

            auto pos = position + ImVec2(25, size.y - 120 - offset);
            auto it_size = ImGui::CalcTextSize(message.message.data()) + ImVec2(6, 8 + 12);

            it_size.x = ImClamp(it_size.x, ImGui::CalcTextSize(message.sender.data()).x + 6, size.x - 50);
          
            if (is_self) {
                pos.x += size.x - 50 - it_size.x;
            }
            auto sender_pos = pos + ImVec2(3, 2);

            if (is_self) {
                sender_pos.x = pos.x + it_size.x - 3 - ImGui::CalcTextSize(message.sender.data()).x;
            }

            draw_list->AddRectFilled(pos, pos + it_size, ImColor(55, 59, 79), 3.f);
            draw_list->AddText(sender_pos, ImColor(215, 215, 215), message.sender.data());
            draw_list->AddText(pos + ImVec2(3, 2 + 16), ImColor(180, 180, 180), message.message.data());

            offset += it_size.y + 10;
        };
        auto current_message = v.at(i);
        invoke_message(current_message);
    }

    draw_list->PopClipRect();

    draw_list->AddRectFilledMultiColor(position + ImVec2(25, 0), position + ImVec2(size.x - 25, 80), ImColor(20, 25, 30), ImColor(20, 25, 30), ImColor(20, 25, 30, 0), ImColor(20, 25, 30, 0));
}
