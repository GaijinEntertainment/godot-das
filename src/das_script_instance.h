#ifndef DAS_SCRIPT_INSTANCE
#define DAS_SCRIPT_INSTANCE

#include "das_script.h"


class DasScriptInstance : public ScriptInstance {
	friend class DasScript;

	Object *owner = nullptr;
	Ref<DasScript> script;
public:
	~DasScriptInstance();

	bool set(const StringName &p_name, const Variant &p_value) override;
	bool get(const StringName &p_name, Variant &r_ret) const override;
	void get_property_list(List<PropertyInfo> *p_properties) const override;
	Variant::Type get_property_type(const StringName &p_name, bool *r_is_valid = nullptr) const override;
	void validate_property(PropertyInfo &p_property) const ;//override;

	bool property_can_revert(const StringName &p_name) const override;
	bool property_get_revert(const StringName &p_name, Variant &r_ret) const override;

	Object *get_owner() override { return owner; }
	void get_property_state(List<Pair<StringName, Variant>> &state) override;

	void get_method_list(List<MethodInfo> *p_list) const override;
	bool has_method(const StringName &p_method) const override;

	Variant callp(const StringName &p_method, const Variant **p_args, int p_argcount, Callable::CallError &r_error) override;

    // implement if language supports const functions
	//virtual Variant call_const(const StringName &p_method, const Variant **p_args, int p_argcount, Callable::CallError &r_error); 
	void notification(int p_notification, bool p_reversed = false) override;
	String to_string(bool *r_valid) override;

	void refcount_incremented() override;
	bool refcount_decremented() override;

	Ref<Script> get_script() const override;

	// is_placeholder, property_set_fallback and property_get_fallback
	// are only overridden by PlaceHolderScriptInstance
	// and should stay default by an actual ScriptInstance

	const Variant get_rpc_config() const override;

	ScriptLanguage *get_language() override;
};

#endif