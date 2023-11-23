#include "das_resource_format_loader.h"
#include "das_script.h"
#include "das_script_language.h"


Ref<Resource> DasResourceFormatLoader::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress, CacheMode p_cache_mode) {
    // copypaste from lua
    DasScript *new_script = static_cast<DasScript *>(DasScriptLanguage::get_singleton()->create_script());

	// TODO make error handling prettier
    if (!new_script) {
		if (r_error) *r_error = ERR_OUT_OF_MEMORY;
		return nullptr;
	}

	new_script->set_path(p_original_path);

	Error error = new_script->load_source_code(p_original_path);

	if (error != OK) {
		if (r_error) *r_error = error;
		memdelete(new_script);
		return nullptr;
	}
	error = new_script->reload();
	if (error != OK) {
		if (r_error) *r_error = error;
		memdelete(new_script);
		return nullptr;
	}
	return Ref<DasScript>(new_script);
}

void DasResourceFormatLoader::get_recognized_extensions(List<String> *p_extensions) const {
    p_extensions->push_back("das");
}

bool DasResourceFormatLoader::handles_type(const String &p_type) const {
    return (p_type == "Script" || p_type == "DasScript");
}

String DasResourceFormatLoader::get_resource_type(const String &p_path) const {
    String el = p_path.get_extension().to_lower();
	if (el == "das") {
		return "DasScript";
	}
	return "";
}

void DasResourceFormatLoader::get_dependencies(const String &p_path, List<String> *p_dependencies, bool p_add_types) {
    // TODO
}
