#include "register_types.h"
#include "tutorial00.h"
#include <iostream>
#include "dascript_script_language.h"

DaScriptLanguage *script_language_das = nullptr;



void initialize_dascript_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS) {
		script_language_das = memnew(DaScriptLanguage);
		ScriptServer::register_language(script_language_das);
	}
}

void uninitialize_dascript_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS) {
		
	}
}