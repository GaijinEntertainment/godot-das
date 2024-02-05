#ifndef GODOT_TYPES_H
#define GODOT_TYPES_H

#include <daScript/daScript.h>

#define MAKE_NATIVE_TYPE_FACTORY(TYPE) MAKE_TYPE_FACTORY(TYPE, TYPE)

#define BIND_NATIVE_BASE(TYPE)\
struct TYPE##Annotation : das::ManagedStructureAnnotation<TYPE, false, false> {\
    TYPE##Annotation(das::ModuleLibrary & ml) : ManagedStructureAnnotation(#TYPE, ml) { }\
    bool hasNonTrivialCtor() const override { return false; } /* tmp solution so types (not ptrs) can be returned */\
};\
das::smart_ptr<TYPE##Annotation> TYPE##ManagedStructureAnnotation = das::make_smart<TYPE##Annotation>(lib);\
addAnnotation(TYPE##ManagedStructureAnnotation);\

#define BIND_NATIVE_TYPE(TYPE, PARENT)\
struct TYPE##Annotation : das::ManagedStructureAnnotation<TYPE, false, false> {\
    das::TypeDeclPtr parentType;\
    das::smart_ptr<das::ManagedStructureAnnotation<PARENT, false, false>> parent_annotation;\
    \
    TYPE##Annotation(das::ModuleLibrary & ml,\
                     das::smart_ptr<das::ManagedStructureAnnotation<PARENT, false, false>> parent_annotation) :\
                     ManagedStructureAnnotation(#TYPE, ml),\
                     parent_annotation(parent_annotation),\
                     parentType(das::makeType<PARENT>(ml)) {}\
    \
    bool canBeSubstituted(TypeAnnotation *pass_type) const override {\
        return parentType->annotation == pass_type || parent_annotation->canBeSubstituted(pass_type);\
    }\
    bool hasNonTrivialCtor() const override { return false; } /* tmp solution so types (not ptrs) can be returned */\
};\
das::smart_ptr<TYPE##Annotation> TYPE##ManagedStructureAnnotation = das::make_smart<TYPE##Annotation>(lib, PARENT##ManagedStructureAnnotation);\
addAnnotation(TYPE##ManagedStructureAnnotation);\
struct type_checker_##TYPE {static bool check(const Object* obj) { return dynamic_cast<const TYPE*>(obj) != nullptr; }};\
das::addExtern<DAS_BIND_FUN(type_checker_##TYPE::check)>(*this, lib, "_check_native_type_"#TYPE, das::SideEffects::none, "type_checker_"#TYPE"::check");

// type_checker_##TYPE instead of just  check_type<T> because it should be after all includes
// but I want all non-generated code not in _gen files. and extra file would be excessive


// taken from dagor/prog/gameLibs/publicInclude/dasModules/dasModulesCommon.h
// typeName and das_alias do not seem to be vital but may be required in the future
#define MAKE_TYPE_FACTORY_ALIAS(TYPE, DAS_DECL_TYPE)\
template <>\
struct das::typeFactory<TYPE>{\
    static das::TypeDeclPtr make(const ModuleLibrary &lib) {\
        auto t = make_smart<TypeDecl>(Type::DAS_DECL_TYPE);\
        t->alias = #TYPE;\
        t->aotAlias = true;\
        return t;\
    }\
};

#endif // GODOT_TYPES_H
