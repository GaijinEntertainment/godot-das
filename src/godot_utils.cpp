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


void Module_Godot::bind_utils(das::ModuleLibrary & lib) {
    das::addExtern<DAS_BIND_FUN(_get_das_type)>(*this, lib, "_get_das_type", das::SideEffects::none, "_get_das_type");
    das::addExtern<DAS_BIND_FUN(_check_das_type)>(*this, lib, "_check_das_type", das::SideEffects::none, "_check_das_type");
    das::addExtern<DAS_BIND_FUN(_promote_to_das_type)>(*this, lib, "_promote_to_das_type", das::SideEffects::modifyArgument, "_promote_to_das_type");
}

REGISTER_MODULE(Module_Godot);
