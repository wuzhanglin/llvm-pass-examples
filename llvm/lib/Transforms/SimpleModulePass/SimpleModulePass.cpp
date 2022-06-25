// SimpleModulePass.cpp

#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include  <iostream>

using namespace llvm;
using std::cout;
using std::endl;

namespace
{
    class SimpleModulePass : public PassInfoMixin<SimpleModulePass>
    {
    public:

        PreservedAnalyses run(Module& module, ModuleAnalysisManager& analysisManager)
        {
            cout << "Module: " << module.getName().str() << endl;
            cout << "    ID: " << module.getModuleIdentifier() << endl;
            cout << "    Source File Name: " << module.getSourceFileName() << endl;
            cout << "    Instruction Count: " << module.getInstructionCount() << endl;

            // Print out all the global variables in this module
            cout << endl << "    Gloabl Variable Count: " << module.getGlobalList().size() << endl;
            for (const auto& globalVariable : module.getGlobalList())
            {
                cout << "        Global Variable: " << globalVariable.getName().str() << endl;
            }

            // Print out all the functions in this module
            cout << endl << "    Function Count: " << module.getFunctionList().size() << endl;
            for (const auto& function : module)
            {
                cout << "        Function: " << function.getName().str() << endl;
            }

            // Assuming we did not change anything of the IR code
            return PreservedAnalyses::all();
        }
    };
}

// This is the new way of registering our pass
extern "C" PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo()
{
    return
    {
        LLVM_PLUGIN_API_VERSION,
        "SimpleModulePass",
        "v0.1",
        [](PassBuilder& passBuilder) {
            passBuilder.registerPipelineParsingCallback(
                [](StringRef name, ModulePassManager& passManager, ArrayRef<PassBuilder::PipelineElement>) {
                    if(name == "simple-module-pass")
                    {
                        passManager.addPass(SimpleModulePass());
                        return true;
                    }

                    return false;
                }
            );
        }
    };
}
