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

#define NAME_NATIVE_TYPE_FACTORY(TYPE) MAKE_TYPE_FACTORY(TYPE##Native, TYPE)

#define STR(X) #X
#define DEFER_STR(X) STR(X)
#define DAS_MEMBER_NAME(CLASS, MEMBER) _##CLASS##_##MEMBER

#define BIND_METHOD(CLASS, METHOD)\
using DAS_MEMBER_NAME(CLASS, METHOD) = DAS_CALL_MEMBER(CLASS::METHOD);\
das::addExtern<DAS_CALL_METHOD(DAS_MEMBER_NAME(CLASS, METHOD))>(*this, lib, DEFER_STR(DAS_MEMBER_NAME(CLASS, METHOD)), das::SideEffects::modifyExternal, DAS_CALL_MEMBER_CPP(CLASS::METHOD));


#define BIND_NATIVE_BASE(TYPE)\
struct TYPE##Annotation : das::ManagedStructureAnnotation<TYPE> {\
    TYPE##Annotation(das::ModuleLibrary & ml) : ManagedStructureAnnotation(#TYPE"Native", ml) { }\
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
                     ManagedStructureAnnotation(#TYPE"Native", ml),\
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
bool check_godot_type(const Object* obj) {
    return dynamic_cast<const T*>(obj) != nullptr;
}

#define BIND_TYPE_CHECKER(TYPE)  das::addExtern<DAS_BIND_FUN(check_godot_type<TYPE>)>(*this, lib, "check_godot_type_"#TYPE, das::SideEffects::none, "check_godot_type<"#TYPE">");


NAME_NATIVE_TYPE_FACTORY(Object)
NAME_NATIVE_TYPE_FACTORY(Node)
NAME_NATIVE_TYPE_FACTORY(Node2D)

template<> struct das::ToBasicType<String>     { enum { type = das::Type::tString }; };

MAKE_TYPE_FACTORY_ALIAS(Vector2, tFloat2);
template <> struct das::cast<Vector2> : das::cast_fVec_half<Vector2> {};

Node* _Node_find_child(const Node& node, const char* p_pattern, bool p_recursive = true, bool p_owned = true) {
    // TODO embed this cast into das?
    // or maybe make all method calls like this
    return node.find_child(p_pattern, p_recursive, p_owned);
}

class Module_Godot : public das::Module {
public:

    Module_Godot() : Module("godot") {
        das::ModuleLibrary lib(this);

        BIND_NATIVE_BASE(Object)
        BIND_NATIVE_TYPE(Node, Object)
        BIND_NATIVE_TYPE(Node2D, Node)

        BIND_METHOD(Node2D, rotate)
        BIND_METHOD(Node2D, translate)
        BIND_METHOD(Node2D, get_position)
        BIND_METHOD(Node2D, set_position)
        BIND_METHOD(Node, get_parent)

        BIND_TYPE_CHECKER(Node)
        BIND_TYPE_CHECKER(Node2D)

        das::addExtern<DAS_BIND_FUN(_Node_find_child)>(*this, lib, "_Node_find_child", das::SideEffects::modifyExternal, "_Node_find_child");

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