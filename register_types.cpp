#include "register_types.h"

#include "das_script_language.h"
#include "das_script.h"
#include "das_resource_io.h"

DasScriptLanguage *das_script_language = nullptr;
Ref<DasResourceFormatLoader> das_resource_loader;
Ref<DasResourceFormatSaver> das_resource_saver;


void initialize_daslang_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS) {
		GDREGISTER_CLASS(DasScript);

		das_script_language = memnew(DasScriptLanguage);
		ScriptServer::register_language(das_script_language);

		das_resource_loader.instantiate();
		ResourceLoader::add_resource_format_loader(das_resource_loader);

		das_resource_saver.instantiate();
		ResourceSaver::add_resource_format_saver(das_resource_saver);
	}
}

void uninitialize_daslang_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS) {
		ScriptServer::unregister_language(das_script_language);

		if (das_script_language) {
			memdelete(das_script_language);
		}

		ResourceLoader::remove_resource_format_loader(das_resource_loader);
		das_resource_loader.unref();

		ResourceSaver::remove_resource_format_saver(das_resource_saver);
		das_resource_saver.unref();
	}
}
