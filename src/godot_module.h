#ifndef GODOT_MODULE_H
#define GODOT_MODULE_H

#include <daScript/daScript.h>
#include <scene/2d/node_2d.h>

MAKE_TYPE_FACTORY(Node2DNative, Node2D);

struct Node2DAnnotation : das::ManagedStructureAnnotation<Node2D, false> {
    Node2DAnnotation(das::ModuleLibrary & ml) : ManagedStructureAnnotation("Node2DNative", ml) {
    }
};


class Module_Godot : public das::Module {
public:

    Module_Godot() : Module("godot") {
        das::ModuleLibrary lib(this);
        addAnnotation(das::make_smart<Node2DAnnotation>(lib));

        using method_Node2D_rotate = DAS_CALL_MEMBER(Node2D::rotate);
        das::addExtern<DAS_CALL_METHOD(method_Node2D_rotate)>(*this, lib, "_Node2D_rotate", das::SideEffects::modifyExternal, DAS_CALL_MEMBER_CPP(Node2D::rotate));

        options["tool"] = das::Type::tBool;

        #include "godot.das.inc"
        compileBuiltinModule("godot.das", src_godot_das, src_godot_das_len);
    }
};

REGISTER_MODULE(Module_Godot);

#endif // GODOT_MODULE_H
