#include "godot_types_extra.h"
#include "godot_module.h"


void Module_Godot::bind_types_extra(das::ModuleLibrary & lib) {
    BIND_NATIVE_BASE(Color)

    addAlias(das::typeFactory<Vector2>::make(lib));
    addAlias(das::typeFactory<::RID>::make(lib));
}