#ifndef DAS_SCRIPT_H
#define DAS_SCRIPT_H

#include <daScript/daScript.h>
#include "core/object/script_language.h"

class DasScript;

#include "das_script_instance.h"


static constexpr int INVALID_OFFSET = -1;

class DasScript : public Script {
    GDCLASS(DasScript, Script);

    bool valid = false;
	bool tool = false;

    HashSet<Object *> instances{};
    String source{};
	das::StructurePtr main_structure;
	std::vector<std::pair<const char*, size_t>> signals;

	// These four objects hold complete state of one script
	das::ContextPtr ctx;
	das::ProgramPtr program;
	std::unique_ptr<das::ModuleGroup> lib_group;
	das::FileAccessPtr file_access; // not sure if needed, but otherwise file infos die

	SelfList<DasScript> script_list;
#ifdef TOOLS_ENABLED
	// TODO reload stored as in _update_exports
	HashSet<PlaceHolderScriptInstance *> placeholders{};
#endif
	// for quick finding by Godot
	HashMap<StringName, size_t> func_offsets{};
public:
	DasScript();
	~DasScript();

	das::ContextPtr get_ctx() const;
	void free_instance(DasScriptInstance *p_instance);
	size_t get_func_offset(const StringName &p_func) const;
	size_t get_field_offset(const char *p_field) const;

	const char* get_class_name() const;

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

	bool is_placeholder_fallback_enabled() const override { /* TODO otherwise crash on load with errors in script */ return true; }

	const Variant get_rpc_config() const override { /* TODO */ return Dictionary(); }

	Error load_source_code(const String &p_path);
protected:
	void _print_errors(das::ProgramPtr p_program, const char* p_msg_begin) const;
#ifdef TOOLS_ENABLED
	void _placeholder_erased(PlaceHolderScriptInstance *p_placeholder) override;
#endif

};

#endif // DAS_SCRIPT_H
