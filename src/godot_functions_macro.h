#ifndef GODOT_FUNCTIONS_MACRO_H
#define GODOT_FUNCTIONS_MACRO_H

#define CTX_AT das::Context *ctx, das::LineInfoArg *at
#define CHECK_IF_NULL_MSG(PTR, MSG) if (PTR == nullptr) { ctx->throw_error_at(at, MSG); return {}; }
#define CHECK_IF_NULL(PTR) CHECK_IF_NULL_MSG(PTR, "dereferencing null pointer")
#define CHECK_IF_NULL_VOID(PTR) if (PTR == nullptr) { ctx->throw_error_at(at, "dereferencing null pointer"); return; }

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


#endif // GODOT_FUNCTIONS_MACRO_H
