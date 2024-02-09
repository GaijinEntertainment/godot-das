#include "generate_bindings.h"

#include "core/object/class_db.h"
// #include "core/core_constants.h"

#include <fstream>
#include <iostream>
#include <set>

// Always run from godot root!
#define SRC_PATH "modules/daslang/src/"
#define BOOST_PATH "modules/daslang/boost/"

#define STR(S) String(S).utf8().get_data()

#include "types_and_funcs.h"


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
    // Object is handled separately! hence i = 1
    for (int i = 1; i < types.size(); i++) {
        auto& type_and_funcs = types[i];
        const char* type = type_and_funcs.first;
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

void generate_godot_signals_gen_das() {
    std::ofstream code(BOOST_PATH"godot_signals_gen.das", std::ios::out | std::ios::trunc);
    code << "// This file is complitely generated\n";
    code << "// TODO `operator . signal` as soon as operator . on pointers is supported\n";
    code << "\n";
    code << "require godot_native\n";
    code << "require godot_signals\n";
    code << "\n";

    const std::set<Variant::Type> supported_args = {Variant::Type::FLOAT, Variant::Type::OBJECT, Variant::RID, Variant::Type::INT};

    // Object is NOT handled separately! hence i = 0
    for (int i = 0; i < types.size(); i++) {

        auto& type_and_funcs = types[i];
        const char* type = type_and_funcs.first;
        List<MethodInfo> signals;
        ClassDB::get_signal_list(type, &signals, true);

        if (signals.size() == 0) {
            continue;
        }
        code << "// " << type << "\n";
        code << "\n";
        for (auto& signal: signals) {
            bool supported = true;
            for (auto& sig_arg : signal.arguments) {
                if (supported_args.find(sig_arg.type) == supported_args.end()) {
                    supported = false;
                    break;
                }
            }
            if (!supported) {
                code << "/* TODO support arguments\n";
            }

            code << "let " << type << "`" << STR(signal.name) << " = \"" << STR(signal.name) << "\"\n";
            code << "\n";
            code << "def get_" << STR(signal.name) << "(obj : " << type << "?)\n";
            code << "    return [[Signal name = " << type << "`" << STR(signal.name) << ", owner = obj]]\n";
            if (!supported) {
                code << "*/\n";
            }
            code << "\n";
        }
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
    // Object is NOT handled separately! hence i = 0
    for (int i = 0; i < types.size(); i++) {
        auto& type_and_funcs = types[i];
        const char* type = type_and_funcs.first;
        code << "MAKE_NATIVE_TYPE_FACTORY(" << type << ")\n";
        code << "\n";
        List<StringName> enums;
        ClassDB::get_enum_list(type, &enums, true);
        for (auto& enum_name : enums) {
            code << "DAS_BIND_ENUM_CAST(" << type << "::" << STR(enum_name) << ")\n";
            code << "DAS_BASE_BIND_ENUM_SAFE(" << type << "::" << STR(enum_name) << ", "
                                               << type << "`" << STR(enum_name) << ", "
                                               << type << "_" << STR(enum_name) << ", ";
            List<StringName> enum_constants;
            ClassDB::get_enum_constants(type, enum_name, &enum_constants, true);
            for (int i = 0; i < enum_constants.size(); i++) {
                code << STR(enum_constants[i]);
                if (i != enum_constants.size() - 1) {
                    code << ", ";
                }
            }
            code << ")\n";
            code << "\n";
        }
    }
    // Doesn't work =(
    // There's no way to differentiate between constants with prefixes and without them
    // This is a job for libclang I'm afraid

    // HashSet<StringName> added;
    // for (int i = 0; i < CoreConstants::get_global_constant_count(); i++) {
    //     auto enum_name = CoreConstants::get_global_constant_enum(i);
    //     if (added.has(enum_name)) {
    //         continue;
    //     }
    //     added.insert(enum_name);
    //     code << "DAS_BIND_ENUM_CAST(" << STR(enum_name) << ")\n";
    //     // static void get_enum_values(StringName p_enum, HashMap<StringName, int64_t> *p_values);
    //     HashMap<StringName, int64_t> values;
    //     CoreConstants::get_enum_values(enum_name, &values);
    //     code << "DAS_BASE_BIND_ENUM(" << STR(enum_name) << ", " << STR(enum_name) << ", ";
    //     for (auto& value : values) {
    //         code << STR(value.key);
    //         if (value.key != values.last()->key) {
    //             code << ", ";
    //         }
    //     }
    //     code << ")\n";
    //     code << "\n";
    // }

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
    // Object is NOT handled separately! hence i = 0
    for (int i = 0; i < types.size(); i++) {
        auto& type_and_funcs = types[i];
        const char* type = type_and_funcs.first;
        if (i > 0) {
            StringName parent = ClassDB::get_parent_class(type);
            // this allows to not bind types in the inheritance chain that are not needed
            while (!added.has(parent)) {
                parent = ClassDB::get_parent_class(parent);
            }
            code << "    BIND_NATIVE_TYPE(" << type << ", " << STR(parent) << ")\n";
        } else {
            code << "    BIND_NATIVE_BASE(" << type << ")\n";
        }
        List<StringName> enums;
        ClassDB::get_enum_list(type, &enums, true);
        for (auto& enum_name : enums) {
            code << "    BIND_ENUM(" << type << ", " << STR(enum_name) << ")\n";
        }
        added.insert(type);
    }

    code << "}\n";
    code.close();
}

void generate_godot_functions_gen_cpp() {
    std::ofstream code(SRC_PATH"godot_functions_gen.cpp", std::ios::out | std::ios::trunc);
    code << "// This file is complitely generated\n";
    code << "\n";
    code << "#include \"godot_module.h\"\n";
    code << "\n";
    code << "#include \"godot_types_gen.h\"\n";
    code << "#include \"godot_types_extra.h\"\n";
    code << "\n";
    code << "#include \"godot_functions_wrapper.h\"\n";
    code << "\n";
    code << "\n";
    code << "void Module_Godot::bind_functions_gen(das::ModuleLibrary & lib) {\n";

    // Object is NOT handled separately! hence i = 0
    for (int i = 0; i < types.size(); i++) {
        auto& type_and_funcs = types[i];
        const char* type = type_and_funcs.first;
        std::vector<const char*>& funcs = type_and_funcs.second;

        code << "    // " << type << "\n";
        // TODO understand how to create RefCounted so they don't die instantly after returning
        if (ClassDB::can_instantiate(type) && !ClassDB::is_parent_class(type, SNAME("RefCounted"))) {
            code << "    BIND_GODOT_CTOR(" << type << ")\n";
        }

        for (auto method_name: funcs) {
            MethodInfo method;
            ClassDB::get_method_info(type, method_name, &method, true, false);
            if (method.flags & METHOD_FLAG_VIRTUAL) {
                // virtual methods should not be called from script
                continue;
            }
            if (method.flags & METHOD_FLAG_VARARG) {
                // too complicated
                continue;
            }
            if (static_cast<char32_t>(method.name[0]) == static_cast<char32_t>('_')) {
                // private methods should not be called from script
                continue;
            }
            if (method.flags & METHOD_FLAG_STATIC) {
                // TODO
                continue;
            }
            if (exceptions.find(type) != exceptions.end() && exceptions[type].find(method_name) != exceptions[type].end()){
                code << "    BIND_GODOT_MEMBER_RENAME(" << type << ", " << exceptions[type][method_name] << ", " << STR(method.name);
            } else {
                code << "    BIND_GODOT_MEMBER(" << type << ", " << STR(method.name);
            }
            for (auto& arg: method.arguments) {
                code << ", \"" << STR(arg.name) << '"';
            }
            code << ")\n";
            int default_args_place = method.arguments.size() + 1 - method.default_arguments.size();
            for (auto& def_arg: method.default_arguments) {
                code << "    SET_DEFAULT_ARG(" << type << ", " << STR(method.name) << ", " << default_args_place << ", ";
                if (def_arg.get_type() == Variant::BOOL) {
                    code << (def_arg.operator bool() ? "true" : "false");
                } else if (def_arg.get_type() == Variant::INT) {
                    code << def_arg.operator int64_t();
                } else if (def_arg.get_type() == Variant::FLOAT) {
                    code << def_arg.operator double();
                } else if (def_arg.get_type() == Variant::STRING) {
                    code << '"' << STR(def_arg.operator String()) << '"';
                } else if (def_arg.get_type() == Variant::COLOR) {
                    Color color = def_arg.operator Color();
                    code << "Color(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")";
                } else if (def_arg.get_type() == Variant::VECTOR2) {
                    Vector2 vec = def_arg.operator Vector2();
                    code << "das::float2(" << vec.x << ", " << vec.y << ")";
                } else {
                    std::cerr << "Unknown default argument type " << def_arg.get_type() << " for " << type << "::" << STR(method.name) << " for argument " << STR(method.arguments[default_args_place-1].name) << "!!!\n";
                }
                code << ")\n";
                default_args_place++;
            }
        }
    }
    code << "}\n";
    code.close();
}

void generate_godot_module_code() {
    generate_godot_casts_gen_das();
    generate_godot_signals_gen_das();
    generate_godot_types_gen_h();
    generate_godot_types_gen_cpp();
    generate_godot_functions_gen_cpp();
}
