#include "das_script_instance.h"
#include "das_script_language.h"
#include <daScript/daScript.h>

DasScriptInstance::~DasScriptInstance() {
	MutexLock lock(DasScriptLanguage::get_singleton()->mutex); // why?

	if (script.is_valid() && owner) {
		script->instances.erase(owner);
	}
}

bool DasScriptInstance::set(const StringName &p_name, const Variant &p_value) {
	// TODO
    return false;
}

bool DasScriptInstance::get(const StringName &p_name, Variant &r_ret) const {
	// TODO
    return false;
}

void DasScriptInstance::get_property_list(List<PropertyInfo> *p_properties) const {
    // TODO
}

Variant::Type DasScriptInstance::get_property_type(const StringName &p_name, bool *r_is_valid) const {
    // TODO
	return Variant::Type();
}

void DasScriptInstance::validate_property(PropertyInfo &p_property) const {
	// TODO
    return;
}

bool DasScriptInstance::property_can_revert(const StringName &p_name) const {
	// TODO
	return false;
}

bool DasScriptInstance::property_get_revert(const StringName &p_name, Variant &r_ret) const {
    // TODO
	return false;
}

void DasScriptInstance::get_property_state(List<Pair<StringName, Variant>> &state) {
    // TODO
}

void DasScriptInstance::get_method_list(List<MethodInfo> *p_list) const {
	// TODO
}

bool DasScriptInstance::has_method(const StringName &p_method) const {


	// TODO take inheritance into account
	return script.ptr()->has_method(p_method);
}

Variant DasScriptInstance::callp(const StringName &p_method, const Variant **p_args, int p_argcount, Callable::CallError &r_error) {
	das::ContextPtr ctx = script.ptr()->ctx;
	das::SimFunction *function = ctx->findFunction(String(p_method).utf8().get_data());
	if (!function) return Variant(); 
	// TODO pass args
	ctx->evalWithCatch(function, nullptr);
	return Variant();
}

void DasScriptInstance::notification(int p_notification, bool p_reversed) {
    // TODO
} 

String DasScriptInstance::to_string(bool *r_valid) {
	// TODO
	return String();
}

void DasScriptInstance::refcount_incremented() {
	// TODO
}

bool DasScriptInstance::refcount_decremented() {
    // TODO
	return true;
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
