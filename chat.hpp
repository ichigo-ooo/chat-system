#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#define IM_VEC2_CLASS_EXTRA
#include "imgui.h"
#include "imgui_internal.h"
#include <string>
#include <vector>

struct globals_t {
    size_t blocking = 0;
    bool skip = false;
}; inline globals_t globals;

struct drag_controller_t {
    bool active = false;
    ImVec2 v = { 0.f, 0.f };
    ImVec2 menu_click_pos = { 0.f, 0.f };
    ImVec2 click_pos = { 0.f, 0.f };

    struct {
        ImVec2 position;
        ImVec2 size;
    } content;

    void call(ImVec2* position);
};

struct resize_controller_t {
    bool active = false;
    bool setup = false;
    ImVec2 v = { 0.f, 0.f };
    ImVec2 v_on_startup = v;

    void call();
};

struct user_message_t {
    std::string sender;
    std::string message;
};

struct chat_t {

    ImVec2 position;
    ImVec2 size = { 0.f, 0.f };

    drag_controller_t drag_controller;

    resize_controller_t resize_controller;

    // our main variables
    std::string username;
    std::string message_to_send;

    // backend variables
    float can_send_message_inerp = 0.f;
    float button_hover_a = 0.f;

    // Set our variable n' what not
    bool render_start_complete = false;
    void on_render_start(std::string client_username, ImVec2 start_position, ImVec2 default_size);

    // Main draw function
    void think(std::vector<user_message_t>& messages);
};
