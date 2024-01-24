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

void Module_Godot::bind_functions_extra(das::ModuleLibrary & lib) {
    das::addExtern<DAS_BIND_FUN(_load)>(*this, lib, "load", das::SideEffects::modifyExternal, "_load");

    // Color
    // TODO properly bind simnode ctor
    using _Color_named = DAS_CALL_GODOT_STATIC_MEMBER(*static_cast<Color (*)(const String &)>(&Color::named));
    das::addExtern<DAS_BIND_FUN(_Color_named::invoke), das::SimNode_ExtFuncCallAndCopyOrMove>(*this, lib, "Color`named", das::SideEffects::modifyExternal, DAS_CALL_GODOT_STATIC_MEMBER_CPP(Color::named));

    BIND_GODOT_SINGLETON_MEMBER(Engine, get_frames_per_second)
    BIND_GODOT_BUILTIN_FUNCTION(VariantUtilityFunctions, randf_range)

	using _Node_add_child = DAS_CALL_GODOT_MEMBER(Node::add_child);
	auto func = das::addExtern<DAS_BIND_FUN(_Node_add_child::invoke)>(*this, lib, "add_child", _Node_add_child::effects, DAS_CALL_GODOT_MEMBER_CPP(Node::add_child));
    func->args({"this", "child","force_readable_name", "internal", "ctx", "at"})
		->arg_init(2, das::make_smart<das::ExprConstBool>(false))
		->arg_init(3, das::make_smart<das::ExprConstEnumeration>(Node::InternalMode::INTERNAL_MODE_DISABLED, das::typeFactory<Node::InternalMode>::make(lib)));

	using _Node_find_child = DAS_CALL_GODOT_MEMBER(Node::find_child);
	das::addExtern<DAS_BIND_FUN(_Node_find_child::invoke)>(*this, lib, "find_child", _Node_find_child::effects, DAS_CALL_GODOT_MEMBER_CPP(Node::find_child))->
    args({"this", "pattern", "recursive", "owned", "ctx", "at"})->
    arg_init(2, das::make_smart<das::ExprConstBool>(false))->
    arg_init(3, das::make_smart<das::ExprConstBool>(false));
}
