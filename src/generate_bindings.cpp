#include "generate_bindings.h"

#include "core/object/class_db.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <unordered_set>

// Always run from godot root!
#define SRC_PATH "modules/daslang/src/"

// TODO generate these automatically? How?
static std::vector<const char*> types = {
    "Object",
        "Node",
            "CanvasItem",
                "Node2D",
                    "Sprite2D",
                "Control",
                    "Label",
            "Viewport",
                "Window",
        "Resource",
            "InputEvent",
                "InputEventMouseButton",
            "Texture2D"
};


void generate_godot_types_gen_h() {
    std::ofstream code(SRC_PATH"godot_types_gen.h", std::ios::out | std::ios::trunc);

    code << "// This file is complitely generated\n";
    code << "\n";
    code << "#ifndef GODOT_TYPES_GEN_H\n";
    code << "#define GODOT_TYPES_GEN_H\n";
    code << "\n";
    code << "#include \"godot_types_macro.h\"\n";
    code << "#include \"godot_all_includes.h\"\n";
    code << "\n";

   for (auto& type : types) {
        code << "MAKE_NATIVE_TYPE_FACTORY(" << type << ")\n";
        code << "\n";
    }

    code << "\n";
    code << "#endif // GODOT_TYPES_GEN_H\n";

    code.close();
}

void generate_godot_types_gen_cpp() {
    std::ofstream code(SRC_PATH"godot_types_gen.cpp", std::ios::out | std::ios::trunc);
    code << "// This file is complitely generated\n";
    code << "\n";
    code << "#include \"godot_module.h\"\n";
    code << "#include \"godot_types_gen.h\"\n";
    code << "\n";
    code << "void Module_Godot::bind_types_gen(das::ModuleLibrary & lib) {\n";

    HashSet<StringName> added;

    for (auto& type : types) {
        StringName parent = ClassDB::get_parent_class(type);
        if (parent == "") {
            code << "    BIND_NATIVE_BASE(" << type << ")\n";
            added.insert(type);
        } else {
            // this allows to not bind types in the inheritance chain that are not needed
            while (!added.has(parent)) {
                parent = ClassDB::get_parent_class(parent);
            }
            code << "    BIND_NATIVE_TYPE(" << type << ", " << String(parent).utf8().get_data() << ")\n";
            added.insert(type);
        }
    }

    code << "}\n";
    code.close();
}

void check_types() {
    std::vector<const char*> filtered_types;
    for (auto& type : types) {
        if (!ClassDB::class_exists(type)) {
            std::cerr << "Class " << type << " does not exist, skipping\n";
            continue;
        }
        filtered_types.push_back(type);
    }
    types = std::move(filtered_types);
}


void generate_godot_module_code() {
    check_types();
    generate_godot_types_gen_h();
    generate_godot_types_gen_cpp();
}
