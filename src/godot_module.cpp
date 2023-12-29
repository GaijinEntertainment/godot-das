#include "das_script_instance.h"

#include <daScript/daScript.h>
#include <daScript/misc/platform.h>

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

#define MAKE_NATIVE_TYPE_FACTORY(TYPE) MAKE_TYPE_FACTORY(TYPE, TYPE)


#define BIND_NATIVE_BASE(TYPE)\
struct TYPE##Annotation : das::ManagedStructureAnnotation<TYPE> {\
    TYPE##Annotation(das::ModuleLibrary & ml) : ManagedStructureAnnotation(#TYPE, ml) { }\
};\
das::smart_ptr<TYPE##Annotation> TYPE##ManagedStructureAnnotation = das::make_smart<TYPE##Annotation>(lib);\
addAnnotation(TYPE##ManagedStructureAnnotation);


#define BIND_NATIVE_TYPE(TYPE, PARENT)\
struct TYPE##Annotation : das::ManagedStructureAnnotation<TYPE> {\
    das::TypeDeclPtr parentType;\
    das::smart_ptr<das::ManagedStructureAnnotation<PARENT>> parent_annotation;\
    \
    TYPE##Annotation(das::ModuleLibrary & ml,\
                     das::smart_ptr<das::ManagedStructureAnnotation<PARENT>> parent_annotation) :\
                     ManagedStructureAnnotation(#TYPE, ml),\
                     parent_annotation(parent_annotation),\
                     parentType(das::makeType<PARENT>(ml)) {}\
    \
    bool canBeSubstituted(TypeAnnotation *pass_type) const override {\
        return parentType->annotation == pass_type || parent_annotation->canBeSubstituted(pass_type);\
    }\
};\
das::smart_ptr<TYPE##Annotation> TYPE##ManagedStructureAnnotation = das::make_smart<TYPE##Annotation>(lib, PARENT##ManagedStructureAnnotation);\
addAnnotation(TYPE##ManagedStructureAnnotation);

template <typename T>
bool _check_native_type(const Object* obj) {
    return dynamic_cast<const T*>(obj) != nullptr;
}

#define BIND_TYPE_CHECKER(TYPE)  das::addExtern<DAS_BIND_FUN(_check_native_type<TYPE>)>(*this, lib, "_check_native_type_"#TYPE, das::SideEffects::none, "_check_native_type<"#TYPE">");

#define CTX_AT das::Context *ctx, das::LineInfoArg *at
#define CHECK_IF_NULL(PTR) if (PTR == nullptr) { ctx->throw_error_at(at, "dereferencing null pointer"); return {}; }
#define CHECK_IF_NULL_VOID(PTR) if (PTR == nullptr) { ctx->throw_error_at(at, "dereferencing null pointer"); return; }

char* _get_dascript_type(const Object* obj, const char* name, das::Context *ctx) {
    if (obj == nullptr) {
        ctx->to_err(nullptr, "cannot cast null");
        return nullptr;
    }
    DasScriptInstance* instance = dynamic_cast<DasScriptInstance*>(obj->get_script_instance());
    if (instance == nullptr || instance->get_das_script()->get_class_name() != name) {
        // two cases: either the object does not have a das script instance, or the das script instance is not of the correct type
        ctx->to_err(nullptr, (std::string("type mismatch: cannot cast to ") + std::string(name)).c_str());
        return nullptr;
    }
    return instance->get_class_ptr();
}

bool _check_dascript_type(const Object* obj, const char* name) {
    if (obj == nullptr) {
        return false;
    }
    DasScriptInstance* instance = dynamic_cast<DasScriptInstance*>(obj->get_script_instance());
    if (instance == nullptr || instance->get_das_script()->get_class_name() != name) {
        return false;
    }
    return true;
}

MAKE_NATIVE_TYPE_FACTORY(Object)
MAKE_NATIVE_TYPE_FACTORY(Node)
MAKE_NATIVE_TYPE_FACTORY(Node2D)

MAKE_TYPE_FACTORY_ALIAS(Vector2, tFloat2);
template <> struct das::cast<Vector2> : das::cast_fVec_half<Vector2> {};

Node* _Node_find_child(const Node* node, const char* p_pattern, CTX_AT) {
    CHECK_IF_NULL(node)
    // TODO add p_recursive and p_owned
    return node->find_child(p_pattern, true, true);
}

Node* _Node_get_parent(const Node* node, CTX_AT) {
    CHECK_IF_NULL(node)
    return node->get_parent();
}

const char* _Node_get_name(const Node* node, CTX_AT) {
    CHECK_IF_NULL(node)
    // TODO bind StringName?
    return ctx->stringHeap->allocateString(String(node->get_name()).utf8().get_data());
}

void _Node2D_rotate(Node2D* node2d, float p_radians, CTX_AT) {
    CHECK_IF_NULL_VOID(node2d)
    node2d->rotate(p_radians);
}

void _Node2D_translate(Node2D* node2d, const Vector2 p_amount, CTX_AT) {
    CHECK_IF_NULL_VOID(node2d)
    node2d->translate(p_amount);
}

void _Node2D_set_position(Node2D* node2d, const Vector2 p_pos, CTX_AT) {
    CHECK_IF_NULL_VOID(node2d)
    node2d->set_position(p_pos);
}

Vector2 _Node2D_get_position(const Node2D* node2d, CTX_AT) {
    CHECK_IF_NULL(node2d)
    return node2d->get_position();
}


class Module_Godot : public das::Module {
public:

    Module_Godot() : Module("godot") {
        das::ModuleLibrary lib(this);

        BIND_NATIVE_BASE(Object)
        BIND_NATIVE_TYPE(Node, Object)
        BIND_NATIVE_TYPE(Node2D, Node)

        das::addExtern<DAS_BIND_FUN(_Node2D_rotate)>(*this, lib, "rotate", das::SideEffects::modifyExternal, "_Node2D_rotate");
        das::addExtern<DAS_BIND_FUN(_Node2D_translate)>(*this, lib, "translate", das::SideEffects::modifyExternal, "_Node2D_translate");
        das::addExtern<DAS_BIND_FUN(_Node2D_get_position)>(*this, lib, "get_position", das::SideEffects::modifyExternal, "_Node2D_get_position");
        das::addExtern<DAS_BIND_FUN(_Node2D_set_position)>(*this, lib, "set_position", das::SideEffects::modifyExternal, "_Node2D_set_position");
        das::addExtern<DAS_BIND_FUN(_Node_get_parent)>(*this, lib, "get_parent", das::SideEffects::modifyExternal, "_Node_get_parent");
        das::addExtern<DAS_BIND_FUN(_Node_find_child)>(*this, lib, "find_child", das::SideEffects::modifyExternal, "_Node_find_child");
        das::addExtern<DAS_BIND_FUN(_Node_get_name)>(*this, lib, "get_name", das::SideEffects::modifyExternal, "_Node_get_name");

        BIND_TYPE_CHECKER(Node)
        BIND_TYPE_CHECKER(Node2D)
        das::addExtern<DAS_BIND_FUN(_get_dascript_type)>(*this, lib, "_get_dascript_type", das::SideEffects::modifyExternal, "_get_dascript_type");
        das::addExtern<DAS_BIND_FUN(_check_dascript_type)>(*this, lib, "_check_dascript_type", das::SideEffects::modifyExternal, "_check_dascript_type");

        addAlias(das::typeFactory<Vector2>::make(lib));

        options["tool"] = das::Type::tBool;

        const char* godot_module_full_path = GODOT_DAS_MODULE_PATH"/src/godot.das";
        std::ifstream input{godot_module_full_path, std::ios_base::binary};
        std::vector<unsigned char> buffer{std::istreambuf_iterator< char>(input), {} };

        compileBuiltinModule(godot_module_full_path, buffer.data(), buffer.size());
    }
};

REGISTER_MODULE(Module_Godot);
