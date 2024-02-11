#ifndef GODOT_FUNCTIONS_MACRO_H
#define GODOT_FUNCTIONS_MACRO_H

#define CTX_AT das::Context *ctx, das::LineInfoArg *at
#define CHECK_IF_NULL_MSG(PTR, MSG) if (PTR == nullptr) { ctx->throw_error_at(at, MSG); return {}; }
#define CHECK_IF_NULL(PTR) CHECK_IF_NULL_MSG(PTR, "dereferencing null pointer")
#define CHECK_IF_NULL_VOID(PTR) if (PTR == nullptr) { ctx->throw_error_at(at, "dereferencing null pointer"); return; }
#define CHECK_IF_NULL_VOID_MSG(PTR, MSG) if (PTR == nullptr) { ctx->throw_error_at(at, MSG); return; }

template <typename FuncT, FuncT fun>
struct das_call_godot_member;

template <typename FuncT, FuncT fun>
struct das_call_godot_static_member;

template <typename FuncT, FuncT fun>
struct das_call_godot_singleton_member;

#define DAS_CALL_GODOT_MEMBER(type_and_fun) das_call_godot_member< DAS_BIND_MEMBER_FUN(type_and_fun) >
#define DAS_CALL_GODOT_STATIC_MEMBER(type_and_fun) das_call_godot_static_member< DAS_BIND_MEMBER_FUN(type_and_fun) >
#define DAS_CALL_GODOT_SINGLETON_MEMBER(type_and_fun) das_call_godot_singleton_member< DAS_BIND_MEMBER_FUN(type_and_fun) >

#define DAS_CALL_GODOT_MEMBER_CPP(type_and_fun) "das_call_godot_member< DAS_BIND_MEMBER_FUN(" #type_and_fun ")>::invoke"
#define DAS_CALL_GODOT_STATIC_MEMBER_CPP(type_and_fun) "das_call_godot_static_member< DAS_BIND_MEMBER_FUN(" #type_and_fun ")>::invoke"
#define DAS_CALL_GODOT_SINGLETON_MEMBER_CPP(type_and_fun) "das_call_godot_sungleton_member< DAS_BIND_MEMBER_FUN(" #type_and_fun ")>::invoke"

#include "core/os/memory.h"

template <class T>
T *creator() {
    return memnew(T);
}

#define BIND_GODOT_MEMBER_RENAME(CLASS, FUN, DAS_FUN, ...)\
    using _##CLASS##_##FUN = DAS_CALL_GODOT_MEMBER(CLASS::FUN);\
    auto _##CLASS##_##FUN##_func = das::addExtern<DAS_BIND_FUN(_##CLASS##_##FUN::invoke), _##CLASS##_##FUN::simnode::type>(*this, lib, #DAS_FUN, _##CLASS##_##FUN::effects, DAS_CALL_GODOT_MEMBER_CPP(CLASS::FUN));\
     _##CLASS##_##FUN##_func->args({"this", ##__VA_ARGS__, "__ctx__", "__at__"});

#define BIND_GODOT_MEMBER(CLASS, FUN, ...) BIND_GODOT_MEMBER_RENAME(CLASS, FUN, FUN, ##__VA_ARGS__)

#define BIND_GODOT_CTOR(CLASS)\
    das::addExtern<DAS_BIND_FUN(creator<CLASS>)>(*this, lib, #CLASS"`new", das::SideEffects::modifyExternal, "creator<"#CLASS">");

#define REMOVE_FIRST_COMMA(_0, ...) __VA_ARGS__

#define BIND_GODOT_SINGLETON_MEMBER_BASE(CLASS, FUN, NAME, ...)\
    using _##CLASS##_##FUN = DAS_CALL_GODOT_SINGLETON_MEMBER(CLASS::FUN);\
    auto _##CLASS##_##FUN##_func = das::addExtern<DAS_BIND_FUN(_##CLASS##_##FUN::invoke)>(*this, lib, NAME, _##CLASS##_##FUN::effects, DAS_CALL_GODOT_SINGLETON_MEMBER_CPP(CLASS::FUN));\
    _##CLASS##_##FUN##_func->args({REMOVE_FIRST_COMMA(,##__VA_ARGS__, "__ctx__", "__at__")});

#define BIND_GODOT_SINGLETON_MEMBER(CLASS, FUN, ...) BIND_GODOT_SINGLETON_MEMBER_BASE(CLASS, FUN, #CLASS"`"#FUN, ##__VA_ARGS__)

#define BIND_GODOT_SINGLETON_MEMBER_BUILTIN(CLASS, FUN, ...) BIND_GODOT_SINGLETON_MEMBER_BASE(CLASS, FUN, #FUN, ##__VA_ARGS__)

// more examples of usage are needed
#define BIND_GODOT_BUILTIN_FUNCTION(CLASS, FUN)\
    using _##CLASS##_##FUN = DAS_CALL_GODOT_STATIC_MEMBER(CLASS::FUN);\
    das::addExtern<DAS_BIND_FUN(_##CLASS##_##FUN::invoke)>(*this, lib, #FUN, _##CLASS##_##FUN::effects, DAS_CALL_GODOT_STATIC_MEMBER_CPP(CLASS::FUN));


template <typename T> struct ExprConstTyped { };

template <> struct ExprConstTyped<bool> { typedef das::ExprConstBool expr_type; };
template <> struct ExprConstTyped<int> { typedef das::ExprConstInt expr_type; };
template <> struct ExprConstTyped<double> { typedef das::ExprConstDouble expr_type; };
template <> struct ExprConstTyped<float> { typedef das::ExprConstFloat expr_type; };
template <> struct ExprConstTyped<const char*> { typedef das::ExprConstString expr_type; };
template <> struct ExprConstTyped<das::float2> { typedef das::ExprConstFloat2 expr_type; };
template <> struct ExprConstTyped<das::int2> { typedef das::ExprConstInt2 expr_type; };

// tmp solution for aliases
// these types are not converted to das types with `escape` because they are aliases
// but they need to be converted here to form a proper const expr
template <typename T> struct ExprAliasConv { typedef T type; };
template <> struct ExprAliasConv<const Vector2&> { typedef das::float2 type; };

template <typename T>
struct ExprConstMakerEnum {
    static auto make(T value, das::ModuleLibrary &lib) { return das::make_smart<das::ExprConstEnumeration>(value, das::typeFactory<T>::make(lib)); }
};

template <typename T>
struct ExprConstMaker {
    static auto make(T value, das::ModuleLibrary &) { return das::make_smart<typename ExprConstTyped<T>::expr_type>(value); }
};

template <typename T>
using ExprConstMakerCond = std::conditional_t<std::is_enum_v<T>, ExprConstMakerEnum<T>, ExprConstMaker<T>>;


#define SET_DEFAULT_ARG(CLASS, FUN, POSITION, VALUE)\
using  _##CLASS##_##FUN##_##POSITION = ExprAliasConv<std::tuple_element_t<POSITION, _##CLASS##_##FUN::args>>::type;\
_##CLASS##_##FUN##_func->arg_init(POSITION, ExprConstMakerCond<_##CLASS##_##FUN##_##POSITION>::make(static_cast<_##CLASS##_##FUN##_##POSITION>(VALUE), lib));



#endif // GODOT_FUNCTIONS_MACRO_H
