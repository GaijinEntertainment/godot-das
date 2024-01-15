#include "godot_module.h"
#include "godot_functions_macro.h"
#include "godot_types_gen.h"

#include "das_script_instance.h"
#include "das_script_language.h"

char* _get_das_type(const Object* obj, const char* name, CTX_AT) {
    if (obj == nullptr) {
        return nullptr;
    }
    DasScriptInstance* instance = dynamic_cast<DasScriptInstance*>(obj->get_script_instance());
    if (instance == nullptr || strcmp(instance->get_das_script()->get_class_name(), name) != 0) {
        // two cases: either the object does not have a das script instance, or the das script instance is not of the correct type
        return nullptr;
    }
    return instance->get_class_ptr();
}

bool _check_das_type(const Object* obj, const char* name) {
    if (obj == nullptr) {
        return false;
    }
    DasScriptInstance* instance = dynamic_cast<DasScriptInstance*>(obj->get_script_instance());
    if (instance == nullptr || strcmp(instance->get_das_script()->get_class_name(), name) != 0) {
        return false;
    }
    return true;
}

void* _promote_to_das_type(Object* obj, const char* script_name, CTX_AT) {
    DasScript* das_script = DasScriptLanguage::get_singleton()->get_script(script_name);
    CHECK_IF_NULL_MSG(das_script, (std::string("cannot find script ") + std::string(script_name)).c_str());
    obj->set_script(das_script);
    return reinterpret_cast<DasScriptInstance*>(obj->get_script_instance())->get_class_ptr();
}

// Not a util, but not generatable in its current state
Resource* _load(Object* obj, const char* p_path, CTX_AT) {
    CHECK_IF_NULL_MSG(obj, "cannot bind resource to null object");
    auto script_instance = dynamic_cast<DasScriptInstance*>(obj->get_script_instance());
    CHECK_IF_NULL_MSG(script_instance, "cannot bind resource to non-das object");
    Ref<Resource> res = ResourceLoader::load(p_path);
    if (res.is_valid()) {
        // tmp solution
        // TODO support das::smart_ptr<Resource> or Ref<Resource> in das
        // so ref counting is handled by das
        script_instance->bind_resource(res);
    }
    return res.ptr();
}


void Module_Godot::bind_utils(das::ModuleLibrary & lib) {
    das::addExtern<DAS_BIND_FUN(_get_das_type)>(*this, lib, "_get_das_type", das::SideEffects::modifyExternal, "_get_das_type");
    das::addExtern<DAS_BIND_FUN(_check_das_type)>(*this, lib, "_check_das_type", das::SideEffects::modifyExternal, "_check_das_type");
    das::addExtern<DAS_BIND_FUN(_promote_to_das_type)>(*this, lib, "_promote_to_das_type", das::SideEffects::modifyExternal, "_promote_to_das_type");

    das::addExtern<DAS_BIND_FUN(_load)>(*this, lib, "load", das::SideEffects::modifyExternal, "_load");
}

REGISTER_MODULE(Module_Godot);
