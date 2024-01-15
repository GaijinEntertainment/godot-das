// This file should be complitely generated

#ifndef GODOT_TYPES_GEN_H
#define GODOT_TYPES_GEN_H

#include "godot_types_macro.h"


#include "core/object/object.h"

MAKE_NATIVE_TYPE_FACTORY(Object)

#include "scene/main/node.h"

MAKE_NATIVE_TYPE_FACTORY(Node)

#include "scene/main/canvas_item.h"

MAKE_NATIVE_TYPE_FACTORY(CanvasItem)

#include "scene/2d/node_2d.h"

MAKE_NATIVE_TYPE_FACTORY(Node2D)

#include "core/input/input.h"

MAKE_NATIVE_TYPE_FACTORY(InputEvent)
MAKE_NATIVE_TYPE_FACTORY(InputEventMouseButton)

#include "scene/2d/sprite_2d.h"
MAKE_NATIVE_TYPE_FACTORY(Sprite2D)

#include "scene/resources/texture.h"

MAKE_NATIVE_TYPE_FACTORY(Resource)
MAKE_NATIVE_TYPE_FACTORY(Texture2D)

#include "scene/main/window.h"

MAKE_NATIVE_TYPE_FACTORY(Window)

#include "scene/gui/label.h"

MAKE_NATIVE_TYPE_FACTORY(Label)

#include "core/math/color.h"

MAKE_NATIVE_TYPE_FACTORY(Color)

template <> struct das::cast<Color> : das::cast_fVec<Color> {};

#include "core/math/vector2.h"

MAKE_TYPE_FACTORY_ALIAS(Vector2, tFloat2);
template <> struct das::cast<Vector2> : das::cast_fVec_half<Vector2> {};

#include "core/input/input_enums.h"

DAS_BIND_ENUM_CAST(MouseButton)
DAS_BASE_BIND_ENUM(MouseButton, MouseButton, NONE, LEFT, RIGHT, MIDDLE)

DAS_BIND_ENUM_CAST(Node::InternalMode)
DAS_BASE_BIND_ENUM(Node::InternalMode, Node_InternalMode, INTERNAL_MODE_DISABLED, INTERNAL_MODE_FRONT, INTERNAL_MODE_BACK)


#endif // GODOT_TYPES_GEN_H
