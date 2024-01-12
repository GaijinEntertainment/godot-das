// This file should be complitely generated


#include "godot_module.h"
#include "godot_types_gen.h"
#include "godot_functions_macro.h"

// Node

Node* _Node_find_child(const Node* node, const char* p_pattern, CTX_AT) {
    CHECK_IF_NULL(node)
    // TODO add p_recursive and p_owned
    return node->find_child(p_pattern, true, true);
}

Node* _Node_get_parent(const Node* node, CTX_AT) {
    CHECK_IF_NULL(node)
    return node->get_parent();
}

const char* _Node_get_name(const Node* node, CTX_AT) {
    CHECK_IF_NULL(node)
    // TODO bind StringName?
    return ctx->stringHeap->allocateString(String(node->get_name()).utf8().get_data());
}

Node* _Node_get_child(const Node* node, int p_index, CTX_AT) {
    CHECK_IF_NULL(node)
    // TODO add p_include_internal
    return node->get_child(p_index, false);
}

int _Node_get_child_count(const Node* node, CTX_AT) {
    CHECK_IF_NULL(node)
    // TODO add p_include_internal
    return node->get_child_count(false);
}

void _Node_add_child(Node* node, Node* p_child, CTX_AT) {
    CHECK_IF_NULL_VOID(node)
    node->add_child(p_child);
}

Window* _Node_get_window(const Node* node, CTX_AT) {
    CHECK_IF_NULL(node)
    return node->get_window();
}

// CanvasItem

Vector2 _CanvasItem_get_global_mouse_position(CanvasItem* canvas_item, CTX_AT) {
    CHECK_IF_NULL(canvas_item)
    return canvas_item->get_global_mouse_position();
}

// Note2D

void _Node2D_rotate(Node2D* node2d, float p_radians, CTX_AT) {
    CHECK_IF_NULL_VOID(node2d)
    node2d->rotate(p_radians);
}

void _Node2D_translate(Node2D* node2d, const Vector2 p_amount, CTX_AT) {
    CHECK_IF_NULL_VOID(node2d)
    node2d->translate(p_amount);
}

void _Node2D_set_position(Node2D* node2d, const Vector2 p_pos, CTX_AT) {
    CHECK_IF_NULL_VOID(node2d)
    node2d->set_position(p_pos);
}

Vector2 _Node2D_get_position(const Node2D* node2d, CTX_AT) {
    CHECK_IF_NULL(node2d)
    return node2d->get_position();
}

// InputEvent

bool _InputEvent_is_pressed(const InputEvent* event, CTX_AT) {
    CHECK_IF_NULL(event)
    return event->is_pressed();
}

// InputEventMouseButton

MouseButton _InputEventMouseButton_get_button_index(const InputEventMouseButton* event, CTX_AT) {
    CHECK_IF_NULL(event)
    return event->get_button_index();
}

// Sprite2D

Sprite2D* _Sprite2D_new() {
    return memnew(Sprite2D);
}

void _Sprite2D_set_texture(Sprite2D* sprite, Texture2D* texture, CTX_AT) {
    CHECK_IF_NULL_VOID(sprite)
    sprite->set_texture(texture);
}

void _Sprite2D_set_modulate(Sprite2D* sprite, const Color& modulate, CTX_AT) {
    CHECK_IF_NULL_VOID(sprite)
    sprite->set_modulate(modulate);
}

// Texture2D

Vector2 _Texture2D_get_size(Texture2D* texture, CTX_AT) {
    CHECK_IF_NULL(texture)
    return texture->get_size();
}

// Window

Vector2 _Window_get_size(Window* window, CTX_AT) {
    CHECK_IF_NULL(window)
    return Vector2{window->get_size()}; // returning Vector2 instead of Vector2i
}

// Label

void _Label_set_text(Label* label, const char* text, CTX_AT) {
    CHECK_IF_NULL_VOID(label)
    label->set_text(text);
}

// Color

Color _named_color(const char* name, CTX_AT) {
    return Color::named(StringName(name));
}

// builtin

float _Engine_get_frames_per_second() {
    return Engine::get_singleton()->get_frames_per_second();
}

#include "core/variant/variant_utility.h"


void Module_Godot::bind_functions(das::ModuleLibrary & lib) {
    // Node
    das::addExtern<DAS_BIND_FUN(_Node_get_parent)>(*this, lib, "get_parent", das::SideEffects::modifyExternal, "_Node_get_parent");
    das::addExtern<DAS_BIND_FUN(_Node_find_child)>(*this, lib, "find_child", das::SideEffects::modifyExternal, "_Node_find_child");
    das::addExtern<DAS_BIND_FUN(_Node_get_name)>(*this, lib, "get_name", das::SideEffects::modifyExternal, "_Node_get_name");
    das::addExtern<DAS_BIND_FUN(_Node_get_child)>(*this, lib, "get_child", das::SideEffects::modifyExternal, "_Node_get_child");
    das::addExtern<DAS_BIND_FUN(_Node_get_child_count)>(*this, lib, "get_child_count", das::SideEffects::modifyExternal, "_Node_get_child_count");
    das::addExtern<DAS_BIND_FUN(_Node_add_child)>(*this, lib, "add_child", das::SideEffects::modifyExternal, "_Node_add_child");
    das::addExtern<DAS_BIND_FUN(_Node_get_window)>(*this, lib, "get_window", das::SideEffects::modifyExternal, "_Node_get_window");
    // CanvasItem
    das::addExtern<DAS_BIND_FUN(_CanvasItem_get_global_mouse_position)>(*this, lib, "get_global_mouse_position", das::SideEffects::modifyExternal, "_CanvasItem_get_global_mouse_position");
    // Node2D
    das::addExtern<DAS_BIND_FUN(_Node2D_rotate)>(*this, lib, "rotate", das::SideEffects::modifyExternal, "_Node2D_rotate");
    das::addExtern<DAS_BIND_FUN(_Node2D_translate)>(*this, lib, "translate", das::SideEffects::modifyExternal, "_Node2D_translate");
    das::addExtern<DAS_BIND_FUN(_Node2D_get_position)>(*this, lib, "get_position", das::SideEffects::modifyExternal, "_Node2D_get_position");
    das::addExtern<DAS_BIND_FUN(_Node2D_set_position)>(*this, lib, "set_position", das::SideEffects::modifyExternal, "_Node2D_set_position");
    // Sprite2D
    das::addExtern<DAS_BIND_FUN(_Sprite2D_new)>(*this, lib, "Sprite2D`new", das::SideEffects::modifyExternal, "_Sprite2D_new");
    das::addExtern<DAS_BIND_FUN(_Sprite2D_set_texture)>(*this, lib, "set_texture", das::SideEffects::modifyExternal, "_Sprite2D_set_texture");
    das::addExtern<DAS_BIND_FUN(_Sprite2D_set_modulate)>(*this, lib, "set_modulate", das::SideEffects::modifyExternal, "_Sprite2D_set_modulate");
    // Label
    das::addExtern<DAS_BIND_FUN(_Label_set_text)>(*this, lib, "set_text", das::SideEffects::modifyExternal, "_Label_set_text");
    // Window
    das::addExtern<DAS_BIND_FUN(_Window_get_size)>(*this, lib, "get_size", das::SideEffects::modifyExternal, "_Window_get_size");
    // Texture2D
    das::addExtern<DAS_BIND_FUN(_Texture2D_get_size)>(*this, lib, "get_size", das::SideEffects::modifyExternal, "_Texture2D_get_size");
    // InputEvent
    das::addExtern<DAS_BIND_FUN(_InputEvent_is_pressed)>(*this, lib, "is_pressed", das::SideEffects::modifyExternal, "_InputEvent_is_pressed");
    // InputEventMouseButton
    das::addExtern<DAS_BIND_FUN(_InputEventMouseButton_get_button_index)>(*this, lib, "get_button_index", das::SideEffects::modifyExternal, "_InputEventMouseButton_get_button_index");
    // Color
    das::addExtern<DAS_BIND_FUN(_named_color), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "named_color", das::SideEffects::modifyExternal, "_named_color");

    // builtin
    das::addExtern<DAS_BIND_FUN(_Engine_get_frames_per_second)>(*this, lib, "Engine`get_frames_per_second", das::SideEffects::modifyExternal, "_Engine_get_frames_per_second");
    das::addExtern<DAS_BIND_FUN(VariantUtilityFunctions::randf_range)>(*this, lib, "randf_range", das::SideEffects::modifyExternal, "VariantUtilityFunctions::randf_range");
}

