#ifndef DAS_SCRIPT_LANGUAGE_H
#define DAS_SCRIPT_LANGUAGE_H

#include "das_script.h"


class DasScriptLanguage : public ScriptLanguage {
	static DasScriptLanguage *singleton;

	Mutex mutex;

	SelfList<DasScript>::List script_list;
public:
	DasScriptLanguage();
	~DasScriptLanguage();

	void add_script(SelfList<DasScript> *p_script);

	_FORCE_INLINE_ static DasScriptLanguage *get_singleton() { return singleton; }
	_FORCE_INLINE_ static MutexLock<Mutex> acquire_lock() { return MutexLock<Mutex>(singleton->mutex); }
	String get_name() const override { return "Daslang"; }

	/* LANGUAGE FUNCTIONS */
	void init() override;
	String get_type() const override { return "DasScript"; }
	String get_extension() const override { return "das"; }
	void finish() override;

	/* EDITOR FUNCTIONS */
	void get_reserved_words(List<String> *p_words) const override;
	bool is_control_flow_keyword(String p_keyword) const override;
	void get_comment_delimiters(List<String> *p_delimiters) const override;
	void get_doc_comment_delimiters(List<String> *p_delimiters) const override { /* TODO */ }
	void get_string_delimiters(List<String> *p_delimiters) const override;
	Ref<Script> make_template(const String &p_template, const String &p_class_name, const String &p_base_class_name) const override;
	Vector<ScriptLanguage::ScriptTemplate> get_built_in_templates(StringName p_object) override;
	bool is_using_templates() override { return true; }
	// TODO put it inside GodotContext maybe?
	static Variant call_function(das::SimFunction *func, das::Context *ctx, void* self, const char* name, const Variant **p_args, int p_argcount, Callable::CallError &r_error);
	static das::ProgramPtr compile_script(const String& p_source, const String& p_path, das::FileAccessPtr p_access, das::TextPrinter& p_logs, das::ModuleGroup& p_libs);
	bool validate(const String &p_script, const String &p_path = "", List<String> *r_functions = nullptr, List<ScriptError> *r_errors = nullptr, List<Warning> *r_warnings = nullptr, HashSet<int> *r_safe_lines = nullptr) const override;
	Script *create_script() const override;
#ifndef DISABLE_DEPRECATED
	virtual bool has_named_classes() const { return false; };
#endif
	bool supports_builtin_mode() const override { return true; }
	int find_function(const String &p_function, const String &p_code) const override { /* TODO */ return -1;}
	String make_function(const String &p_class, const String &p_name, const PackedStringArray &p_args) const override { /* TODO */ return String();}

	String _get_indentation() const;
	void auto_indent_code(String &p_code, int p_from_line, int p_to_line) const override { /* TODO */ }
	void add_global_constant(const StringName &p_variable, const Variant &p_value) override { /* TODO */ }

	/* DEBUGGER FUNCTIONS */
	String debug_get_error() const override { /* TODO*/ return String(); }
	int debug_get_stack_level_count() const override { /* TODO */ return 0; }
	int debug_get_stack_level_line(int p_level) const { /* TODO */ return 0;}
	String debug_get_stack_level_function(int p_level) const override {	/* TODO */ return String(); }
	String debug_get_stack_level_source(int p_level) const override { /* TODO */ return String();}
	void debug_get_stack_level_locals(int p_level, List<String> *p_locals, List<Variant> *p_values, int p_max_subitems = -1, int p_max_depth = -1) override { /* TODO */ }
	void debug_get_stack_level_members(int p_level, List<String> *p_members, List<Variant> *p_values, int p_max_subitems = -1, int p_max_depth = -1) override { /* TODO */ }
	ScriptInstance *debug_get_stack_level_instance(int p_level) override { /* TODO */ return nullptr; }
	void debug_get_globals(List<String> *p_globals, List<Variant> *p_values, int p_max_subitems = -1, int p_max_depth = -1) override { /* TODO */ }
	String debug_parse_stack_level_expression(int p_level, const String &p_expression, int p_max_subitems = -1, int p_max_depth = -1) override { /* TODO */ return String(); }
	Vector<StackInfo> debug_get_current_stack_info() override { /* TODO */ return Vector<StackInfo>(); }

	void reload_all_scripts() override { /* TODO */ }
	void reload_tool_script(const Ref<Script> &p_script, bool p_soft_reload) override;

	/* LOADER FUNCTIONS */
	void get_recognized_extensions(List<String> *p_extensions) const override;
	void get_public_functions(List<MethodInfo> *p_functions) const override { /* TODO */ }
	void get_public_constants(List<Pair<String, Variant>> *p_constants) const override{ /* TODO */ }
	void get_public_annotations(List<MethodInfo> *p_annotations) const override{ /* TODO */ }

	/* PROFILING FUNCTIONS */
	void profiling_start() override { /* TODO */ }
	void profiling_stop() override { /* TODO */ }

	int profiling_get_accumulated_data(ProfilingInfo *p_info_arr, int p_info_max) override { /* TODO */ }
	int profiling_get_frame_data(ProfilingInfo *p_info_arr, int p_info_max) override { /* TODO */ }

	void frame() override { /* TODO */ }

	/* GLOBAL CLASSES */
	bool handles_global_class_type(const String &p_type) const override;
	String get_global_class_name(const String &p_path, String *r_base_type = nullptr, String *r_icon_path = nullptr) const override { /* TODO */ return String(); }

};

#endif // DAS_SCRIPT_LANGUAGE_H
