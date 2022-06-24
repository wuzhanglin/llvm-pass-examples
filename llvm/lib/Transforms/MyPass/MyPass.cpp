// MyPass.cpp

#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

// Only needed for printing
#include  <iostream>

using namespace llvm;

namespace
{
    class MyPass : public PassInfoMixin<MyPass>
    {
    public:

        // The first argument of the run() function defines on what level
        // of granularity your pass will run (e.g. Module, Function).
        // The second argument is the corresponding AnalysisManager
        // (e.g ModuleAnalysisManager, FunctionAnalysisManager)
        PreservedAnalyses run(Function& function, FunctionAnalysisManager& analysisManager)
        {
            std::cout << "MyPass in function: " << function.getName().str() << std::endl;

            // Here goes what you want to do with a pass

            // Assuming we did not change anything of the IR code
            return PreservedAnalyses::all();
        }
    };
}

// This part is the new way of registering your pass
extern "C" PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo()
{
    return
    {
        LLVM_PLUGIN_API_VERSION,
        "MyPass",
        "v0.1",
        [](PassBuilder &passBuilder) {
            passBuilder.registerPipelineParsingCallback(
                [](StringRef name, FunctionPassManager &passManager, ArrayRef<PassBuilder::PipelineElement>) {
                    if(name == "my-pass")
                    {
                        passManager.addPass(MyPass());
                        return true;
                    }

                    return false;
                }
            );
        }
    };
}
