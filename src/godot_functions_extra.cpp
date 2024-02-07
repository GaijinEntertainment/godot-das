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

void connect_signal(Object* obj, const char* name, Object* target, const char* func, CTX_AT) {
    CHECK_IF_NULL_VOID(obj)
    CHECK_IF_NULL_VOID_MSG(target, "cannot connect null to signal")

    // TODO check if `func` can be called on `target`
    Callable c(target, func);
    obj->connect(name, c, 0); // TODO flags
}

void emit_signal(Object* obj, const char* name, CTX_AT) {
    CHECK_IF_NULL_VOID(obj)
    obj->emit_signal(name);
}


void Module_Godot::bind_functions_extra(das::ModuleLibrary & lib) {
    das::addExtern<DAS_BIND_FUN(_load)>(*this, lib, "load", das::SideEffects::modifyExternal, "_load");

    // Color
    // TODO properly bind simnode ctor
    using _Color_named = DAS_CALL_GODOT_STATIC_MEMBER(*static_cast<Color (*)(const String &)>(&Color::named));
    das::addExtern<DAS_BIND_FUN(_Color_named::invoke), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "Color`named", das::SideEffects::modifyExternal, DAS_CALL_GODOT_STATIC_MEMBER_CPP(Color::named));

    das::addExtern<DAS_BIND_FUN(connect_signal)>(*this, lib, "connect_signal", das::SideEffects::modifyArgument, "connect_signal")
    ->args({"emitter", "signal_name", "peer", "func_name", "__ctx__", "__at__"});
    das::addExtern<DAS_BIND_FUN(emit_signal)>(*this, lib, "emit_signal", das::SideEffects::modifyArgument, "emit_signal")
    ->args({"emmiter", "signal_name", "__ctx__", "__at__"});

    BIND_GODOT_SINGLETON_MEMBER(Engine, get_frames_per_second)
    BIND_GODOT_SINGLETON_MEMBER(Input, is_action_just_pressed, "action", "exact_match")
    SET_DEFAULT_ARG(Input, is_action_just_pressed, 1, false)

    BIND_GODOT_BUILTIN_FUNCTION(VariantUtilityFunctions, randf_range)

}
