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
        "is_node_ready"
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

struct less_c_str {
    bool operator()(const char* a, const char* b) const {
        return strcmp(a, b) < 0;
    }
};

static std::map<const char*, std::map<const char*, const char*>, less_c_str> exceptions = {
    {"Node", {{"is_node_ready", "is_ready"}}},
};

#endif // TYPES_AND_FUNCS_H