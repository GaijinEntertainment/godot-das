#ifndef MODULE_GODOT_H
#define MODULE_GODOT_H


#include <daScript/daScript.h>

class Module_Godot : public das::Module {
    void bind_types(das::ModuleLibrary & lib);
    void bind_functions_gen(das::ModuleLibrary & lib);
    void bind_functions_extra(das::ModuleLibrary & lib);
    void bind_utils(das::ModuleLibrary & lib);
public:
    Module_Godot() : Module("godot_native") {
        das::ModuleLibrary lib(this);
        options["tool"] = das::Type::tBool;

        bind_types(lib);
        bind_functions_gen(lib);
        bind_functions_extra(lib);
        bind_utils(lib);
    }
};

#endif // MODULE_GODOT_H
