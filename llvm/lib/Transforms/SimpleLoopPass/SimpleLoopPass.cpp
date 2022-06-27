// SimpleLoopPass.cpp

#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>

using namespace llvm;
using std::cout;
using std::endl;

namespace
{
    class SimpleLoopPass : public PassInfoMixin<SimpleLoopPass>
    {
    public:

        PreservedAnalyses run(Loop& loop,
                              LoopAnalysisManager& analysisManager,
                              LoopStandardAnalysisResults& analysisResults,
                              LPMUpdater& updater)
        {
            cout << "Loop: " << loop.getName().str() << endl;

            // Print out all the sub-loops in this loop
            cout << endl << "    Sub-Loop Count: " << loop.getSubLoops().size() << endl;
            for (const auto& subLoop : loop)
            {
                cout << "        Sub-Loop: " << subLoop->getName().str() << endl;
            }

            // Print out all the basic blocks in this loop
            cout << endl << "    Basic Block Count: " << loop.getNumBlocks() << endl;
            for (const auto& basicBlock : loop.getBlocks())
            {
                cout << "        Basic Block: " << basicBlock->getName().str() << endl;
            }

            cout << endl << endl;

            // Assuming we did not change anything of the IR code
            return PreservedAnalyses::all();
        }
    };
} // namespace

// This is the new way of registering our pass
extern "C" PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo()
{
    return {LLVM_PLUGIN_API_VERSION, "SimpleLoopPass", "v0.1", [](PassBuilder& passBuilder) {
                passBuilder.registerPipelineParsingCallback(
                    [](StringRef name, LoopPassManager& passManager, ArrayRef<PassBuilder::PipelineElement>) {
                        if (name == "simple-loop-pass")
                        {
                            passManager.addPass(SimpleLoopPass());
                            return true;
                        }

                        return false;
                    });
            }};
}
