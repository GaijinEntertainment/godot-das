#include "register_types.h"
#include "tutorial00.h"
#include <iostream>

void initialize_dascript_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
			return;
	}
	tutorial();
}

void uninitialize_dascript_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
			return;
	}
}