#include "generate_bindings.h"

#include "core/object/class_db.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <unordered_set>

// Always run from godot root!
#define SRC_PATH "modules/daslang/src/"
#define BOOST_PATH "modules/daslang/boost/"

// TODO generate these automatically? How?
// all these types should inherit from Object
static std::vector<const char*> types = {
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

void generate_godot_casts_gen_das() {
    std::ofstream code(BOOST_PATH"godot_casts_gen.das", std::ios::out | std::ios::trunc);
    // For now, as operates as GDScript as - return null if cast fail, and return null if casting null
    code << "// This file is complitely generated\n";
    code << "\n";
    code << "require godot_native\n";
    code << "\n";
    code << "// Object\n";
    code << "\n";
    code << "def operator as Object(native: Object?)\n";
    code << "    return native\n";
    code << "\n";
    code << "def operator is Object(native: Object?)\n";
    code << "    return true\n";
    code << "\n";
    for (auto type : types) {
        code << "// " << type << "\n";
        code << "\n";
        code << "def operator as " << type << "(native: " << type << "?)\n";
        code << "    return native\n";
        code << "\n";
        code << "def operator as " << type << "(native: Object?)\n";
        code << "    if _check_native_type_" << type << "(native)\n";
        code << "        return unsafe(reinterpret<" << type << "?>(native))\n";
        code << "    return null\n";
        code << "\n";
        code << "def operator is " << type << "(native: " << type << "?)\n";
        code << "    return true\n";
        code << "\n";
        code << "def operator is " << type << "(native: Object?)\n";
        code << "    return _check_native_type_" << type << "(native)\n";
        code << "\n";
    }
    code.close();
}


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
    code << "MAKE_NATIVE_TYPE_FACTORY(Object)\n";
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
    code << "    BIND_NATIVE_BASE(Object)\n";
    added.insert("Object");
    for (auto type : types) {
        StringName parent = ClassDB::get_parent_class(type);
        // this allows to not bind types in the inheritance chain that are not needed
        while (!added.has(parent)) {
            parent = ClassDB::get_parent_class(parent);
        }
        code << "    BIND_NATIVE_TYPE(" << type << ", " << String(parent).utf8().get_data() << ")\n";
        added.insert(type);
    }

    code << "}\n";
    code.close();
}

// void generate_godot_functions_gen_cpp() {
//     std::ofstream code(SRC_PATH"godot_functions_gen.cpp", std::ios::out | std::ios::trunc);
//     code << "// This file is complitely generated\n";
//     code << "\n";
//     code << "#include \"godot_module.h\"\n";
//     code << "\n";
//     code << "#include \"godot_types_gen.h\"\n";
//     code << "#include \"godot_types_extra.h\"\n";
//     code << "\n";
//     code << "#include \"godot_functions_wrapper.h\"\n";
//     code << "\n";
//     code << "\n";
//     code << "void Module_Godot::bind_functions_gen(das::ModuleLibrary & lib) {\n";

//     code << "    // Object\n";
//     code << "    BIND_GODOT_CTOR(Object)\n";
//     for (auto type : types) {
//         code << "    // " << type << "\n";
//         if (ClassDB::can_instantiate(type)) {
//             code << "    BIND_GODOT_CTOR(" << type << ")\n";
//         }
//         List<MethodInfo> methods;
//         ClassDB::get_method_list(type, &methods, true, false);
//         for (int i = 0; i < methods.size(); i++) {
//             auto& method = methods[i];
//             if (method.flags & METHOD_FLAG_VIRTUAL) {
//                 // virtual methods should not be called from script
//                 continue;
//             }
//             if (method.flags & METHOD_FLAG_VARARG) {
//                 // too complicated
//                 continue;
//             }
//             if (static_cast<char32_t>(method.name[0]) == static_cast<char32_t>('_')) {
//                 // private methods should not be called from script
//                 continue;
//             }
//             if (method.flags & METHOD_FLAG_STATIC) {
//                 // TODO
//                 continue;
//             }

//             code << "    BIND_GODOT_MEMBER(" << type << ", " << method.name.utf8().get_data() << ")\n";
//         }
//     }
//     code << "}\n";
//     code.close();
// }

void check_types() {
    std::vector<const char*> filtered_types;
    for (auto type : types) {
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
    generate_godot_casts_gen_das();
    //generate_godot_functions_gen_cpp();
}
