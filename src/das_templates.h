#ifndef DAS_TEMPLATES_H
#define DAS_TEMPLATES_H

#include <core/object/object.h>
#include <core/object/script_language.h>

// TODO more and better
// TODO steal generation grom gdscript

const char * simple_template =R""""(options always_export_initializer = true // do not remove this option

class _CLASS_
_TS_def _ready()
_TS__TS_print("It's ready!")

_TS_def _enter_tree()
_TS__TS_print("It's enter tree!")
)"""";

static const int DAS_TEMPLATES_ARRAY_SIZE = 1;

static const struct ScriptLanguage::ScriptTemplate DAS_TEMPLATES[DAS_TEMPLATES_ARRAY_SIZE] = {
	{ String("Node"), String("Default"),  String("Base template for Node with default Godot cycle methods"),  String(simple_template) },
};



#endif // TEMPLATES_H
