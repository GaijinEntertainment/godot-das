
#include "das_script_language.h"
#include "das_script.h"


DasScriptLanguage *DasScriptLanguage::singleton = nullptr;


String DasScriptLanguage::get_name() const {
    return "daScript";
}

void DasScriptLanguage::init() {
    // TODO
}

String DasScriptLanguage::get_type() const {
    return "DasScript";
}

String DasScriptLanguage::get_extension() const {
    return "das";
}

void DasScriptLanguage::finish() {
    // TODO
}

void DasScriptLanguage::get_reserved_words(List<String> *p_words) const {
    static const char *keywords[] = {
        "struct",      "class",       "let",       "def",       "while",    "if",
        "static_if",   "else",        "for",       "recover",   "true",     "false",
        "new",         "typeinfo",    "type",      "in",        "is",       "as",
        "elif",        "static_elif", "array",     "return",    "null",     "break",
        "try",         "options",     "table",     "expect",    "const",    "require",
        "operator",    "enum",        "finally",   "delete",    "deref",    "aka",
        "typedef",     "with",        "cast",      "override",  "abstract", "upcast",
        "iterator",    "var",         "addr",      "continue",  "where",    "pass",
        "reinterpret", "module",      "public",    "label",     "goto",     "implicit",
        "shared",      "private",     "smart_ptr", "generator", "yield",    "unsafe",
        "assume",      "explicit",    "sealed",    nullptr
	};

    const char **w = keywords;

	while (*w) {
		p_words->push_back(*w);
		w++;
	}
    // TODO add types?
}

bool DasScriptLanguage::is_control_flow_keyword(String p_keyword) const {
    // TODO
    return false;
}

void DasScriptLanguage::get_comment_delimiters(List<String> *p_delimiters) const {
    p_delimiters->push_back("/* */"); // Block comment
	p_delimiters->push_back("//"); // Single-line comment
}


void DasScriptLanguage::get_doc_comment_delimiters(List<String> *p_delimiters) const {
    // TODO
}

void DasScriptLanguage::get_string_delimiters(List<String> *p_delimiters) const {
    p_delimiters->push_back("\" \"");
}


Ref<Script> DasScriptLanguage::make_template(
		const String &p_template,
		const String &p_class_name,
		const String &p_base_class_name) const {
    // TODO
	String _template = String() + "TEST";

	Ref<DasScript> new_script;
	new_script.instantiate();
	new_script->set_source_code(_template);
	new_script->set_name(p_class_name);
	new_script->valid = true;

	return new_script;
}

Vector<ScriptLanguage::ScriptTemplate> DasScriptLanguage::get_built_in_templates(StringName p_object) {
    // TODO
	return Vector<ScriptTemplate>();
} 

bool DasScriptLanguage::is_using_templates() {
	return true;
}

bool DasScriptLanguage::validate(const String &p_script, const String &p_path, List<String> *r_functions, List<ScriptError> *r_errors, List<Warning> *r_warnings, HashSet<int> *r_safe_lines) const {
    // TODO
    return true;
}

Script *DasScriptLanguage::create_script() const {
	return memnew(DasScript);
}

bool DasScriptLanguage::supports_builtin_mode() const {
	return true;
}

int DasScriptLanguage::find_function(const String &p_function, const String &p_code) const {
    // TODO
	return -1;
}

String DasScriptLanguage::make_function(const String &p_class, const String &p_name, const PackedStringArray &p_args) const {
	// TODO required?
    return String();
}

void DasScriptLanguage::auto_indent_code(String &p_code, int p_from_line, int p_to_line) const {
    // TODO
}

void DasScriptLanguage::add_global_constant(const StringName &p_variable, const Variant &p_value) {
    // TODO
}


DasScriptLanguage::DasScriptLanguage() {
	ERR_FAIL_COND(this->singleton);
	this->singleton = this;
    // TODO
}

DasScriptLanguage::~DasScriptLanguage() {
    if (this->singleton == this) {
		this->singleton = nullptr;
	}
    // TODO
}

// one huge TODO
String DasScriptLanguage::debug_get_error() const { }
int DasScriptLanguage::debug_get_stack_level_count() const { }
int DasScriptLanguage::debug_get_stack_level_line(int p_level) const { }
String DasScriptLanguage::debug_get_stack_level_function(int p_level) const { }
String DasScriptLanguage::debug_get_stack_level_source(int p_level) const { }
void DasScriptLanguage::debug_get_stack_level_locals(int p_level, List<String> *p_locals, List<Variant> *p_values, int p_max_subitems, int p_max_depth) { }
void DasScriptLanguage::debug_get_stack_level_members(int p_level, List<String> *p_members, List<Variant> *p_values, int p_max_subitems, int p_max_depth) { }
// ScriptInstance *DasScriptLanguage::debug_get_stack_level_instance(int p_level) { return nullptr; }
void DasScriptLanguage::debug_get_globals(List<String> *p_globals, List<Variant> *p_values, int p_max_subitems, int p_max_depth) { }
String DasScriptLanguage::debug_parse_stack_level_expression(int p_level, const String &p_expression, int p_max_subitems, int p_max_depth) { }

void DasScriptLanguage::reload_all_scripts() { }
void DasScriptLanguage::reload_tool_script(const Ref<Script> &p_script, bool p_soft_reload) { }


void DasScriptLanguage::get_recognized_extensions(List<String> *p_extensions) const {
    p_extensions->push_back("das");
}
// TODO
void DasScriptLanguage::get_public_functions(List<MethodInfo> *p_functions) const { }
void DasScriptLanguage::get_public_constants(List<Pair<String, Variant>> *p_constants) const { }
void DasScriptLanguage::get_public_annotations(List<MethodInfo> *p_annotations) const { }

void DasScriptLanguage::profiling_start() { }
void DasScriptLanguage::profiling_stop() { }

int DasScriptLanguage::profiling_get_accumulated_data(ProfilingInfo *p_info_arr, int p_info_max) { }
int DasScriptLanguage::profiling_get_frame_data(ProfilingInfo *p_info_arr, int p_info_max) { }

bool DasScriptLanguage::handles_global_class_type(const String &p_type) const {
    return p_type == "DasScript";
}
String DasScriptLanguage::get_global_class_name(const String &p_path, String *r_base_type, String *r_icon_path) const {
    return String();
}
