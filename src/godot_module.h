#ifndef GODOT_MODULE_H
#define GODOT_MODULE_H

#include <daScript/daScript.h>

class Module_Godot : public das::Module {
public:
    Module_Godot() : Module("godot") {
        #include "godot.das.inc"
        compileBuiltinModule("godot.das", src_godot_das, src_godot_das_len);
    }
};

REGISTER_MODULE(Module_Godot);

#endif // GODOT_MODULE_H
