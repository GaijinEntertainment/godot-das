#include "godot_module.h"

#include "godot_types_gen.h"
#include "godot_types_extra.h"

#include "godot_functions_wrapper.h"

#include "das_script_instance.h"

// Non-generatable functions go here

Resource* _load(Object* obj, const char* p_path, CTX_AT) {
    CHECK_IF_NULL_MSG(obj, "cannot bind resource to null object");
    auto script_instance = dynamic_cast<DasScriptInstance*>(obj->get_script_instance());
    CHECK_IF_NULL_MSG(script_instance, "cannot bind resource to non-das object");
    Ref<Resource> res = ResourceLoader::load(p_path);
    if (res.is_valid()) {
        // tmp solution
        // TODO support das::smart_ptr<Resource> or Ref<Resource> in das
        // so ref counting is handled by das
        script_instance->bind_resource(res);
    }
    return res.ptr();
}

void connect_signal_to_func_str(Object* owner, const char* signal_name, Object* peer, const char* func_name, CTX_AT) {
    CHECK_IF_NULL_VOID(owner)
    CHECK_IF_NULL_VOID_MSG(peer, "cannot connect null to signal")

    // TODO check if `func_name` can be called on `peer`
    Callable func_str_to_call(peer, func_name);
    owner->connect(signal_name, func_str_to_call, 0); // TODO flags
}

// The same as below I guess

void connect_signal_to_func_ptr(Object* owner, const char* signal_name, das::Func func, CTX_AT) {
    CHECK_IF_NULL_VOID(owner)
    // TODO understand how to call function with self
    ctx->throw_error_at(at, "not implemented");
}

// class DasLambdaCallable : public CallableCustom {
//     das::Lambda lambda;
// public:
// 	uint32_t hash() const override;
// 	String get_as_text() const override;
// 	CompareEqualFunc get_compare_equal_func() const override;
// 	CompareLessFunc get_compare_less_func() const override;
// 	bool is_valid() const override;
//     ObjectID get_object() const override;
// 	void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const override;
// };

void connect_signal_to_lambda(Object* owner, const char* signal_name, das::Lambda lambda, CTX_AT) {
    CHECK_IF_NULL_VOID(owner)
    // TODO understand how to call lambda
    ctx->throw_error_at(at, "not implemented");
}

void emit_signal(Object* owner, const char* signal_name, CTX_AT) {
    CHECK_IF_NULL_VOID(owner)
    owner->emit_signal(signal_name);
}


void Module_Godot::bind_functions_extra(das::ModuleLibrary & lib) {
    das::addExtern<DAS_BIND_FUN(_load)>(*this, lib, "load", das::SideEffects::modifyExternal, "_load");

    // Color
    // TODO properly bind simnode ctor
    using _Color_named = DAS_CALL_GODOT_STATIC_MEMBER(*static_cast<Color (*)(const String &)>(&Color::named));
    das::addExtern<DAS_BIND_FUN(_Color_named::invoke), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "Color`named", das::SideEffects::modifyExternal, DAS_CALL_GODOT_STATIC_MEMBER_CPP(Color::named));

    das::addExtern<DAS_BIND_FUN(connect_signal_to_func_str)>(*this, lib, "connect_signal_to_func_str", das::SideEffects::modifyArgument, "connect_signal_to_func_str")
    ->args({"owner", "signal_name", "peer", "func_name", "__ctx__", "__at__"});
    das::addExtern<DAS_BIND_FUN(connect_signal_to_func_ptr)>(*this, lib, "connect_signal_to_func_ptr", das::SideEffects::modifyArgument, "connect_signal_to_func_ptr")
    ->args({"owner", "signal_name", "func", "__ctx__", "__at__"});
    das::addExtern<DAS_BIND_FUN(connect_signal_to_lambda)>(*this, lib, "connect_signal_to_lambda", das::SideEffects::modifyArgument, "connect_signal_to_lambda")
    ->args({"owner", "signal_name", "lambda", "__ctx__", "__at__"});
    das::addExtern<DAS_BIND_FUN(emit_signal)>(*this, lib, "emit_signal", das::SideEffects::modifyArgument, "emit_signal")
    ->args({"owner", "signal_name", "__ctx__", "__at__"});

    BIND_GODOT_SINGLETON_MEMBER(Engine, get_frames_per_second)
    BIND_GODOT_SINGLETON_MEMBER(Input, is_action_just_pressed, "action", "exact_match")
    SET_DEFAULT_ARG(Input, is_action_just_pressed, 1, false)

    BIND_GODOT_BUILTIN_FUNCTION(VariantUtilityFunctions, randf_range)

}
