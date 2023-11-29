#include "das_script_instance.h"
#include "das_script_language.h"
#include <daScript/daScript.h>

DasScriptInstance::~DasScriptInstance() {
	DasScriptLanguage::get_singleton()->acquire();

	if (script.is_valid() && owner) {
		script->erase_instance(owner);
	}
}

void DasScriptInstance::get_method_list(List<MethodInfo> *p_list) const {
	script.ptr()->get_script_method_list(p_list);
}

bool DasScriptInstance::has_method(const StringName &p_method) const {
	// TODO take inheritance into account (see notes)
	return script.ptr()->has_method(p_method);
}

Variant DasScriptInstance::callp(const StringName &p_method, const Variant **p_args, int p_argcount, Callable::CallError &r_error) {
	das::ContextPtr ctx = script->get_ctx();
	das::SimFunction *function = ctx->findFunction(String(p_method).utf8().get_data());
	if (!function) {
		r_error.error = Callable::CallError::CALL_ERROR_INVALID_METHOD;
		return Variant();
	}
	// TODO pass args
	ctx->evalWithCatch(function, nullptr);
	return Variant();
}

Ref<Script> DasScriptInstance::get_script() const {
	return script;
}

const Variant DasScriptInstance::get_rpc_config() const {
	return script->get_rpc_config();
}

ScriptLanguage *DasScriptInstance::get_language() {
	return DasScriptLanguage::get_singleton();
}
