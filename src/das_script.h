
#ifndef DAS_SCRIPT_H
#define DAS_SCRIPT_H

#include "core/object/script_language.h"
#include "daScript/daScript.h"


class DasScript : public Script {
    GDCLASS(DasScript, Script);

	friend class DasScriptLanguage;
	friend class DasScriptInstance;

    bool valid = false;
	bool tool = true; // tmp solution to make debugging easier

    HashSet<Object *> instances{};
    String source{};
	String path{};

	das::ContextPtr ctx;
	das::ProgramPtr program;

    Dictionary rpc_config;
    
#ifdef TOOLS_ENABLED
	bool placeholder_fallback_enabled{};
	// TODO reload stored as in _update_exports
	HashSet<PlaceHolderScriptInstance *> placeholders{};
#endif

public:
	DasScript();
	~DasScript();

	bool can_instantiate() const override;

	Ref<Script> get_base_script() const override;
	 StringName get_global_name() const override;

	bool inherits_script(const Ref<Script> &p_script) const override;

	StringName get_instance_base_type() const override;
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
	Vector<DocData::ClassDoc> get_documentation() const override;
	String get_class_icon_path() const override;
	PropertyInfo get_class_category() const override;
#endif

	bool has_method(const StringName &p_method) const override;
	MethodInfo get_method_info(const StringName &p_method) const override;

	bool is_tool() const override;
	bool is_valid() const override;
	bool is_abstract() const override;

	ScriptLanguage *get_language() const override;

	bool has_script_signal(const StringName &p_signal) const override;
	void get_script_signal_list(List<MethodInfo> *r_signals) const override;

	bool get_property_default_value(const StringName &p_property, Variant &r_value) const override;

	void update_exports() override;
	void get_script_method_list(List<MethodInfo> *p_list) const override;
	void get_script_property_list(List<PropertyInfo> *p_list) const override;

	int get_member_line(const StringName &p_member) const override;

	void get_constants(HashMap<StringName, Variant> *p_constants) override;
	void get_members(HashSet<StringName> *p_members) override;
#ifdef TOOLS_ENABLED
    bool is_placeholder_fallback_enabled() const override;
#endif
	const Variant get_rpc_config() const override;

	Variant _new(const Variant **p_args, int p_argcount, Callable::CallError &r_error);

	Error load_source_code(const String &p_path);
protected:
	static void _bind_methods();

#ifdef TOOLS_ENABLED
	void _placeholder_erased(PlaceHolderScriptInstance *p_placeholder) override;
#endif


};

#endif // DAS_SCRIPT_H
