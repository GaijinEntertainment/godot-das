#include "init_dascript.h"

#include <daScript/daScript.h>


void initialize_dascript() {
    NEED_ALL_DEFAULT_MODULES;
    das::Module::Initialize();
}

void deinitialize_dascript(){
    das::Module::Shutdown();
}
