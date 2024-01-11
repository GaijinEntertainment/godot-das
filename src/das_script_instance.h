#ifndef DAS_SCRIPT_INSTANCE
#define DAS_SCRIPT_INSTANCE

#include "das_script.h"


class DasScriptInstance : public ScriptInstance {
	Object *owner = nullptr;
	Ref<DasScript> script;
	Vector<Ref<Resource>> bound_resources; // tmp solution

	char* class_ptr = nullptr;
public:
	~DasScriptInstance();

	// DasScriptInstance-specific methods
	void set_script(Ref<DasScript> p_script);
	void set_owner(Object *p_owner);
	void set_class_ptr(char* p_class_ptr);
	char* get_class_ptr();
	void bind_resource(Ref<Resource> p_resource);

	// ScriptInstance methods
	bool set(const StringName &p_name, const Variant &p_value) override { /* TODO */ return false; }
	bool get(const StringName &p_name, Variant &r_ret) const override { /* TODO */ return false; }
	void get_property_list(List<PropertyInfo> *p_properties) const override { /* TODO */ }
	Variant::Type get_property_type(const StringName &p_name, bool *r_is_valid = nullptr) const override { /* TODO */ return Variant::Type(); }
	void validate_property(PropertyInfo &p_property) const override { /* TODO */ }

	bool property_can_revert(const StringName &p_name) const override { /* TODO */ return false; }
	bool property_get_revert(const StringName &p_name, Variant &r_ret) const override { /* TODO */ return false; }

	Object *get_owner() override { return owner; }
	void get_property_state(List<Pair<StringName, Variant>> &state) override { /* TODO */ }

	void get_method_list(List<MethodInfo> *p_list) const override;
	bool has_method(const StringName &p_method) const override;

	Variant callp(const StringName &p_method, const Variant **p_args, int p_argcount, Callable::CallError &r_error) override;

	void notification(int p_notification, bool p_reversed = false) override { /* TODO */ }

	Ref<Script> get_script() const override;
	Ref<DasScript> get_das_script() const;

	const Variant get_rpc_config() const override;

	ScriptLanguage *get_language() override;
};

#endif