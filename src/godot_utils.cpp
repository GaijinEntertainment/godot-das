#include "godot_module.h"
#include "godot_types_gen.h"

#include "das_script_instance.h"
#include "das_script_language.h"

char* _get_das_type(const Object* obj, const char* name) {
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

void* _promote_to_das_type(Object* obj, const char* script_path) {
    Ref<DasScript> das_script = ResourceLoader::load(script_path);
    if (das_script.is_null()) {
        // should never be here
        CRASH_NOW_MSG(String("File ") + String(script_path) + String(" not found"));
    }
    obj->set_script(das_script);
    DasScriptInstance* instance = reinterpret_cast<DasScriptInstance*>(obj->get_script_instance());
    return instance->get_class_ptr();
}


void Module_Godot::bind_utils(das::ModuleLibrary & lib) {
    das::addExtern<DAS_BIND_FUN(_get_das_type)>(*this, lib, "_get_das_type", das::SideEffects::none, "_get_das_type");
    das::addExtern<DAS_BIND_FUN(_check_das_type)>(*this, lib, "_check_das_type", das::SideEffects::none, "_check_das_type");
    das::addExtern<DAS_BIND_FUN(_promote_to_das_type)>(*this, lib, "_promote_to_das_type", das::SideEffects::modifyArgument, "_promote_to_das_type");
}

REGISTER_MODULE(Module_Godot);
