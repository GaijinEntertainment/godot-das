#include "das_script_instance.h"

#include <daScript/daScript.h>
#include <daScript/misc/platform.h>

#include <scene/2d/node_2d.h>
#include <core/math/vector2.h>

#include <vector>
#include <fstream>

// taken from dagor/prog/gameLibs/publicInclude/dasModules/dasModulesCommon.h
// typeName and das_alias do not seem to be vital but may be required in the future
#define MAKE_TYPE_FACTORY_ALIAS(TYPE, DAS_DECL_TYPE)           \
namespace das {                                                \
  template <>                                                  \
  struct typeFactory<TYPE>                                     \
  {                                                            \
    static TypeDeclPtr make(const ModuleLibrary &lib)          \
    {                                                          \
      auto t = make_smart<TypeDecl>(Type::DAS_DECL_TYPE);      \
      t->alias = #TYPE;                                        \
      t->aotAlias = true;                                      \
      return t;                                                \
    }                                                          \
  };                                                           \
}

#define NAME_NATIVE_TYPE_FACTORY(TYPE) MAKE_TYPE_FACTORY(TYPE, TYPE)


#define BIND_NATIVE_BASE(TYPE)\
struct TYPE##Annotation : das::ManagedStructureAnnotation<TYPE> {\
    TYPE##Annotation(das::ModuleLibrary & ml) : ManagedStructureAnnotation(#TYPE, ml) { }\
};\
das::smart_ptr<TYPE##Annotation> TYPE##ManagedStructureAnnotation = das::make_smart<TYPE##Annotation>(lib);\
addAnnotation(TYPE##ManagedStructureAnnotation);


#define BIND_NATIVE_TYPE(TYPE, PARENT)\
struct TYPE##Annotation : das::ManagedStructureAnnotation<TYPE> {\
    das::TypeDeclPtr parentType;\
    das::smart_ptr<das::ManagedStructureAnnotation<PARENT>> parent_annotation;\
    \
    TYPE##Annotation(das::ModuleLibrary & ml,\
                     das::smart_ptr<das::ManagedStructureAnnotation<PARENT>> parent_annotation) :\
                     ManagedStructureAnnotation(#TYPE, ml),\
                     parent_annotation(parent_annotation),\
                     parentType(das::makeType<PARENT>(ml)) {}\
    \
    bool canBeSubstituted(TypeAnnotation *pass_type) const override {\
        return parentType->annotation == pass_type || parent_annotation->canBeSubstituted(pass_type);\
    }\
};\
das::smart_ptr<TYPE##Annotation> TYPE##ManagedStructureAnnotation = das::make_smart<TYPE##Annotation>(lib, PARENT##ManagedStructureAnnotation);\
addAnnotation(TYPE##ManagedStructureAnnotation);

template <typename T>
bool _check_native_type(const Object* obj) {
    return dynamic_cast<const T*>(obj) != nullptr;
}

#define BIND_TYPE_CHECKER(TYPE)  das::addExtern<DAS_BIND_FUN(_check_native_type<TYPE>)>(*this, lib, "_check_native_type_"#TYPE, das::SideEffects::none, "_check_native_type<"#TYPE">");

#define CTX_AT das::Context *ctx, das::LineInfoArg *at
#define CHECK_IF_NULL(PTR) if (PTR == nullptr) { ctx->throw_error_at(at, "dereferencing null pointer"); return {}; }
#define CHECK_IF_NULL_VOID(PTR) if (PTR == nullptr) { ctx->throw_error_at(at, "dereferencing null pointer"); return; }

char* _check_dascript_type(const Object* obj, const char* name, das::Context *ctx) {
    if (obj == nullptr) {
        ctx->to_err(nullptr, "cannot cast null");
        return nullptr;
    }
    DasScriptInstance* instance = dynamic_cast<DasScriptInstance*>(obj->get_script_instance());
    if (instance == nullptr || instance->get_das_script()->get_class_name() != name) {
        // two cases: either the object does not have a das script instance, or the das script instance is not of the correct type
        ctx->to_err(nullptr, (std::string("type mismatch: cannot cast to ") + std::string(name)).c_str());
        return nullptr;
    }
    return instance->get_class_ptr();
}

NAME_NATIVE_TYPE_FACTORY(Object)
NAME_NATIVE_TYPE_FACTORY(Node)
NAME_NATIVE_TYPE_FACTORY(Node2D)

template<> struct das::ToBasicType<String>     { enum { type = das::Type::tString }; };

MAKE_TYPE_FACTORY_ALIAS(Vector2, tFloat2);
template <> struct das::cast<Vector2> : das::cast_fVec_half<Vector2> {};

Node* _Node_find_child(const Node* node, const char* p_pattern, CTX_AT) {
    CHECK_IF_NULL(node)
    // TODO add p_recursive and p_owned
    return node->find_child(p_pattern, true, true);
}

Node* _Node_get_parent(const Node* node, CTX_AT) {
    CHECK_IF_NULL(node)
    return node->get_parent();
}

void _Node2D_rotate(Node2D* node2d, float p_radians, CTX_AT) {
    CHECK_IF_NULL_VOID(node2d)
    node2d->rotate(p_radians);
}

void _Node2D_translate(Node2D* node2d, const Vector2 p_amount, CTX_AT) {
    CHECK_IF_NULL_VOID(node2d)
    node2d->translate(p_amount);
}

void _Node2D_set_position(Node2D* node2d, const Vector2 p_pos, CTX_AT) {
    CHECK_IF_NULL_VOID(node2d)
    node2d->set_position(p_pos);
}

Vector2 _Node2D_get_position(const Node2D* node2d, CTX_AT) {
    CHECK_IF_NULL(node2d)
    return node2d->get_position();
}


class Module_Godot : public das::Module {
public:

    Module_Godot() : Module("godot") {
        das::ModuleLibrary lib(this);

        BIND_NATIVE_BASE(Object)
        BIND_NATIVE_TYPE(Node, Object)
        BIND_NATIVE_TYPE(Node2D, Node)

        das::addExtern<DAS_BIND_FUN(_Node2D_rotate)>(*this, lib, "rotate", das::SideEffects::modifyExternal, "_Node2D_rotate");
        das::addExtern<DAS_BIND_FUN(_Node2D_translate)>(*this, lib, "translate", das::SideEffects::modifyExternal, "_Node2D_translate");
        das::addExtern<DAS_BIND_FUN(_Node2D_get_position)>(*this, lib, "get_position", das::SideEffects::modifyExternal, "_Node2D_get_position");
        das::addExtern<DAS_BIND_FUN(_Node2D_set_position)>(*this, lib, "set_position", das::SideEffects::modifyExternal, "_Node2D_set_position");
        das::addExtern<DAS_BIND_FUN(_Node_get_parent)>(*this, lib, "get_parent", das::SideEffects::modifyExternal, "_Node_get_parent");
        das::addExtern<DAS_BIND_FUN(_Node_find_child)>(*this, lib, "find_child", das::SideEffects::modifyExternal, "_Node_find_child");

        BIND_TYPE_CHECKER(Node)
        BIND_TYPE_CHECKER(Node2D)
        das::addExtern<DAS_BIND_FUN(_check_dascript_type)>(*this, lib, "_check_dascript_type", das::SideEffects::modifyExternal, "_check_dascript_type");

        addAlias(das::typeFactory<Vector2>::make(lib));

        options["tool"] = das::Type::tBool;

        const char* godot_module_full_path = GODOT_DAS_MODULE_PATH"/src/godot.das";
        std::ifstream input{godot_module_full_path, std::ios_base::binary};
        std::vector<char> buffer{std::istreambuf_iterator< char>(input), {} };

        compileBuiltinModuleButWithDaslib(godot_module_full_path, buffer.data(), buffer.size());
    }

    // Module::compileBuiltinModule with das::FsFileAccess instead of das::FileAccess
    // and part of compileDaScript instead of parseDaScript so "require daslib/*" works
    // TODO remove code that is not used
    bool compileBuiltinModuleButWithDaslib (const das::string & modName, char * str, unsigned int str_len ) {
        das::TextWriter issues;
        auto access = das::make_smart<das::FsFileAccess>();
        auto fileInfo = das::make_unique<das::TextFileInfo>((char *) str, uint32_t(str_len), false);
        access->setFileInfo(modName, das::move(fileInfo));
        das::ModuleGroup dummyLibGroup;

        das::vector<das::ModuleInfo> req;
        das::vector<das::string> missing, circular, notAllowed;
        das::das_set<das::string> dependencies;
        getPrerequisits(modName, access, req, missing, circular, notAllowed, dependencies, dummyLibGroup, nullptr, 1, true);
        for ( auto & mod : req ) {
            if ( dummyLibGroup.findModule(mod.moduleName) ) {
                continue;
            }
            auto program = parseDaScript(mod.fileName, access, issues, dummyLibGroup, true, true);
            if ( program->failed() ) {
                return program;
            }
            if ( program->thisModule->name.empty() ) {
                program->thisModule->name = mod.moduleName;
                program->thisModule->wasParsedNameless = true;
            }
            if ( program->promoteToBuiltin ) {
                program->thisModule->promoteToBuiltin(access);
            }
            dummyLibGroup.addModule(program->thisModule.release());
            program->library.foreach([&](Module * pm) -> bool {
                if ( !pm->name.empty() && pm->name!="$" ) {
                    if ( !dummyLibGroup.findModule(pm->name) ) {
                        dummyLibGroup.addModule(pm);
                    }
                }
                return true;
            }, "*");
        }
        auto program = parseDaScript(modName, access, issues, dummyLibGroup, true);


        if ( program ) {
            if (program->failed()) {
                for (auto & err : program->errors) {
                    issues << das::reportError(err.at, err.what, err.extra, err.fixme, err.cerr);
                }
                DAS_FATAL_ERROR("%s\nbuiltin module did not compile %s\n", issues.str().c_str(), modName.c_str());
                return false;
            }
            // ok, now let's rip content
            program->thisModule->aliasTypes.foreach([&](auto aliasTypePtr){
                addAlias(aliasTypePtr);
            });
            program->thisModule->enumerations.foreach([&](auto penum){
                addEnumeration(penum);
            });
            program->thisModule->structures.foreach([&](auto pst){
                addStructure(pst);
            });
            program->thisModule->generics.foreach([&](auto fn){
                addGeneric(fn);
            });
            program->thisModule->globals.foreach([&](auto gvar){
                addVariable(gvar);
            });
            program->thisModule->functions.foreach([&](auto fn){
                addFunction(fn);
            });
            for (auto & rqm : program->thisModule->requireModule) {
                if ( rqm.first != this ) {
                    requireModule[rqm.first] |= rqm.second;
                }
            }
            // macros
            auto ptm = program->thisModule.get();
            if ( ptm->macroContext ) {
                swap ( macroContext, ptm->macroContext );
                ptm->handleTypes.foreach([&](auto fna){
                    addAnnotation(fna);
                });
            }
            simulateMacros.insert(simulateMacros.end(), ptm->simulateMacros.begin(), ptm->simulateMacros.end());
            captureMacros.insert(captureMacros.end(), ptm->captureMacros.begin(), ptm->captureMacros.end());
            forLoopMacros.insert(forLoopMacros.end(), ptm->forLoopMacros.begin(), ptm->forLoopMacros.end());
            variantMacros.insert(variantMacros.end(), ptm->variantMacros.begin(), ptm->variantMacros.end());
            macros.insert(macros.end(), ptm->macros.begin(), ptm->macros.end());
            inferMacros.insert(inferMacros.end(), ptm->inferMacros.begin(), ptm->inferMacros.end());
            optimizationMacros.insert(optimizationMacros.end(), ptm->optimizationMacros.begin(), ptm->optimizationMacros.end());
            lintMacros.insert(lintMacros.end(), ptm->lintMacros.begin(), ptm->lintMacros.end());
            globalLintMacros.insert(globalLintMacros.end(), ptm->globalLintMacros.begin(), ptm->globalLintMacros.end());
            for ( auto & rm : ptm->readMacros ) {
                addReaderMacro(rm.second);
            }
            commentReader = ptm->commentReader;
            for ( auto & op : ptm->options) {
                DAS_ASSERTF(options.find(op.first)==options.end(),"duplicate option %s", op.first.c_str());
                options[op.first] = op.second;
            }
            struct SubstituteModuleRefs : das::Visitor {
                SubstituteModuleRefs ( Module * from, Module * to ) : to(to), from(from) {}
                virtual void preVisit ( das::TypeDecl * td ) {
                    if ( td->module == from ) td->module = to;
                }
                Module * const to;
                Module * const from;
            } subs ( program->thisModule.get(), this );
            program->visit(subs,/*visitGenerics =*/true);
            return true;
        } else {
            DAS_FATAL_ERROR("builtin module did not parse %s\n", modName.c_str());
            return false;
        }
    }
};

REGISTER_MODULE(Module_Godot);


        // // Module::compileBuiltinModule with das::FsFileAccess instead of das::FileAccess
        // // and compileDaScript instead of parseDaScript so "require daslib/*" works
        // // TODO remove code that is not used
        // das::TextWriter issues;
        // auto access = das::make_smart<das::FsFileAccess>();
        // auto fileInfo = das::make_unique<das::TextFileInfo>(buffer.data(),  buffer.size(), false);
        // access->setFileInfo(godot_module_full_path, das::move(fileInfo));
        // das::ModuleGroup dummyLibGroup;
        // auto program = parseDaScript(godot_module_full_path, access, issues, dummyLibGroup, false);
        // if ( program ) {
        //     if (program->failed()) {
        //         for (auto & err : program->errors) {
        //             issues << reportError(err.at, err.what, err.extra, err.fixme, err.cerr);
        //         }
        //         DAS_FATAL_ERROR("%s\nbuiltin module did not compile %s\n", issues.str().c_str(), godot_module_full_path);
        //     }
        //     // ok, now let's rip content
        //     program->thisModule->aliasTypes.foreach([&](auto aliasTypePtr){
        //         addAlias(aliasTypePtr);
        //     });
        //     program->thisModule->enumerations.foreach([&](auto penum){
        //         addEnumeration(penum);
        //     });
        //     program->thisModule->structures.foreach([&](auto pst){
        //         addStructure(pst);
        //     });
        //     program->thisModule->generics.foreach([&](auto fn){
        //         addGeneric(fn);
        //     });
        //     program->thisModule->globals.foreach([&](auto gvar){
        //         addVariable(gvar);
        //     });
        //     program->thisModule->functions.foreach([&](auto fn){
        //         addFunction(fn);
        //     });
        //     for (auto & rqm : program->thisModule->requireModule) {
        //         if ( rqm.first != this ) {
        //             requireModule[rqm.first] |= rqm.second;
        //         }
        //     }
        //     // macros
        //     auto ptm = program->thisModule.get();
        //     if ( ptm->macroContext ) {
        //         swap ( macroContext, ptm->macroContext );
        //         ptm->handleTypes.foreach([&](auto fna){
        //             addAnnotation(fna);
        //         });
        //     }
        //     simulateMacros.insert(simulateMacros.end(), ptm->simulateMacros.begin(), ptm->simulateMacros.end());
        //     captureMacros.insert(captureMacros.end(), ptm->captureMacros.begin(), ptm->captureMacros.end());
        //     forLoopMacros.insert(forLoopMacros.end(), ptm->forLoopMacros.begin(), ptm->forLoopMacros.end());
        //     variantMacros.insert(variantMacros.end(), ptm->variantMacros.begin(), ptm->variantMacros.end());
        //     macros.insert(macros.end(), ptm->macros.begin(), ptm->macros.end());
        //     inferMacros.insert(inferMacros.end(), ptm->inferMacros.begin(), ptm->inferMacros.end());
        //     optimizationMacros.insert(optimizationMacros.end(), ptm->optimizationMacros.begin(), ptm->optimizationMacros.end());
        //     lintMacros.insert(lintMacros.end(), ptm->lintMacros.begin(), ptm->lintMacros.end());
        //     globalLintMacros.insert(globalLintMacros.end(), ptm->globalLintMacros.begin(), ptm->globalLintMacros.end());
        //     for ( auto & rm : ptm->readMacros ) {
        //         addReaderMacro(rm.second);
        //     }
        //     commentReader = ptm->commentReader;
        //     for ( auto & op : ptm->options) {
        //         DAS_ASSERTF(options.find(op.first)==options.end(),"duplicate option %s", op.first.c_str());
        //         options[op.first] = op.second;
        //     }
        //     struct SubstituteModuleRefs : das::Visitor {
        //         SubstituteModuleRefs ( Module * from, Module * to ) : to(to), from(from) {}
        //         virtual void preVisit (das::TypeDecl * td ) {
        //             if ( td->module == from ) td->module = to;
        //         }
        //         Module * const to;
        //         Module * const from;
        //     }subs ( program->thisModule.get(), this );
        //     program->visit(subs,/*visitGenerics =*/true);
        // } else {
        //     DAS_FATAL_ERROR("builtin module did not parse %s\n", godot_module_full_path);
        // }