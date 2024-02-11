#include "godot_module.h"
#include "godot_types_gen.h"

#include "das_script_instance.h"
#include "das_script_language.h"

#include "godot_functions_macro.h"


// Type manipulation

char* _get_das_type(const Object* obj, const char* name) {
    if (obj == nullptr) {
        return nullptr;
    }
    DasScriptInstance* instance = dynamic_cast<DasScriptInstance*>(obj->get_script_instance());
    if (instance == nullptr || strcmp(instance->get_das_script()->get_class_name(), name) != 0) {
        // two cases: either the object does not have a das script instance, or the das script instance is not of the correct type
        return nullptr;
    }
    return instance->get_class_ptr();
}

bool _check_das_type(const Object* obj, const char* name) {
    if (obj == nullptr) {
        return false;
    }
    DasScriptInstance* instance = dynamic_cast<DasScriptInstance*>(obj->get_script_instance());
    if (instance == nullptr || strcmp(instance->get_das_script()->get_class_name(), name) != 0) {
        return false;
    }
    return true;
}

void* _promote_to_das_type(Object* obj, const char* script_path) {
    Ref<DasScript> das_script = ResourceLoader::load(script_path);
    if (das_script.is_null()) {
        // should never be here
        CRASH_NOW_MSG(String("File ") + String(script_path) + String(" not found"));
    }
    obj->set_script(das_script);
    DasScriptInstance* instance = reinterpret_cast<DasScriptInstance*>(obj->get_script_instance());
    return instance->get_class_ptr();
}

// Signals manipulation

class DasLambdaCallable : public CallableCustom {
    das::Lambda lambda;
    das::Context* context;
    // TODO maybe line info that in passed to `connect` survives and I don't need to copy it?
    das::LineInfo lineinfo;

    static bool compare_equal(const CallableCustom *p_a, const CallableCustom *p_b) { return p_a == p_b; }
    static bool compare_less(const CallableCustom *p_a, const CallableCustom *p_b) { return p_a < p_b; }
public:
	uint32_t hash() const override {return reinterpret_cast<uint64_t>(lambda.capture); } // ??
	String get_as_text() const override { return "das lambda"; } // ??
	CompareEqualFunc get_compare_equal_func() const override { return compare_equal; } // ??
	CompareLessFunc get_compare_less_func() const override { return compare_less; } // ??
    ObjectID get_object() const override { return ObjectID(); } // ??

	bool is_valid() const override {
        // TODO is overkill?
        bool context_dead = context->category.value & static_cast<uint32_t>(das::ContextCategory::dead);
        bool lambda_not_null = lambda.capture && *reinterpret_cast<das::SimFunction **>(lambda.capture);
        return !context_dead && lambda_not_null;
    }

	void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const override {
        das::LineInfo lineinfo_copy = lineinfo;

        das::SimFunction ** fnpp = (das::SimFunction **) lambda.capture;
        if (!fnpp) context->throw_error_at(&lineinfo_copy, "invoke null lambda");
        das::SimFunction * simFunc = *fnpp;
        if (!simFunc) context->throw_error_at(&lineinfo_copy, "invoke null function");

        r_return_value = DasScriptLanguage::call_function(simFunc, context, lambda.capture, "das lambda", p_arguments, p_argcount, r_call_error);
    }

    DasLambdaCallable(das::Lambda lambda, das::Context* context, das::LineInfo* lineinfo)
        : lambda(lambda), context(context), lineinfo(*lineinfo) {}

    virtual ~DasLambdaCallable() {
        // TODO understand how to clean lambdas properly
        // das::das_delete<das::Lambda>::clear(context, lambda);
    }
};

void connect_signal_to_func_str(Object* owner, const char* signal_name, Object* peer, const char* func_name, CTX_AT) {
    CHECK_IF_NULL_VOID(owner)
    CHECK_IF_NULL_VOID_MSG(peer, "cannot connect null to signal")

    // TODO check if `func_name` can be called on `peer`
    Callable func_str_to_call(peer, func_name);
    owner->connect(signal_name, func_str_to_call, 0); // TODO flags
}

void connect_signal_to_lambda(Object* owner, const char* signal_name, das::Lambda lambda, CTX_AT) {
    CHECK_IF_NULL_VOID(owner)

    DasLambdaCallable *lambda_wrapped = memnew(DasLambdaCallable(lambda, ctx, at));
    Callable lambda_to_call(lambda_wrapped);
    owner->connect(signal_name, lambda_to_call, 0); // TODO flags
}

void emit_signal(Object* owner, const char* signal_name, CTX_AT) {
    CHECK_IF_NULL_VOID(owner)
    owner->emit_signal(signal_name);
}

void Module_Godot::bind_utils(das::ModuleLibrary & lib) {
    das::addExtern<DAS_BIND_FUN(_get_das_type)>(*this, lib, "_get_das_type", das::SideEffects::none, "_get_das_type");
    das::addExtern<DAS_BIND_FUN(_check_das_type)>(*this, lib, "_check_das_type", das::SideEffects::none, "_check_das_type");
    das::addExtern<DAS_BIND_FUN(_promote_to_das_type)>(*this, lib, "_promote_to_das_type", das::SideEffects::modifyArgument, "_promote_to_das_type");

    das::addExtern<DAS_BIND_FUN(connect_signal_to_func_str)>(*this, lib, "connect_signal_to_func_str", das::SideEffects::modifyArgument, "connect_signal_to_func_str")
    ->args({"owner", "signal_name", "peer", "func_name", "__ctx__", "__at__"});
    das::addExtern<DAS_BIND_FUN(connect_signal_to_lambda)>(*this, lib, "connect_signal_to_lambda", das::SideEffects::modifyArgument, "connect_signal_to_lambda")
    ->args({"owner", "signal_name", "lambda", "__ctx__", "__at__"});
    das::addExtern<DAS_BIND_FUN(emit_signal)>(*this, lib, "emit_signal", das::SideEffects::modifyArgument, "emit_signal")
    ->args({"owner", "signal_name", "__ctx__", "__at__"});
}

REGISTER_MODULE(Module_Godot);
