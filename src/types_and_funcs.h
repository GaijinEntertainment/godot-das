#ifndef TYPES_AND_FUNCS_H
#define TYPES_AND_FUNCS_H

#include <map>
#include <vector>


// TODO generate these automatically? How?
// all these types should inherit from Object
static std::map<const char*, std::vector<const char*>> types = {
    {"Node", {
        "find_child",
        "get_name",
        "get_parent",
        "get_child",
        "get_child_count",
        "add_child",
        "get_window",
    }},
        {"CanvasItem", {
            "get_global_mouse_position",
            "set_modulate",
        }},
            {"Node2D", {
                "rotate",
                "translate",
                "get_position",
                "set_position",
            }},
                {"Sprite2D", {
                    "set_texture",
                }},
            {"Label", {
                "set_text",
            }},
            {"Window", {
                "get_size",
            }},
    {"Resource", {
        // TODO
    }},
        {"InputEvent", {
            "is_pressed",
        }},
            {"InputEventMouseButton", {
                "get_button_index",
            }},
        {"Texture2D", {
            "get_size",
        }}
};

#endif // TYPES_AND_FUNCS_H