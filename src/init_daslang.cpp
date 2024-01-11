#include "init_daslang.h"

#include <daScript/daScript.h>


void initialize_daslang() {
    if (das::Module::require("$")) {
        return;
    }
    NEED_ALL_DEFAULT_MODULES;
    NEED_MODULE(Module_Godot);
    das::Module::Initialize();
}

void deinitialize_daslang(){
    das::Module::Shutdown();
}
