#ifndef DAS_SCRIPT_H
#define DAS_SCRIPT_H

#include <core/object/script_language.h>

#include <daScript/daScript.h>


class DasScript : public Script {
    GDCLASS(DasScript, Script);

    bool valid = false;
	bool tool = true; // tmp solution to make debugging easier

    HashSet<Object *> instances{};
    String source{};
	std::string script_name;
	das::StructurePtr main_structure;
	das::SimFunction *struct_ctor = nullptr;

	das::ContextPtr ctx;
	das::ProgramPtr program;
	SelfList<DasScript> script_list;
#ifdef TOOLS_ENABLED
	// TODO reload stored as in _update_exports
	HashSet<PlaceHolderScriptInstance *> placeholders{};
#endif

public:
	DasScript();
	~DasScript();

	das::ContextPtr get_ctx();
	void erase_instance(Object *p_owner);
	int get_func_offset(const StringName &p_func);

	bool can_instantiate() const override;

	Ref<Script> get_base_script() const override { /* TODO */ return Ref<Script>{}; }
	 StringName get_global_name() const override { /* TODO */ return StringName{}; }

	bool inherits_script(const Ref<Script> &p_script) const override { /* TODO */ return false;}

	StringName get_instance_base_type() const override { /* TODO */ return StringName(); }
	ScriptInstance *instance_create(Object *p_this) override;
#ifdef TOOLS_ENABLED
	PlaceHolderScriptInstance *placeholder_instance_create(Object *p_this) override;
#endif
	bool instance_has(const Object *p_this) const override;

	bool has_source_code() const override;
	String get_source_code() const override;
	void set_source_code(const String &p_code) override;
	Error reload(bool p_keep_state = false) override;

#ifdef TOOLS_ENABLED
	Vector<DocData::ClassDoc> get_documentation() const override { /* TODO */ return Vector<DocData::ClassDoc>{}; }
	String get_class_icon_path() const override { /* TODO */ return String(); }
#endif

	bool has_method(const StringName &p_method) const override;
	MethodInfo get_method_info(const StringName &p_method) const override;

	bool is_tool() const override;
	bool is_valid() const override;
	bool is_abstract() const override { /* TODO? */ return false; }

	ScriptLanguage *get_language() const override;

	bool has_script_signal(const StringName &p_signal) const override { /* TODO */ return false; }
	void get_script_signal_list(List<MethodInfo> *r_signals) const override { /* TODO */ }

	bool get_property_default_value(const StringName &p_property, Variant &r_value) const override;

	void get_script_method_list(List<MethodInfo> *p_list) const override { /* TODO */ }
	void get_script_property_list(List<PropertyInfo> *p_list) const override { /* TODO */ }

	const Variant get_rpc_config() const override { /* TODO */ return Dictionary(); }

	Variant _new(const Variant **p_args, int p_argcount, Callable::CallError &r_error);

	Error load_source_code(const String &p_path);
protected:
	static void _bind_methods();

#ifdef TOOLS_ENABLED
	void _placeholder_erased(PlaceHolderScriptInstance *p_placeholder) override;
#endif

};

#endif // DAS_SCRIPT_H
