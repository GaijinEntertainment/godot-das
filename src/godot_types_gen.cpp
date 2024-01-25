// This file is complitely generated

#include "godot_module.h"
#include "godot_types_gen.h"

void Module_Godot::bind_types_gen(das::ModuleLibrary & lib) {
    BIND_NATIVE_BASE(Object)
    BIND_NATIVE_TYPE(Node, Object)
    BIND_NATIVE_TYPE(CanvasItem, Node)
    BIND_NATIVE_TYPE(Node2D, CanvasItem)
    BIND_NATIVE_TYPE(Sprite2D, Node2D)
    BIND_NATIVE_TYPE(Label, CanvasItem)
    BIND_NATIVE_TYPE(Window, Node)
    BIND_NATIVE_TYPE(Resource, Object)
    BIND_NATIVE_TYPE(InputEvent, Resource)
    BIND_NATIVE_TYPE(InputEventMouseButton, InputEvent)
    BIND_NATIVE_TYPE(Texture2D, Resource)
}
