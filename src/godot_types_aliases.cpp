#include "godot_types_aliases.h"
#include "godot_module.h"


void Module_Godot::bind_types_aliases(das::ModuleLibrary & lib) {
    addAlias(das::typeFactory<Vector2>::make(lib));
    addAlias(das::typeFactory<Vector2i>::make(lib));
    addAlias(das::typeFactory<::RID>::make(lib));
}
