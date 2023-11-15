#include <daScript/daScript.h>
#include <gdextension_interface.h>
#include "tutorial00.h"


extern "C" {
GDExtensionBool godot_das_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
    tutorial();
    // TODO initialize gdextension properly
    return 1;
}
}
