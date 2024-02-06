// This file is complitely generated

#include "godot_module.h"
#include "godot_types_gen.h"

void Module_Godot::bind_types_gen(das::ModuleLibrary & lib) {
    BIND_NATIVE_BASE(Object)
    BIND_ENUM(Object, ConnectFlags)
    BIND_NATIVE_TYPE(Node, Object)
    BIND_ENUM(Node, ProcessMode)
    BIND_ENUM(Node, ProcessThreadGroup)
    BIND_ENUM(Node, ProcessThreadMessages)
    BIND_ENUM(Node, DuplicateFlags)
    BIND_ENUM(Node, InternalMode)
    BIND_NATIVE_TYPE(CanvasItem, Node)
    BIND_ENUM(CanvasItem, TextureFilter)
    BIND_ENUM(CanvasItem, TextureRepeat)
    BIND_ENUM(CanvasItem, ClipChildrenMode)
    BIND_NATIVE_TYPE(Node2D, CanvasItem)
    BIND_NATIVE_TYPE(Sprite2D, Node2D)
    BIND_NATIVE_TYPE(Area2D, Node2D)
    BIND_ENUM(Area2D, SpaceOverride)
    BIND_NATIVE_TYPE(Label, CanvasItem)
    BIND_NATIVE_TYPE(Window, Node)
    BIND_ENUM(Window, Mode)
    BIND_ENUM(Window, Flags)
    BIND_ENUM(Window, ContentScaleMode)
    BIND_ENUM(Window, ContentScaleAspect)
    BIND_ENUM(Window, ContentScaleStretch)
    BIND_ENUM(Window, LayoutDirection)
    BIND_ENUM(Window, WindowInitialPosition)
    BIND_NATIVE_TYPE(Resource, Object)
    BIND_NATIVE_TYPE(PackedScene, Resource)
    BIND_ENUM(PackedScene, GenEditState)
    BIND_NATIVE_TYPE(InputEvent, Resource)
    BIND_NATIVE_TYPE(InputEventMouseButton, InputEvent)
    BIND_NATIVE_TYPE(Texture2D, Resource)
}
