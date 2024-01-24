// This file should be complitely generated


#include "godot_module.h"

#include "godot_types_gen.h"
#include "godot_types_extra.h"

#include "godot_functions_wrapper.h"


void Module_Godot::bind_functions_gen(das::ModuleLibrary & lib) {
    // Object
    BIND_GODOT_CTOR(Object)
    // Node
    BIND_GODOT_CTOR(Node)
    //BIND_GODOT_MEMBER(Node, find_child)
    BIND_GODOT_MEMBER(Node, get_name)
    BIND_GODOT_MEMBER(Node, get_parent)
    BIND_GODOT_MEMBER(Node, get_child)
    BIND_GODOT_MEMBER(Node, get_child_count)
    //BIND_GODOT_MEMBER(Node, add_child)
    BIND_GODOT_MEMBER(Node, get_window)
    // CanvasItem
    // note: cannot be created
    BIND_GODOT_MEMBER(CanvasItem, get_global_mouse_position)
    // Node2D
    BIND_GODOT_CTOR(Node2D)
    BIND_GODOT_MEMBER(Node2D, rotate)
    BIND_GODOT_MEMBER(Node2D, translate)
    BIND_GODOT_MEMBER(Node2D, get_position)
    BIND_GODOT_MEMBER(Node2D, set_position)
    // Sprite2D
    BIND_GODOT_CTOR(Sprite2D)
    BIND_GODOT_MEMBER(Sprite2D, set_texture)
    BIND_GODOT_MEMBER(Sprite2D, set_modulate)
    // Label
    BIND_GODOT_CTOR(Label)
    BIND_GODOT_MEMBER(Label, set_text)
    // Window
    BIND_GODOT_CTOR(Window)
    BIND_GODOT_MEMBER(Window, get_size)
    // Texture2D
    // TODO ctor for RefCountet
    BIND_GODOT_MEMBER(Texture2D, get_size)
    // InputEvent
    BIND_GODOT_MEMBER(InputEvent, is_pressed)
    // InputEventMouseButton
    BIND_GODOT_MEMBER(InputEventMouseButton, get_button_index)

}
