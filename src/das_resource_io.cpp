#include "das_resource_io.h"

#include "das_script.h"
#include "das_script_language.h"

#include "init_daslang.h"

// analogically to GDScriptCache::get_full_script
Ref<Resource> DasResourceFormatLoader::_get_full_script(const String &p_path, Error &r_error) {
	DasScript *new_script = static_cast<DasScript *>(DasScriptLanguage::get_singleton()->create_script());
    if (!new_script) {
		if (r_error) {
			r_error = ERR_OUT_OF_MEMORY;
		}
		return nullptr;
	}
	// in case script is loaded in a separate thread,
	// in the main thread initialization and deinitialization is performed is DasScriptLanguage
	initialize_daslang(); // TODO add shutdown somewhere

	new_script->set_path(p_path);
	r_error = new_script->load_source_code(p_path);
	// can the resource be used even if it failed to load?
	if (r_error != OK) {
		return new_script;
	}
	r_error = new_script->reload();
	return new_script;
}


Ref<Resource> DasResourceFormatLoader::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress, CacheMode p_cache_mode) {
    Error err;
	Ref<Resource> scr = _get_full_script(p_path, err);

	if (err && scr.is_valid()) {
		// If !scr.is_valid(), the error was likely from scr->load_source_code(), which already generates an error.
		ERR_PRINT_ED(vformat(R"(Failed to load script "%s" with error "%s".)", p_path, error_names[err]));
	}

	if (r_error) {
		// Don't fail loading because of parsing error.
		*r_error = scr.is_valid() ? OK : err;
	}

	return scr;
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

Error DasResourceFormatSaver::save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags) {
    Ref<DasScript> sqscr = p_resource;
	ERR_FAIL_COND_V(sqscr.is_null(), ERR_INVALID_PARAMETER);

	String source = sqscr->get_source_code();

	{
		Error err;
		Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::WRITE, &err);

		ERR_FAIL_COND_V_MSG(err, err, "Cannot save Daslang file '" + p_path + "'.");

		file->store_string(source);
		if (file->get_error() != OK && file->get_error() != ERR_FILE_EOF) {
			return ERR_CANT_CREATE;
		}
	}

	if (ScriptServer::is_reload_scripts_on_save_enabled()) {
		DasScriptLanguage::get_singleton()->reload_tool_script(p_resource, true);
	}
    return OK;
}

void DasResourceFormatSaver::get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const {
    if (Object::cast_to<DasScript>(*p_resource)) {
		p_extensions->push_back("das");
	}
}

bool DasResourceFormatSaver::recognize(const Ref<Resource> &p_resource) const {
    return Object::cast_to<DasScript>(*p_resource) != nullptr;
}
