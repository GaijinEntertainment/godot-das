#ifndef GODOT_MODULE_H
#define GODOT_MODULE_H

#include <daScript/daScript.h>

class Module_Godot : public das::Module {
public:
    Module_Godot() : Module("godot") {
        das::addConstant(*this, "TEST", 5);
    }
};

REGISTER_MODULE(Module_Godot);

#endif // GODOT_MODULE_H
