#include "das_script_language.h"
#include "das_script.h"
#include "init_dascript.h"


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

Ref<Script> DasScriptLanguage::make_template(
		const String &p_template,
		const String &p_class_name,
		const String &p_base_class_name) const {
    // TODO
	String _template = String() + R""""(
[export]
def _ready()
    print("Hello World!")
)"""";

	Ref<DasScript> new_script;
	new_script.instantiate();
	new_script->set_source_code(_template);
	return new_script;
}

bool DasScriptLanguage::validate(const String &p_script, const String &p_path, List<String> *r_functions, List<ScriptError> *r_errors, List<Warning> *r_warnings, HashSet<int> *r_safe_lines) const {
    // TODO important
    return true;
}

Script *DasScriptLanguage::create_script() const {
	return memnew(DasScript);
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
