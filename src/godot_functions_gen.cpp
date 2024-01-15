// This file should be complitely generated


#include "godot_module.h"
#include "godot_types_gen.h"
#include "godot_functions_wrapper.h"

// builtin

float _Engine_get_frames_per_second() {
    return Engine::get_singleton()->get_frames_per_second();
}

#include "core/variant/variant_utility.h"


void Module_Godot::bind_functions(das::ModuleLibrary & lib) {
    // Object
    das::addExtern<DAS_BIND_FUN(creator<Object>)>(*this, lib, "Object`new", das::SideEffects::modifyExternal, "creator<Object>");
    // Node
    das::addExtern<DAS_BIND_FUN(creator<Node>)>(*this, lib, "Node`new", das::SideEffects::modifyExternal, "creator<Node>");
    using _Node_find_child = DAS_CALL_GODOT_MEMBER(Node::find_child);
    das::addExtern<DAS_BIND_FUN(_Node_find_child::invoke)>(*this, lib, "find_child", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Node::find_child));
    using _Node_get_name = DAS_CALL_GODOT_MEMBER(Node::get_name);
    das::addExtern<DAS_BIND_FUN(_Node_get_name::invoke)>(*this, lib, "get_name", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Node::get_name));
    using _Node_get_parent = DAS_CALL_GODOT_MEMBER(Node::get_parent);
    das::addExtern<DAS_BIND_FUN(_Node_get_parent::invoke)>(*this, lib, "get_parent", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Node::get_parent));
    using _Node_get_child = DAS_CALL_GODOT_MEMBER(Node::get_child);
    das::addExtern<DAS_BIND_FUN(_Node_get_child::invoke)>(*this, lib, "get_child", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Node::get_child));
    using _Node_get_child_count = DAS_CALL_GODOT_MEMBER(Node::get_child_count);
    das::addExtern<DAS_BIND_FUN(_Node_get_child_count::invoke)>(*this, lib, "get_child_count", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Node::get_child_count));
    using _Node_add_child = DAS_CALL_GODOT_MEMBER(Node::add_child);
    das::addExtern<DAS_BIND_FUN(_Node_add_child::invoke)>(*this, lib, "add_child", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Node::add_child));
    using _Node_get_window = DAS_CALL_GODOT_MEMBER(Node::get_window);
    das::addExtern<DAS_BIND_FUN(_Node_get_window::invoke)>(*this, lib, "get_window", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Node::get_window));
    // CanvasItem
    // note: cannot be created
    using _CanvasItem_get_global_mouse_position = DAS_CALL_GODOT_MEMBER(CanvasItem::get_global_mouse_position);
    das::addExtern<DAS_BIND_FUN(_CanvasItem_get_global_mouse_position::invoke)>(*this, lib, "get_global_mouse_position", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(CanvasItem::get_global_mouse_position));
    // Node2D
    das::addExtern<DAS_BIND_FUN(creator<Node2D>)>(*this, lib, "Node2D`new", das::SideEffects::modifyExternal, "creator<Node2D>");
    using _Node2D_rotate = DAS_CALL_GODOT_MEMBER(Node2D::rotate);
    das::addExtern<DAS_BIND_FUN(_Node2D_rotate::invoke)>(*this, lib, "rotate", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Node2D::rotate));
    using _Node2D_translate = DAS_CALL_GODOT_MEMBER(Node2D::translate);
    das::addExtern<DAS_BIND_FUN(_Node2D_translate::invoke)>(*this, lib, "translate", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Node2D::translate));
    using _Node2D_get_position = DAS_CALL_GODOT_MEMBER(Node2D::get_position);
    das::addExtern<DAS_BIND_FUN(_Node2D_get_position::invoke)>(*this, lib, "get_position", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Node2D::get_position));
    using _Node2D_set_position = DAS_CALL_GODOT_MEMBER(Node2D::set_position);
    das::addExtern<DAS_BIND_FUN(_Node2D_set_position::invoke)>(*this, lib, "set_position", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Node2D::set_position));
    // Sprite2D
    das::addExtern<DAS_BIND_FUN(creator<Sprite2D>)>(*this, lib, "Sprite2D`new", das::SideEffects::modifyExternal, "creator<Sprite2D>");
    using _Sprite2D_set_texture = DAS_CALL_GODOT_MEMBER(Sprite2D::set_texture);
    das::addExtern<DAS_BIND_FUN(_Sprite2D_set_texture::invoke)>(*this, lib, "set_texture", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Sprite2D::set_texture));
    using _Sprite2D_set_modulate = DAS_CALL_GODOT_MEMBER(Sprite2D::set_modulate);
    das::addExtern<DAS_BIND_FUN(_Sprite2D_set_modulate::invoke)>(*this, lib, "set_modulate", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Sprite2D::set_modulate));
    // Label
    using _Label_set_text = DAS_CALL_GODOT_MEMBER(Label::set_text);
    das::addExtern<DAS_BIND_FUN(_Label_set_text::invoke)>(*this, lib, "set_text", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Label::set_text));
    // Window
    using _Window_get_size = DAS_CALL_GODOT_MEMBER(Window::get_size);
    das::addExtern<DAS_BIND_FUN(_Window_get_size::invoke)>(*this, lib, "get_size", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Window::get_size));
    // Texture2D
    using _Texture2D_get_size = DAS_CALL_GODOT_MEMBER(Texture2D::get_size);
    das::addExtern<DAS_BIND_FUN(_Texture2D_get_size::invoke)>(*this, lib, "get_size", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(Texture2D::get_size));
    // InputEvent
    using _InputEvent_is_pressed = DAS_CALL_GODOT_MEMBER(InputEvent::is_pressed);
    das::addExtern<DAS_BIND_FUN(_InputEvent_is_pressed::invoke)>(*this, lib, "is_pressed", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(InputEvent::is_pressed));
    // InputEventMouseButton
    using _InputEventMouseButton_get_button_index = DAS_CALL_GODOT_MEMBER(InputEventMouseButton::get_button_index);
    das::addExtern<DAS_BIND_FUN(_InputEventMouseButton_get_button_index::invoke)>(*this, lib, "get_button_index", das::SideEffects::modifyExternal, DAS_CALL_GODOT_MEMBER_CPP(InputEventMouseButton::get_button_index));
    // Color
    // named is overloaded, hmmm, what to do?
    using _Color_named = DAS_CALL_GODOT_STATIC_MEMBER(*static_cast<Color (*)(const String &)>(&Color::named));
    das::addExtern<DAS_BIND_FUN(_Color_named::invoke), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "Color`named", das::SideEffects::modifyExternal, DAS_CALL_GODOT_STATIC_MEMBER_CPP(*static_cast<Color (*)(const String &)>(&Color::named)));

    // builtin , das::SimNode_ExtFuncCallAndCopyOrMove
    das::addExtern<DAS_BIND_FUN(_Engine_get_frames_per_second)>(*this, lib, "Engine`get_frames_per_second", das::SideEffects::modifyExternal, "_Engine_get_frames_per_second");
    das::addExtern<DAS_BIND_FUN(VariantUtilityFunctions::randf_range)>(*this, lib, "randf_range", das::SideEffects::modifyExternal, "VariantUtilityFunctions::randf_range");
}

