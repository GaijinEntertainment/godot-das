#include "init_daslang.h"

#include <daScript/daScript.h>

#include "core/config/project_settings.h"

void initialize_daslang() {
    if (das::Module::require("$")) {
        return;
    }
    CharString boost_global_path = ProjectSettings::get_singleton()->globalize_path("res://boost").utf8();
    das::setDasRoot(boost_global_path.get_data());
    NEED_ALL_DEFAULT_MODULES;
    NEED_MODULE(Module_Godot);
    das::Module::Initialize();
}

void deinitialize_daslang(){
    das::Module::Shutdown();
}
