#include <daScript/daScript.h>

class MyModule : public das::Module {
public:
    bool compileBuiltinModuleButWithDaslib (const das::string & modName, char * str, unsigned int str_len );
};


// Module::compileBuiltinModule with das::FsFileAccess instead of das::FileAccess
// and part of compileDaScript instead of parseDaScript so "require daslib/*" works
// TODO remove code that is not used
bool MyModule::compileBuiltinModuleButWithDaslib (const das::string & modName, char * str, unsigned int str_len ) {
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