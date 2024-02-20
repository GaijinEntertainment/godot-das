#ifndef GODOT_TYPES_EXTRA_H
#define GODOT_TYPES_EXTRA_H

// this file is for everything that I don't yet know how to generate

#include "godot_types_macro.h"

#include "core/math/color.h"

MAKE_NATIVE_TYPE_FACTORY(Color)

template <> struct das::cast<Color> : das::cast_fVec<Color> {};

#include "core/core_bind.h"

DAS_BIND_ENUM_CAST(core_bind::ResourceLoader::CacheMode)
DAS_BASE_BIND_ENUM_SAFE(core_bind::ResourceLoader::CacheMode, ResourceLoader`CacheMode, ResourceLoader_CacheMode, CACHE_MODE_IGNORE, CACHE_MODE_REUSE, CACHE_MODE_REPLACE)

#include "core/variant/variant_utility.h"
#include "core/config/engine.h"


#endif // GODOT_TYPES_EXTRA_H