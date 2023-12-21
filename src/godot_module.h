#ifndef GODOT_MODULE_H
#define GODOT_MODULE_H

#include <daScript/daScript.h>
#include <scene/2d/node_2d.h>
#include <core/math/vector2.h>

#include <vector>
#include <fstream>

// taken from dagor/prog/gameLibs/publicInclude/dasModules/dasModulesCommon.h
// typeName and das_alias do not seem to be vital but may be required in the future
#define MAKE_TYPE_FACTORY_ALIAS(TYPE, DAS_DECL_TYPE)           \
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

#define STR(X) #X
#define DEFER_STR(X) STR(X)
#define DAS_MEMBER_NAME(CLASS, MEMBER) _##CLASS##_##MEMBER

#define BIND_METHOD(CLASS, METHOD)\
using DAS_MEMBER_NAME(CLASS, METHOD) = DAS_CALL_MEMBER(CLASS::METHOD);\
das::addExtern<DAS_CALL_METHOD(DAS_MEMBER_NAME(CLASS, METHOD))>(*this, lib, DEFER_STR(DAS_MEMBER_NAME(CLASS, METHOD)), das::SideEffects::modifyExternal, DAS_CALL_MEMBER_CPP(CLASS::METHOD));


MAKE_TYPE_FACTORY(Node2DNative, Node2D);
MAKE_TYPE_FACTORY_ALIAS(Vector2, tFloat2);
template <> struct das::cast<Vector2> : das::cast_fVec_half<Vector2> {};


struct Node2DAnnotation : das::ManagedStructureAnnotation<Node2D> {
    Node2DAnnotation(das::ModuleLibrary & ml) : ManagedStructureAnnotation("Node2DNative", ml) {
    }
};


class Module_Godot : public das::Module {
public:

    Module_Godot() : Module("godot") {
        das::ModuleLibrary lib(this);
        addAnnotation(das::make_smart<Node2DAnnotation>(lib));
        addAlias(das::typeFactory<Vector2>::make(lib));

        BIND_METHOD(Node2D, rotate)
        BIND_METHOD(Node2D, translate)
        BIND_METHOD(Node2D, get_position)
        BIND_METHOD(Node2D, set_position)

        options["tool"] = das::Type::tBool;

        std::ifstream input{GODOT_DAS_MODULE_PATH"/src/godot.das", std::ios_base::binary};
        std::vector<unsigned char> buffer{std::istreambuf_iterator< char>(input), {} };
        compileBuiltinModule("godot.das", buffer.data(), buffer.size());
    }
};

REGISTER_MODULE(Module_Godot);

#endif // GODOT_MODULE_H
