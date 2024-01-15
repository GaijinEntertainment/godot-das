#ifndef GODOT_FUNCTIONS_WRAPPER_H
#define GODOT_FUNCTIONS_WRAPPER_H

#include "godot_functions_macro.h"
#include "daScript/daScript.h"
#include "core/typedefs.h"


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

#include "core/object/ref_counted.h"

template <typename T>
struct escape<const Ref<T>&> {
    typedef T* type;
    _FORCE_INLINE_ static T* ret(T* t, das::Context *) {
        return t;
    }
};



#include "core/math/vector2.h"
#include "core/math/vector2i.h"
// TODO just bind Vector2i lol
template <>
struct escape<Vector2i> {
    typedef Vector2 type;
    _FORCE_INLINE_ static Vector2 ret(Vector2 t, das::Context *) {
        return t;
    }
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



// no const, return
template <typename R, typename CC, typename ...Args, R (CC::*func)(Args...) >
struct das_call_godot_member < R (CC::*)(Args...),  func> {
    static typename escape<R>::type invoke ( CC * THIS, typename escape<Args>::type... args, CTX_AT) {
        CHECK_IF_NULL(THIS)
        return escape<R>::ret( ((*THIS).*(func)) ( args... ) );
    }
};

//no const, no return

template <typename CC, typename ...Args, void (CC::*func)(Args...) >
struct das_call_godot_member < void (CC::*)(Args...),  func> {
    static void invoke ( CC * THIS, typename escape<Args>::type... args, CTX_AT) {
        CHECK_IF_NULL_VOID(THIS)
        ((*THIS).*(func)) ( args... );
    }
};

// const, return

template <typename R, typename CC, typename ...Args, R (CC::*func)(Args...) const>
struct das_call_godot_member < R (CC::*)(Args...) const,  func> {
    static typename escape<R>::type invoke ( const CC * THIS, typename escape<Args>::type... args, CTX_AT) {
        CHECK_IF_NULL(THIS)
        return escape<R>::ret( ((*THIS).*(func)) ( args... ), ctx);
    }
};

// const, no return

template <typename CC, typename ...Args, void (CC::*func)(Args...) const>
struct das_call_godot_member < void (CC::*)(Args...) const,  func> {
    static void invoke ( const CC * THIS, typename escape<Args>::type... args, CTX_AT) {
        CHECK_IF_NULL_VOID(THIS)
        ((*THIS).*(func)) ( args... );
    }
};

// static variant

template <typename R, typename ...Args, R (*func)(Args...) >
struct das_call_godot_static_member < R (*)(Args...), func> {
    static typename escape<R>::type invoke (typename escape<Args>::type... args, CTX_AT) {
        return escape<R>::ret( (*func) ( args... ), ctx );
    }
};

#endif // GODOT_FUNCTIONS_WRAPPER_H