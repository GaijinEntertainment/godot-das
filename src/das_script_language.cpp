#include "das_script_language.h"

#include "init_daslang.h"

#include "core/config/engine.h"
#include "editor/editor_settings.h"
#include "core/config/project_settings.h"
#include "core/os/os.h"

#ifdef DEV_ENABLED
#include "generate_bindings.h"
#endif

const char * simple_template =R""""(require godot

class _CLASS_
_TS_def _ready()
_TS__TS_print("It's ready!")

_TS_def _enter_tree()
_TS__TS_print("It's enter tree!")
)"""";

static const int DAS_TEMPLATES_ARRAY_SIZE = 1;

static const struct ScriptLanguage::ScriptTemplate DAS_TEMPLATES[DAS_TEMPLATES_ARRAY_SIZE] = {
	{ String("Node"), String("Default"),  String("Base template for Node with default Godot cycle methods"),  String(simple_template) },
};


DasScriptLanguage *DasScriptLanguage::singleton = nullptr;

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

void DasScriptLanguage::add_script(SelfList<DasScript> *p_script) {
    script_list.add(p_script);
}

/* LANGUAGE FUNCTIONS */

void DasScriptLanguage::init() {
#ifdef DEV_ENABLED
    // TODO make this in a better place (but after Main::setup2)
    if (OS::get_singleton()->get_cmdline_user_args().find("--bind-das")) {
        generate_godot_module_code();
        print_line("Code generation complete, now rebuild please");
        std::abort();
    }
#endif
    initialize_daslang();
}

void DasScriptLanguage::finish() {
    deinitialize_daslang();
}

/* EDITOR FUNCTIONS */

void DasScriptLanguage::get_reserved_words(List<String> *p_words) const {
    // copypasted table from https://dascript.org/doc/reference/language/lexical_structure.html#keywords
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
    return
        // branches
        p_keyword == "if" ||
        p_keyword == "elif" ||
        p_keyword == "else" ||
        // static branches
        p_keyword == "static_if" ||
        p_keyword == "static_elif" ||
        // cycles
        p_keyword == "for" ||
        p_keyword == "while" ||
        p_keyword == "continue" ||
        p_keyword == "break" ||
        // try
        p_keyword == "try" ||
        p_keyword == "finally" ||
        p_keyword == "expect" ||
        // function
        p_keyword == "yield" ||
        p_keyword == "goto" ||
        p_keyword == "return" ||
        p_keyword == "pass";
}

void DasScriptLanguage::get_comment_delimiters(List<String> *p_delimiters) const {
    p_delimiters->push_back("/* */"); // Block comment
	p_delimiters->push_back("//"); // Single-line comment
}

void DasScriptLanguage::get_string_delimiters(List<String> *p_delimiters) const {
    p_delimiters->push_back("\" \"");
}

Ref<Script> DasScriptLanguage::make_template(const String &p_template, const String &p_class_name, const String &p_base_class_name) const {
    Ref<DasScript> new_script;
	new_script.instantiate();
    String processed_template = p_template;

	processed_template = processed_template.replace("_BASE_", p_base_class_name)
								 .replace("_CLASS_", p_class_name) // TODO validate identifier
								 .replace("_TS_", _get_indentation());
	new_script->set_source_code(processed_template);
	return new_script;
}

Vector<ScriptLanguage::ScriptTemplate> DasScriptLanguage::get_built_in_templates(StringName p_object) {
	Vector<ScriptLanguage::ScriptTemplate> templates;
#ifdef TOOLS_ENABLED
	for (int i = 0; i < DAS_TEMPLATES_ARRAY_SIZE; i++) {
		if (DAS_TEMPLATES[i].inherit == p_object) {
			templates.append(DAS_TEMPLATES[i]);
		}
	}
#endif
	return templates;
}

das::ProgramPtr DasScriptLanguage::compile_script(const String& p_source, const String& p_path, das::FileAccessPtr p_access, das::TextPrinter& p_logs, das::ModuleGroup& p_libs) {
	CharString global_file_path_utf8 = ProjectSettings::get_singleton()->globalize_path(p_path).utf8();
    const char* path_data = global_file_path_utf8.get_data();

    // TODO inject `require godot` as well
    // now it doesn't work because it is not injected when including from other scripts
	CharString source_with_injection_utf8 = (p_source + "\noptions always_export_initializer = true\n").utf8();
	const char*  source_data = source_with_injection_utf8.get_data();
    size_t source_len = strlen(source_data);

    p_access->setFileInfo(path_data,  das::make_unique<das::TextFileInfo>(source_data, source_len, false));

    return das::compileDaScript(path_data, p_access, p_logs, p_libs);
}

bool DasScriptLanguage::validate(const String &p_script, const String &p_path, List<String> *r_functions, List<ScriptError> *r_errors, List<Warning> *r_warnings, HashSet<int> *r_safe_lines) const {
    auto fAccess = das::make_smart<das::FsFileAccess>();
	das::TextPrinter dummyLogs;
	das::ModuleGroup dummyLibGroup;

    auto program = compile_script(p_script, p_path, fAccess, dummyLogs, dummyLibGroup);
    if (program->failed() && r_errors) {
		for ( auto & err : program->errors ) {
            ScriptLanguage::ScriptError e;
            e.path = p_path;
            e.line = err.at.line;
            e.column = err.at.column;
            e.message = String(err.what.c_str());
            r_errors->push_back(e);
		}
		return false;
	}
    // TODO can simulation fail because of static errors??? add simulation if so
    // TODO fill other lists

    return true;
}

Script *DasScriptLanguage::create_script() const {
	return memnew(DasScript);
}

String DasScriptLanguage::_get_indentation() const {
    // Copypasted from GDScript
    // TODO only tabs and 4 spaces are actually supported automatically in das
#ifdef TOOLS_ENABLED
	if (Engine::get_singleton()->is_editor_hint()) {
		bool use_space_indentation = EDITOR_GET("text_editor/behavior/indent/type");

		if (use_space_indentation) {
			int indent_size = EDITOR_GET("text_editor/behavior/indent/size");
			return String(" ").repeat(indent_size);
		}
	}
#endif
	return "\t";
}

void DasScriptLanguage::reload_tool_script(const Ref<Script> &p_script, bool p_soft_reload) {
    SelfList<DasScript> *elem = script_list.first();
    while (elem) {
        elem->self()->reload(p_soft_reload);
        elem = elem->next();
    }
}

void DasScriptLanguage::get_recognized_extensions(List<String> *p_extensions) const {
    p_extensions->push_back("das");
}

bool DasScriptLanguage::handles_global_class_type(const String &p_type) const {
    return p_type == "DasScript";
}
