#include "das_script.h"

#include "das_script_language.h"
#include "das_script_instance.h"

#include "core/os/os.h"
#include "core/error/error_macros.h"
#include "core/config/project_settings.h"


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
}

DasScript::~DasScript() {
	DasScriptLanguage::get_singleton()->acquire_lock();

	script_list.remove_from_list();
	// TODO other stuff?
}

das::ContextPtr DasScript::get_ctx() const {
	return ctx;
}

void DasScript::erase_instance(Object *p_owner) {
	instances.erase(p_owner);
}

size_t DasScript::get_field_offset(const StringName &p_field) const {
	// getptr because it's safe and calls `_lookup_pos` only once
	auto ptr = offsets.getptr(p_field);
	return ptr ? *ptr : INVALID_OFFSET;
}


const char* DasScript::get_class_name() const {
	return main_structure->name.c_str();
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

	for (auto &signal : signals) {
		p_this->add_user_signal(String(signal.first));
		// probably not exactly safe
		const size_t name_offset = 0;
		const size_t owner_offset = 8;
		// TODO move this to das inside constructor
		const char** signal_name = (const char**)(class_ptr + signal.second + name_offset);
		*signal_name = signal.first;
		Object** signal_owner = (Object**)(class_ptr + signal.second + owner_offset);
		*signal_owner = p_this;
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

void DasScript::_print_errors(das::ProgramPtr p_program, const char* p_msg_begin) const {
	CharString path_utf8 = get_path().utf8();
	for (auto &err : p_program->errors) {
		std::string err_msg = p_msg_begin + err.what;
		if (!err.fixme.empty())
			err_msg += "; " + err.fixme;
		if (!err.extra.empty())
			err_msg += "; " + err.extra;
		_err_print_error("DasScript::reload", path_utf8.get_data(), err.at.line, err_msg.c_str(), false, ERR_HANDLER_SCRIPT);
	}
}


Error DasScript::reload(bool p_keep_state) {
	valid = false;

	auto new_file_access = das::make_smart<das::FsFileAccess>();
	auto new_lib_group = std::make_unique<das::ModuleGroup>();
	das::TextPrinter dummy_logs{};

	auto new_program = DasScriptLanguage::compile_script(source, get_path(), new_file_access, dummy_logs, *new_lib_group);
	if (new_program->failed()) {
		_print_errors(new_program, "Compilation Error: ");
		return ERR_PARSE_ERROR;
	}

	auto new_ctx = std::make_shared<GodotContext>(new_program->getContextStackSize());
	bool simulation_ok = new_program->simulate(*new_ctx, dummy_logs);
	if (!simulation_ok) {
		_print_errors(new_program, "Simulation Error: ");
		return ERR_PARSE_ERROR;
	}

	auto this_module = new_program->thisModule.get();
	das::FunctionPtr new_struct_ctor = this_module->findFunction("__instance_ctor");

	if (!new_struct_ctor) {
		return OK;
	}
	auto new_main_structure = new_struct_ctor->result->structType;

	valid = true;

	lib_group = std::move(new_lib_group);
	ctx = std::move(new_ctx);
	program = std::move(new_program);
	main_structure = std::move(new_main_structure);
	file_access = std::move(new_file_access);

	struct_ctor = ctx->findFunction("__instance_ctor");
	tool = program->options.getBoolOption("tool", false);


	for (auto& field : main_structure->fields) {
		offsets[StringName(field.name.c_str())] = field.offset;

		if (field.type->structType && field.type->structType->name == "Signal") {
			// filed name will be alive for the whole lifetime of the script
			signals.emplace_back(field.name.c_str(), field.offset);
		}
	}

	for (auto &instance_owner : instances) {
		instance_create(instance_owner);
	}

	return OK;

}

bool DasScript::has_method(const StringName &p_method) const {
	// TODO precompute func search
	return get_field_offset(p_method) != INVALID_OFFSET;
}

MethodInfo DasScript::get_method_info(const StringName &p_method) const {
    // TODO
	// convert Daslang function signature to Godot MethodInfo
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

#ifdef TOOLS_ENABLED
void DasScript::_placeholder_erased(PlaceHolderScriptInstance *p_placeholder) {
	this->placeholders.erase(p_placeholder);
}
#endif
