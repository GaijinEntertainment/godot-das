#ifndef GODOT_FUNCTIONS_WRAPPER_H
#define GODOT_FUNCTIONS_WRAPPER_H

#include "godot_functions_macro.h"
#include "daScript/daScript.h"
#include "core/typedefs.h"


// This is a separate file because
// 1. godot_functions_macro.h can be used elsewhere (it was used in godot_utils.cpp originally)
// 2. if this file was merged into godot_functions_macro.h it would be too cluttered


template <typename T>
struct simnode_chooser {
    template <typename FuncT, FuncT fun>
    using type = das::SimNode_ExtFuncCall<FuncT, fun>;
};

template <typename TT>
struct simnode_chooser<TypedArray<TT>> {
    template <typename FuncT, FuncT fun>
    using type = das::SimNode_ExtFuncCallAndCopyOrMove<FuncT, fun>;
};


template <typename T>
struct default_return {
    _FORCE_INLINE_ static T&& ret(T&& t, das::Context *ctx) {
        return std::forward<T>(t);
    }
};

template <typename T>
struct escape : default_return<T> {
    typedef T type;
};

template <typename T>
struct escape<T*> {
    typedef T* type;
    _FORCE_INLINE_ static T* ret(T* t, das::Context *) {
        return t;
    }
};

// tmp solution, let's just use floats everywhere
template <>
struct escape<double> : default_return<float>{
    typedef float type;
};

#include "core/variant/typed_array.h"

template <typename T>
struct escape<TypedArray<T>> {
    typedef das::TArray<T*> type;

    _FORCE_INLINE_ static das::TArray<T*> ret(const TypedArray<T>& t, das::Context *ctx) {
        das::TArray<T*> array;
        array.data = nullptr;
        array.size = 0;
        array.capacity = 0;
        array.lock = 0;
        int new_size = t.size();
        if (new_size > 0) {
            das::array_resize(*ctx, array, t.size(), sizeof(T), false, nullptr);
            for (int i = 0; i < t.size(); i++) {
                array[i] = reinterpret_cast<T*>(t[i].operator Object*());
            }
        }
        return array;
    }
};

#include "core/object/ref_counted.h"

template <typename T>
struct escape<Ref<T>> {
    typedef T* type;
    _FORCE_INLINE_ static T* ret(Ref<T> t, das::Context *) {
        // TODO THIS SOLUTION IS SUPER HACKY AND WILL CAUSE MEMORY LEAKS
        t->reference();
        return t.ptr();
    }
};

template <typename T>
struct escape<const Ref<T>&> : escape<Ref<T>> { };


#include "core/math/vector2.h"
#include "core/math/vector2i.h"
// TODO just bind Vector2i lol
template <>
struct escape<Vector2i>: default_return<Vector2> {
    typedef Vector2 type;
};

#include "core/string/ustring.h"

template<>
struct escape<const String&> {
    typedef const char* type;
    _FORCE_INLINE_ const char * ret(const String& t, das::Context *ctx) {
        return ctx->stringHeap->allocateString(t.utf8().get_data());
    }
};

#include "core/string/string_name.h"

template<>
struct escape<StringName> {
    typedef const char* type;
    _FORCE_INLINE_ static const char * ret(const StringName& t, das::Context *ctx) {
        return ctx->stringHeap->allocateString(String(t).utf8().get_data());
    }
};

// TODO escape const T& more generally
template<>
struct escape<const StringName&> : escape<StringName> {};

#define RETURN(V) return escape<decltype(V)>::ret(V, ctx);
#define ESCAPE(T) typename escape<T>::type
#define ESCAPE_R ESCAPE(R)
// this is done because VSCode doesn't like `ESCAPE(Args)...` in function declaration for some reason
#define ESCAPE_ARGS ESCAPE(Args)...

// TODO maybe some functions should have modufyExternal??

// no const, return
template <typename R, typename CC, typename ...Args, R (CC::*func)(Args...) >
struct das_call_godot_member < R (CC::*)(Args...),  func> {
    static ESCAPE_R invoke ( CC * THIS, ESCAPE_ARGS args, CTX_AT) {
        CHECK_IF_NULL(THIS)
        RETURN( ((*THIS).*(func)) ( args... ) );
    }
    constexpr static das::SideEffects effects = das::SideEffects::modifyArgument;
    typedef std::tuple<CC, ESCAPE_ARGS> args;
    typedef simnode_chooser<R> simnode;
};

// no const, no return

template <typename CC, typename ...Args, void (CC::*func)(Args...) >
struct das_call_godot_member < void (CC::*)(Args...),  func> {
    static void invoke ( CC * THIS, ESCAPE_ARGS args, CTX_AT) {
        CHECK_IF_NULL_VOID(THIS)
        ((*THIS).*(func)) ( args... );
    }
    constexpr static das::SideEffects effects = das::SideEffects::modifyArgument;
    typedef std::tuple<CC, ESCAPE_ARGS> args;
    typedef simnode_chooser<void> simnode;
};

// const, return

template <typename R, typename CC, typename ...Args, R (CC::*func)(Args...) const>
struct das_call_godot_member < R (CC::*)(Args...) const,  func> {
    static ESCAPE_R invoke ( const CC * THIS, ESCAPE_ARGS args, CTX_AT) {
        CHECK_IF_NULL(THIS)
        RETURN( ((*THIS).*(func)) ( args... ) );
    }
    constexpr static das::SideEffects effects = das::SideEffects::none;
    typedef std::tuple<CC, ESCAPE_ARGS> args;
    typedef simnode_chooser<R> simnode;
};

// const, no return

template <typename CC, typename ...Args, void (CC::*func)(Args...) const>
struct das_call_godot_member < void (CC::*)(Args...) const,  func> {
    static void invoke ( const CC * THIS, ESCAPE_ARGS args, CTX_AT) {
        CHECK_IF_NULL_VOID(THIS)
        ((*THIS).*(func)) ( args... );
    }
    constexpr static das::SideEffects effects = das::SideEffects::none;
    typedef std::tuple<CC, ESCAPE_ARGS> args;
    typedef simnode_chooser<void> simnode;
};

// static variant

template <typename R, typename ...Args, R (*func)(Args...) >
struct das_call_godot_static_member < R (*)(Args...), func> {
    static ESCAPE_R invoke (ESCAPE_ARGS args, CTX_AT) {
        RETURN( (*func) ( args... ) );
    }
    // modifyExternal - because, for example, random changes global random generator
    constexpr static das::SideEffects effects = das::SideEffects::modifyExternal;
    typedef typename std::tuple<ESCAPE_ARGS> args;
};

// singleton variant, const

template <typename R, typename CC, typename ...Args, R (CC::*func)(Args...) const>
struct das_call_godot_singleton_member < R (CC::*)(Args...) const,  func> {
    static ESCAPE_R invoke (ESCAPE_ARGS args, CTX_AT) {
        RETURN( (CC::get_singleton()->*(func)) ( args... ) );
    }
    constexpr static das::SideEffects effects = das::SideEffects::accessExternal;
    typedef std::tuple<ESCAPE_ARGS> args;
};

// singleton variant, no const

template <typename R, typename CC, typename ...Args, R (CC::*func)(Args...)>
struct das_call_godot_singleton_member < R (CC::*)(Args...),  func> {
    static ESCAPE_R invoke (ESCAPE_ARGS args, CTX_AT) {
        RETURN( (CC::get_singleton()->*(func)) ( args... ) );
    }
    constexpr static das::SideEffects effects = das::SideEffects::accessExternal;
    typedef std::tuple<ESCAPE_ARGS> args;
};

#endif // GODOT_FUNCTIONS_WRAPPER_H