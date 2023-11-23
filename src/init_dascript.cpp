#include <daScript/daScript.h>
#include "init_dascript.h"

using namespace das;


void initialize_dascript() {
    NEED_ALL_DEFAULT_MODULES;
    Module::Initialize();
}

void deinitialize_dascript(){
    Module::Shutdown();
}
