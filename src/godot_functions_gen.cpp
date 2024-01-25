// This file is complitely generated

#include "godot_module.h"

#include "godot_types_gen.h"
#include "godot_types_extra.h"

#include "godot_functions_wrapper.h"


void Module_Godot::bind_functions_gen(das::ModuleLibrary & lib) {
    // Object
    BIND_GODOT_CTOR(Object)
    // Node
    BIND_GODOT_CTOR(Node)
    BIND_GODOT_MEMBER(Node, find_child, "pattern", "recursive", "owned")
    SET_DEFAULT_ARG(Node, find_child, 2, true)
    SET_DEFAULT_ARG(Node, find_child, 3, true)
    BIND_GODOT_MEMBER(Node, get_name)
    BIND_GODOT_MEMBER(Node, get_parent)
    BIND_GODOT_MEMBER(Node, get_child, "idx", "include_internal")
    SET_DEFAULT_ARG(Node, get_child, 2, false)
    BIND_GODOT_MEMBER(Node, get_child_count, "include_internal")
    SET_DEFAULT_ARG(Node, get_child_count, 1, false)
    BIND_GODOT_MEMBER(Node, add_child, "node", "force_readable_name", "internal")
    SET_DEFAULT_ARG(Node, add_child, 2, false)
    SET_DEFAULT_ARG(Node, add_child, 3, 0)
    BIND_GODOT_MEMBER(Node, get_window)
    BIND_GODOT_MEMBER_RENAME(Node, is_ready, is_node_ready)
    // CanvasItem
    BIND_GODOT_MEMBER(CanvasItem, get_global_mouse_position)
    BIND_GODOT_MEMBER(CanvasItem, set_modulate, "modulate")
    // Node2D
    BIND_GODOT_CTOR(Node2D)
    BIND_GODOT_MEMBER(Node2D, rotate, "radians")
    BIND_GODOT_MEMBER(Node2D, translate, "offset")
    BIND_GODOT_MEMBER(Node2D, get_position)
    BIND_GODOT_MEMBER(Node2D, set_position, "position")
    // Sprite2D
    BIND_GODOT_CTOR(Sprite2D)
    BIND_GODOT_MEMBER(Sprite2D, set_texture, "texture")
    // Label
    BIND_GODOT_CTOR(Label)
    BIND_GODOT_MEMBER(Label, set_text, "text")
    // Window
    BIND_GODOT_CTOR(Window)
    BIND_GODOT_MEMBER(Window, get_size)
    // Resource
    // InputEvent
    BIND_GODOT_MEMBER(InputEvent, is_pressed)
    // InputEventMouseButton
    BIND_GODOT_MEMBER(InputEventMouseButton, get_button_index)
    // Texture2D
    BIND_GODOT_MEMBER(Texture2D, get_size)
}
