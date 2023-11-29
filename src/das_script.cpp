#include "das_script.h"
#include "das_script_language.h"
#include "das_script_instance.h"


DasScript::DasScript() {
    // TODO
	// add to script list in DasScriptLanguage
}

DasScript::~DasScript() {
    // TODO
	// remove from script list in DasScriptLanguage
}

das::ContextPtr DasScript::get_ctx() {
	return ctx;
}

void DasScript::erase_instance(Object *p_owner) {
	instances.erase(p_owner);
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

	p_this->set_script_instance(instance);
	{
		DasScriptLanguage::get_singleton()->acquire();
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
	DasScriptLanguage::get_singleton()->acquire();

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
    auto fAccess = das::make_smart<das::FsFileAccess>();
	auto source_data = source.utf8().get_data();
	auto source_len = uint32_t(source.size());
    auto fileInfo = das::make_unique<das::TextFileInfo>(source_data, source_len, false);
    fAccess->setFileInfo("dummy.das", das::move(fileInfo));

	das::TextPrinter tout;

	das::ModuleGroup dummyLibGroup;
	program = das::compileDaScript("dummy.das", fAccess, tout, dummyLibGroup);

	// TODO output somewhere nice
	// also not sure compilation goes here
	if (program->failed()) {
		tout << "failed to compile\n";
		for ( auto & err : program->errors ) {
			tout << das::reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
		}
		return OK;
	}
	ctx = std::make_shared<das::Context>(program->getContextStackSize());
	if ( !program->simulate(*ctx, tout) ) {
		tout << "failed to simulate\n";
		for ( auto & err : program->errors ) {
			tout << das::reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
		}
		return OK;
	}
	valid = true;
	return OK;

}

bool DasScript::has_method(const StringName &p_method) const {
	das::SimFunction *function = ctx->findFunction(String(p_method).utf8().get_data());
	return function != nullptr;
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
	path = p_path;
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
