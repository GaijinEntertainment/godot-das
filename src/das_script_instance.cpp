#include "das_script_instance.h"

#include "das_script_language.h"


DasScriptInstance::~DasScriptInstance() {
	script->free_instance(this);
}

void DasScriptInstance::set_script(Ref<DasScript> p_script) {
	script = p_script;
}

void DasScriptInstance::set_owner(Object *p_owner) {
	owner = p_owner;
}

void DasScriptInstance::set_class_ptr(char* p_class_ptr) {
	class_ptr = p_class_ptr;
}

char* DasScriptInstance::get_class_ptr() {
	return class_ptr;
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
	int offset = script->get_func_offset(p_method);
	if (offset == INVALID_OFFSET) {
		r_error.error = Callable::CallError::CALL_ERROR_INVALID_METHOD;
		return Variant();
	}
	auto func_ptr = reinterpret_cast<das::Func*>(class_ptr + offset)->PTR;

	auto ret = DasScriptLanguage::call_function(func_ptr, ctx.get(), class_ptr, String(p_method).utf8().get_data(), p_args, p_argcount, r_error);
	return ret;
}

Ref<Script> DasScriptInstance::get_script() const {
	return script;
}

Ref<DasScript> DasScriptInstance::get_das_script() const {
	return script;
}


const Variant DasScriptInstance::get_rpc_config() const {
	return script->get_rpc_config();
}

ScriptLanguage *DasScriptInstance::get_language() {
	return DasScriptLanguage::get_singleton();
}
