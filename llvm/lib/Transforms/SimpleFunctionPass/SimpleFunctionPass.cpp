// SimpleFunctionPass.cpp

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
    class SimpleFunctionPass : public PassInfoMixin<SimpleFunctionPass>
    {
    public:

        PreservedAnalyses run(Function& function, FunctionAnalysisManager& analysisManager)
        {
            cout << "Function: " << function.getName().str() << endl;
            cout << "    Instruction Count: " << function.getInstructionCount() << endl;

            // Print out all the basic blocks in this function
            cout << endl << "    Basic Block Count: " << function.getBasicBlockList().size() << endl;
            for (const auto& basicBlock : function)
            {
                cout << "        BasicBlock: " << basicBlock.getName().str() << endl;
            }

            // Print out all the arguments of this function
            cout << endl << "    Argument Count: " << function.arg_size() << endl;
            for (const auto& arg : function.args())
            {
                cout << "        Argument: " << arg.getName().str() << endl;
            }

            cout << endl << "    Function Type: " << endl;
            function.getFunctionType()->print(outs());
            cout << endl << endl;

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
        "SimpleFunctionPass",
        "v0.1",
        [](PassBuilder& passBuilder) {
            passBuilder.registerPipelineParsingCallback(
                [](StringRef name, FunctionPassManager& passManager, ArrayRef<PassBuilder::PipelineElement>) {
                    if(name == "simple-function-pass")
                    {
                        passManager.addPass(SimpleFunctionPass());
                        return true;
                    }

                    return false;
                }
            );
        }
    };
}
