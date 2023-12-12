#include "das_script_instance.h"

#include "das_script_language.h"


DasScriptInstance::~DasScriptInstance() {
	{
		DasScriptLanguage::get_singleton()->acquire_lock();

		if (script.is_valid() && owner) {
			script->erase_instance(owner);
		}
	}
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


void DasScriptInstance::get_method_list(List<MethodInfo> *p_list) const {
	script.ptr()->get_script_method_list(p_list);
}

bool DasScriptInstance::has_method(const StringName &p_method) const {
	// TODO take inheritance into account (see notes)
	return script.ptr()->has_method(p_method);
}

Variant DasScriptInstance::callp(const StringName &p_method, const Variant **p_args, int p_argcount, Callable::CallError &r_error) {
	das::ContextPtr ctx = script->get_ctx();
	int offset = script->get_field_offset(p_method);
	if (offset == INVALID_OFFSET) {
		r_error.error = Callable::CallError::CALL_ERROR_INVALID_METHOD;
		return Variant();
	}
	auto func = *reinterpret_cast<das::Func*>(class_ptr + offset);

	std::vector<vec4f> arguments;
	arguments.push_back(das::cast<void*>::from(class_ptr));
	for (int i = 0; i < p_argcount; i++) {
		switch (p_args[i]->get_type()) {
			case Variant::Type::FLOAT:
				arguments.push_back(das::cast<float>::from(p_args[i]->operator float()));
				break;
			default:
				continue;
		}
	}
	ctx->tryRestartAndLock();
	ctx->evalWithCatch(func.PTR, arguments.data());
	ctx->unlock();
	if (const char* exception = ctx->getException()) {
		_err_print_error(String(p_method).utf8().get_data(), ctx->exceptionAt.fileInfo->name.c_str(), ctx->exceptionAt.line, exception, false, ERR_HANDLER_SCRIPT);
	}
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
