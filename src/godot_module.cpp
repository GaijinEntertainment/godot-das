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

#define NAME_NATIVE_TYPE_FACTORY(TYPE) MAKE_TYPE_FACTORY(TYPE##Native, TYPE)

#define STR(X) #X
#define DEFER_STR(X) STR(X)
#define DAS_MEMBER_NAME(CLASS, MEMBER) _##CLASS##_##MEMBER

#define BIND_METHOD(CLASS, METHOD)\
using DAS_MEMBER_NAME(CLASS, METHOD) = DAS_CALL_MEMBER(CLASS::METHOD);\
das::addExtern<DAS_CALL_METHOD(DAS_MEMBER_NAME(CLASS, METHOD))>(*this, lib, DEFER_STR(DAS_MEMBER_NAME(CLASS, METHOD)), das::SideEffects::modifyExternal, DAS_CALL_MEMBER_CPP(CLASS::METHOD));


#define BIND_NATIVE_BASE(TYPE)\
struct TYPE##Annotation : das::ManagedStructureAnnotation<TYPE> {\
    TYPE##Annotation(das::ModuleLibrary & ml) : ManagedStructureAnnotation(#TYPE"Native", ml) { }\
};\
addAnnotation(das::make_smart<TYPE##Annotation>(lib));


#define BIND_NATIVE_TYPE(TYPE, PARENT)\
struct TYPE##Annotation : das::ManagedStructureAnnotation<TYPE> {\
    das::TypeDeclPtr parentType;\
    TYPE##Annotation(das::ModuleLibrary & ml) : ManagedStructureAnnotation(#TYPE"Native", ml) {\
        parentType = das::makeType<PARENT>(ml);\
    }\
    bool canBeSubstituted(TypeAnnotation *pass_type) const override {\
        return parentType->annotation == pass_type;\
    }\
};\
addAnnotation(das::make_smart<TYPE##Annotation>(lib));



NAME_NATIVE_TYPE_FACTORY(Object)
NAME_NATIVE_TYPE_FACTORY(Node)
NAME_NATIVE_TYPE_FACTORY(Node2D)

template<> struct das::ToBasicType<String>     { enum { type = das::Type::tString }; };

MAKE_TYPE_FACTORY_ALIAS(Vector2, tFloat2);
template <> struct das::cast<Vector2> : das::cast_fVec_half<Vector2> {};

Node* fwd_Node_find_child(Node& node, const char* p_pattern, bool p_recursive = true, bool p_owned = true) {
    // TODO embed this cast into das?
    // or maybe make all method calls like this
    return node.find_child(p_pattern, p_recursive, p_owned);
}

class Module_Godot : public das::Module {
public:

    Module_Godot() : Module("godot") {
        das::ModuleLibrary lib(this);

        BIND_NATIVE_BASE(Object)
        BIND_NATIVE_TYPE(Node, Object)
        BIND_NATIVE_TYPE(Node2D, Node)

        BIND_METHOD(Node2D, rotate)
        BIND_METHOD(Node2D, translate)
        BIND_METHOD(Node2D, get_position)
        BIND_METHOD(Node2D, set_position)
        BIND_METHOD(Node, get_parent)

        das::addExtern<DAS_BIND_FUN(fwd_Node_find_child)>(*this, lib, "_Node_find_child", das::SideEffects::modifyExternal, "fwd_Node_find_child");

        addAlias(das::typeFactory<Vector2>::make(lib));

        options["tool"] = das::Type::tBool;

        std::ifstream input{GODOT_DAS_MODULE_PATH"/src/godot.das", std::ios_base::binary};
        std::vector<unsigned char> buffer{std::istreambuf_iterator< char>(input), {} };
        compileBuiltinModule("godot.das", buffer.data(), buffer.size());
    }
};

REGISTER_MODULE(Module_Godot);
