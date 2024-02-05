#ifndef GODOT_TYPES_EXTRA_H
#define GODOT_TYPES_EXTRA_H

// this file is for everything that I don't yet know how to generate

#include "godot_types_macro.h"

#include "core/math/color.h"

MAKE_NATIVE_TYPE_FACTORY(Color)

template <> struct das::cast<Color> : das::cast_fVec<Color> {};

#include "core/math/vector2.h"

MAKE_TYPE_FACTORY_ALIAS(Vector2, tFloat2);
template <> struct das::cast<Vector2> : das::cast_fVec_half<Vector2> {};

#include "core/input/input_enums.h"

DAS_BIND_ENUM_CAST(MouseButton)
DAS_BASE_BIND_ENUM(MouseButton, MouseButton, NONE, LEFT, RIGHT, MIDDLE)

#include "core/variant/variant_utility.h"
#include "core/config/engine.h"

#endif // GODOT_TYPES_EXTRA_H