#include "das_script_language.h"

#include "init_dascript.h"
#include "das_templates.h"

#include <core/config/engine.h>
#include <editor/editor_settings.h>

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
    das::setDasRoot(GODOT_DAS_MODULE_PATH"/daScript");
    initialize_dascript();
}

void DasScriptLanguage::finish() {
    deinitialize_dascript();
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

bool DasScriptLanguage::validate(const String &p_script, const String &p_path, List<String> *r_functions, List<ScriptError> *r_errors, List<Warning> *r_warnings, HashSet<int> *r_safe_lines) const {
    static const char* DUMMY_FILE = "dummy.das";

    auto fAccess = das::make_smart<das::FsFileAccess>();
	auto source_utf8 = p_script.utf8();

	auto source_data = source_utf8.get_data();
	auto source_len = uint32_t(strlen(source_data));
    auto fileInfo = das::make_unique<das::TextFileInfo>(source_data, source_len, false);
    fAccess->setFileInfo(DUMMY_FILE, das::move(fileInfo));

	das::TextPrinter dummyLogs;
	das::ModuleGroup dummyLibGroup;

    auto program = das::compileDaScript(DUMMY_FILE, fAccess, dummyLogs, dummyLibGroup);
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
