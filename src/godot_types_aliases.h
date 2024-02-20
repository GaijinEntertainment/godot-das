#ifndef GODOT_TYPES_ALIASES_H
#define GODOT_TYPES_ALIASES_H

#include "godot_types_macro.h"

#include "core/math/vector2.h"

MAKE_TYPE_FACTORY_ALIAS(Vector2, tFloat2);
template <> struct das::cast<Vector2> : das::cast_fVec_half<Vector2> {};

#include "core/math/vector2i.h"

MAKE_TYPE_FACTORY_ALIAS(Vector2i, tInt2);
template <> struct das::cast<Vector2i> : cast_iVec_half<Vector2i> {};

#include "core/templates/rid.h"

MAKE_TYPE_FACTORY_ALIAS(::RID, tUInt64);
template <> struct das::cast<::RID> : das::cast <uint64_t>{};

#endif // GODOT_TYPES_ALIASES_H
