#ifndef GODOT_MODULE_H
#define GODOT_MODULE_H

#include <daScript/daScript.h>
#include <scene/2d/node_2d.h>
#include <core/math/vector2.h>

// taken from dagor/prog/gameLibs/publicInclude/dasModules/dasModulesCommon.h
// typeName and das_alias do not seem to be vital but may be required in the future

#define MAKE_TYPE_FACTORY_ALIAS(TYPE, DAS_DECL_TYPE) \
namespace das {                                                \
  template <>                                                  \
  struct typeFactory<TYPE>                                     \
  {                                                            \
    static TypeDeclPtr make(const ModuleLibrary &lib)          \
    {                                                          \
      auto t = make_smart<TypeDecl>(Type::DAS_DECL_TYPE);      \
      t->alias = #TYPE;                                        \
      t->aotAlias = true;                                      \
      return t;                                                \
    }                                                          \
  };                                                           \
}

MAKE_TYPE_FACTORY(Node2DNative, Node2D);
MAKE_TYPE_FACTORY_ALIAS(Vector2, tFloat2);

struct Node2DAnnotation : das::ManagedStructureAnnotation<Node2D, false> {
    Node2DAnnotation(das::ModuleLibrary & ml) : ManagedStructureAnnotation("Node2DNative", ml) {
    }
};


class Module_Godot : public das::Module {
public:

    Module_Godot() : Module("godot") {
        das::ModuleLibrary lib(this);
        addAnnotation(das::make_smart<Node2DAnnotation>(lib));
        addAlias(das::typeFactory<Vector2>::make(lib));

        using method_Node2D_rotate = DAS_CALL_MEMBER(Node2D::rotate);
        das::addExtern<DAS_CALL_METHOD(method_Node2D_rotate)>(*this, lib, "_Node2D_rotate", das::SideEffects::modifyExternal, DAS_CALL_MEMBER_CPP(Node2D::rotate));
        using method_Node2D_translate = DAS_CALL_MEMBER(Node2D::translate);
        das::addExtern<DAS_CALL_METHOD(method_Node2D_translate)>(*this, lib, "_Node2D_translate", das::SideEffects::modifyExternal, DAS_CALL_MEMBER_CPP(Node2D::translate));

        options["tool"] = das::Type::tBool;

        #include "godot.das.inc"
        compileBuiltinModule("godot.das", modules_dascript_src_godot_das, modules_dascript_src_godot_das_len);
    }
};

REGISTER_MODULE(Module_Godot);

#endif // GODOT_MODULE_H
