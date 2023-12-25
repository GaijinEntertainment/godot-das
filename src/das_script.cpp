#include "das_script.h"

#include "das_script_language.h"
#include "das_script_instance.h"

#include <core/os/os.h>
#include <core/error/error_macros.h>
#include <core/config/project_settings.h>


class GodotContext : public das::Context {
public:
	GodotContext(uint32_t stackSize = 16*1024, bool ph = false) : Context(stackSize, ph) { }
	void to_out(const das::LineInfo *, const char * message) override {
        __print_line(message);
    }
	void to_err(const das::LineInfo *, const char * message) override {
        print_error(message);
    }
};

DasScript::DasScript() : script_list(this) {
	{
		DasScriptLanguage::get_singleton()->acquire_lock();

		DasScriptLanguage::get_singleton()->add_script(&script_list);
	}

	path = vformat("dascript://%d.gd", get_instance_id());
}

DasScript::~DasScript() {
	DasScriptLanguage::get_singleton()->acquire_lock();

	script_list.remove_from_list();
	// TODO other stuff?
}

das::ContextPtr DasScript::get_ctx() {
	return ctx;
}

void DasScript::erase_instance(Object *p_owner) {
	instances.erase(p_owner);
}

int DasScript::get_field_offset(const StringName &p_field) const {
	// TODO precompute this
	for (auto &field : main_structure->fields) {
		if (field.name == String(p_field).utf8().get_data()) {
			return field.offset;
		}
	}
	return INVALID_OFFSET;
}

bool DasScript::can_instantiate() const {
#ifdef TOOLS_ENABLED
	return valid && (tool || ScriptServer::is_scripting_enabled());
#else
	return valid;
#endif
}

ScriptInstance *DasScript::instance_create(Object *p_this) {
	DasScriptInstance *instance = memnew(DasScriptInstance);
	instance->set_script(Ref<DasScript>(this));
	instance->set_owner(p_this);

	char* class_ptr = ctx->heap->allocate(main_structure->getSizeOf64());
	instance->set_class_ptr(class_ptr);
	vec4f args[1];
	ctx->callWithCopyOnReturn(struct_ctor, args, class_ptr, nullptr);

	int native_offset = get_field_offset("native");
	if (native_offset != INVALID_OFFSET) {
		Object **native_obj = (Object **)(class_ptr + native_offset);
		*native_obj = p_this;
	}
	// Both gdscript and csharpscript have this line, but this operation seems redundant
	// because in Object::set_script all operations are already performed
	// Now I need this line to safely recreate script instance on reload, so don't remove it
	p_this->set_script_instance(instance);
	{
		DasScriptLanguage::get_singleton()->acquire_lock();
		instances.insert(p_this);
	}

	// TODO more stuff

	return instance;
}

PlaceHolderScriptInstance *DasScript::placeholder_instance_create(Object *p_this) {
#ifdef TOOLS_ENABLED
	// no idea how this works
	PlaceHolderScriptInstance *instance = memnew(PlaceHolderScriptInstance(DasScriptLanguage::get_singleton(), Ref<Script>(this), p_this));
	placeholders.insert(instance);
	update_exports();

	return instance;
#else
	return nullptr;
#endif
}

bool DasScript::instance_has(const Object *p_this) const {
	DasScriptLanguage::get_singleton()->acquire_lock();

	return instances.has((Object *)p_this);
}

bool DasScript::has_source_code() const {
	return !source.is_empty();
}

String DasScript::get_source_code() const {
	return source;
}

void DasScript::set_source_code(const String &p_code) {
	if (source == p_code) {
		return;
	}
	source = p_code;
}


Error DasScript::reload(bool p_keep_state) {
	valid = false;
	// TODO wrap this code in a separate function
	const char* global_file_path = ProjectSettings::get_singleton()->globalize_path(path).utf8().get_data();
    auto new_fAccess = das::make_smart<das::FsFileAccess>();
	auto source_utf8 = source.utf8();

	auto source_data = source_utf8.get_data();
	auto source_len = uint32_t(strlen(source_data));
    auto fileInfo = das::make_unique<das::TextFileInfo>(source_data, source_len, false);
    new_fAccess->setFileInfo(global_file_path, das::move(fileInfo));

	das::TextPrinter dummyLogs;
	auto new_lib_group = std::make_unique<das::ModuleGroup>();

	auto new_program = das::compileDaScript(global_file_path, new_fAccess, dummyLogs, *new_lib_group);

	if (new_program->failed()) {
		auto first_error = new_program->errors.front();
		// TODO what about fixme and extra???
		_err_print_error("DasScript::reload", (const char *)path.utf8().get_data(), first_error.at.line, ("Parse Error: " + first_error.what).c_str(), false, ERR_HANDLER_SCRIPT);
		return ERR_PARSE_ERROR;
	}
	bool new_tool = new_program->options.getBoolOption("tool", false);
	auto new_ctx = std::make_shared<GodotContext>(new_program->getContextStackSize());
	if ( !new_program->simulate(*new_ctx, dummyLogs) ) {
		auto first_error = new_program->errors.front();
		// TODO will simulation errors appear in program->errors?
		_err_print_error("DasScript::reload", (const char *)path.utf8().get_data(), first_error.at.line, ("Simulation Error: " + first_error.what).c_str(), false, ERR_HANDLER_SCRIPT);
		return ERR_PARSE_ERROR;
	}

	auto* rootModule = new_program->thisModule.get();
	auto new_main_structure = rootModule->findStructure(class_name);
	if (!new_main_structure) {
		_err_print_error("DasScript::reload", (const char *)path.utf8().get_data(), 0, "Script must contain a class with the same name as the script", false, ERR_HANDLER_SCRIPT);
		return OK;
	}
	auto new_struct_ctor = new_ctx->findFunction(class_name.c_str());
	if (!new_struct_ctor) {
		_err_print_error("DasScript::reload", (const char *)path.utf8().get_data(), 0, "Do not remove the option at the beginning of the script", false, ERR_HANDLER_SCRIPT);
		return OK;
	}

	valid = true;

	lib_group = std::move(new_lib_group);
	ctx = std::move(new_ctx);
	program = std::move(new_program);
	main_structure = std::move(new_main_structure);
	struct_ctor = std::move(new_struct_ctor);
	file_access = std::move(new_fAccess);
	tool = new_tool;

	for (auto &instance_owner : instances) {
		instance_create(instance_owner);
	}

	return OK;

}

void DasScript::set_path(const String &p_path, bool p_take_over) {
	// TODO more
	Script::set_path(p_path, p_take_over);
	path = p_path;
	if (class_name.empty()) {
		class_name = p_path.get_file().get_basename().utf8().get_data();
	}
}

bool DasScript::has_method(const StringName &p_method) const {
	// TODO precompute func search
	return get_field_offset(p_method) != INVALID_OFFSET;
}

MethodInfo DasScript::get_method_info(const StringName &p_method) const {
    // TODO
	// convert daScript function signature to Godot MethodInfo
	return MethodInfo{};
}

bool DasScript::is_tool() const {
	return tool;
}

bool DasScript::is_valid() const {
	return valid;
}

ScriptLanguage *DasScript::get_language() const {
	return DasScriptLanguage::get_singleton();
}

bool DasScript::get_property_default_value(const StringName &p_property, Variant &r_value) const {
#ifdef TOOLS_ENABLED
    // TODO
    // here only
	return false;
#endif
    // nothing here
	return false;
}

Error DasScript::load_source_code(const String &p_path) {
	Error error;

	Ref<FileAccess> f = FileAccess::open(p_path, FileAccess::READ, &error);
	if (error) {
		ERR_FAIL_COND_V(error, error);
	}
	Vector<uint8_t> sourcef;
	uint64_t len = f->get_length();
	sourcef.resize(len + 1);
	uint8_t *w = sourcef.ptrw();
	uint64_t r = f->get_buffer(w, len);
	ERR_FAIL_COND_V(r != len, ERR_CANT_OPEN);
	w[len] = 0;

	String s;
	if (s.parse_utf8((const char *)w) != OK) {
		ERR_FAIL_V_MSG(ERR_INVALID_DATA, "Script '" + p_path + "' contains invalid unicode (UTF-8), so it was not loaded. Please ensure that scripts are saved in valid UTF-8 unicode.");
	}

	set_source_code(s);

	// TODO what will happen in case of a file rename?
	set_path(p_path);
	return OK;
}

void DasScript::_bind_methods() {
	ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "new", &DasScript::_new, MethodInfo("new"));
}


#ifdef TOOLS_ENABLED
void DasScript::_placeholder_erased(PlaceHolderScriptInstance *p_placeholder) {
	this->placeholders.erase(p_placeholder);
}
#endif

Variant DasScript::_new(const Variant **p_args, int p_argcount, Callable::CallError &r_error) {
    // TODO
	// looking at GDScript implementation, this is pretty important
	return Variant{};
}
