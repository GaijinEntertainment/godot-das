#include "das_script.h"
#include "das_script_language.h"
#include "das_script_instance.h"


DasScript::DasScript() {
    // TODO
}

DasScript::~DasScript() {
    // TODO
}

bool DasScript::can_instantiate() const {
#ifdef TOOLS_ENABLED
	return valid && (tool || ScriptServer::is_scripting_enabled());
#else
	return valid;
#endif
}

Ref<Script> DasScript::get_base_script() const { 
	// TODO
	return Ref<Script>{};
}

StringName DasScript::get_global_name() const {
    // TODO
	return StringName{};
}

bool DasScript::inherits_script(const Ref<Script> &p_script) const {
    // TODO
	return false;
}

StringName DasScript::get_instance_base_type() const {
    // TODO
	return StringName();
}

ScriptInstance *DasScript::instance_create(Object *p_this) { 
    DasScriptInstance *instance = memnew(DasScriptInstance);
	instance->script = Ref<DasScript>(this);
	instance->owner = p_this;

	instance->owner->set_script_instance(instance);
	{
		MutexLock lock(DasScriptLanguage::singleton->mutex);
		instances.insert(instance->owner);
	}

	// TODO more stuff

	return instance;
}

#ifdef TOOLS_ENABLED
PlaceHolderScriptInstance *DasScript::placeholder_instance_create(Object *p_this) {
	// no idea how this works
	PlaceHolderScriptInstance *instance = memnew(PlaceHolderScriptInstance(DasScriptLanguage::get_singleton(), Ref<Script>(this), p_this));
	placeholders.insert(instance);
	update_exports();

	return instance;
}
#endif


bool DasScript::instance_has(const Object *p_this) const { 
	MutexLock lock(DasScriptLanguage::singleton->mutex);

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

#ifdef TOOLS_ENABLED
Vector<DocData::ClassDoc> DasScript::get_documentation() const {
    // TODO
	return Vector<DocData::ClassDoc>{};
} 

String DasScript::get_class_icon_path() const {
    // TODO
	return String();
}

PropertyInfo DasScript::get_class_category() const {
    // TODO
	return PropertyInfo();
}
#endif

bool DasScript::has_method(const StringName &p_method) const {
	das::SimFunction *function = ctx->findFunction(String(p_method).utf8().get_data());
	return function != nullptr;
}

MethodInfo DasScript::get_method_info(const StringName &p_method) const {
    // TODO
	return MethodInfo{};
}

bool DasScript::is_tool() const {
	return tool;
}

bool DasScript::is_valid() const {
	return valid;
}

bool DasScript::is_abstract() const {
	// TODO
	return false;
}

ScriptLanguage *DasScript::get_language() const {
	return DasScriptLanguage::get_singleton();
}

bool DasScript::has_script_signal(const StringName &p_signal) const {
    // TODO
	return false;
}

void DasScript::get_script_signal_list(List<MethodInfo> *r_signals) const {
    // TODO
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

void DasScript::update_exports() {
#ifdef TOOLS_ENABLED
	// TODO
#endif
}

void DasScript::get_script_method_list(List<MethodInfo> *p_list) const {
    // TODO
}

void DasScript::get_script_property_list(List<PropertyInfo> *p_list) const {
    // TODO
}


int DasScript::get_member_line(const StringName &p_member) const {
    // TODO
	return -1;
}

void DasScript::get_constants(HashMap<StringName, Variant> *p_constants) {
    // TODO
}

void DasScript::get_members(HashSet<StringName> *p_members) {
	// TODO
}

const Variant DasScript::get_rpc_config() const {
	return rpc_config;
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
	return Variant{};
}
