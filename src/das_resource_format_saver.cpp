#include "das_resource_format_saver.h"

#include "das_script.h"
#include "das_script_language.h"


Error DasResourceFormatSaver::save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags) {
    Ref<DasScript> sqscr = p_resource;
	ERR_FAIL_COND_V(sqscr.is_null(), ERR_INVALID_PARAMETER);

	String source = sqscr->get_source_code();

	{
		Error err;
		Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::WRITE, &err);

		ERR_FAIL_COND_V_MSG(err, err, "Cannot save daScript file '" + p_path + "'.");

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
