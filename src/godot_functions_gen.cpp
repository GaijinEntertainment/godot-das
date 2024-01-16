// This file should be complitely generated


#include "godot_module.h"
#include "godot_types_gen.h"
#include "godot_functions_wrapper.h"

#include "core/variant/variant_utility.h"
#include "core/config/engine.h"


void Module_Godot::bind_functions(das::ModuleLibrary & lib) {
    // Object
    BIND_GODOT_CTOR(Object)
    // Node
    BIND_GODOT_CTOR(Node)
    BIND_GODOT_MEMBER(Node, find_child)
    BIND_GODOT_MEMBER(Node, get_name)
    BIND_GODOT_MEMBER(Node, get_parent)
    BIND_GODOT_MEMBER(Node, get_child)
    BIND_GODOT_MEMBER(Node, get_child_count)
    BIND_GODOT_MEMBER(Node, add_child)
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

    BIND_GODOT_SINGLETON_MEMBER(Engine, get_frames_per_second)
    BIND_GODOT_BUILTIN_FUNCTION(VariantUtilityFunctions, randf_range)

    // NOT GENERTATABLE

    // Color
    // TODO properly bind simnode ctor
    using _Color_named = DAS_CALL_GODOT_STATIC_MEMBER(*static_cast<Color (*)(const String &)>(&Color::named));
    das::addExtern<DAS_BIND_FUN(_Color_named::invoke), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "Color`named", das::SideEffects::modifyExternal, DAS_CALL_GODOT_STATIC_MEMBER_CPP(Color::named));
}
