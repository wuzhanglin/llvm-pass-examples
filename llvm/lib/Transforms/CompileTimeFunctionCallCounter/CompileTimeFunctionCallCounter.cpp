// CompileTimeFunctionCallCounter.cpp

#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

#include <iostream>
#include <map>

using namespace llvm;
using FunctionCalls = std::map<const Function*, size_t>;

// A LLVM Pass to analyse the function calls
class CompileTimeFunctionCallCounter : public AnalysisInfoMixin<CompileTimeFunctionCallCounter>
{
public:

    using Result = FunctionCalls;

    Result run(Module& module, ModuleAnalysisManager&);

private:

    friend struct AnalysisInfoMixin<CompileTimeFunctionCallCounter>;

    // A special key to identify a particular analysis pass type
    static AnalysisKey Key;
};

// A LLVM Pass to print the function call statistics
class CompileTimeFunctionCallPrinter : public PassInfoMixin<CompileTimeFunctionCallPrinter>
{
public:

    PreservedAnalyses run(Module& module, ModuleAnalysisManager& analysisManager);
};

AnalysisKey CompileTimeFunctionCallCounter::Key;

CompileTimeFunctionCallCounter::Result CompileTimeFunctionCallCounter::run(Module& module, ModuleAnalysisManager&)
{
    Result functionCalls;

    for (const Function& function : module)
    {
        for (const BasicBlock& basicBlock : function)
        {
            for (const Instruction& instruction : basicBlock)
            {
                const CallBase* functionCallInstruction = dyn_cast<CallBase>(&instruction);
                if (nullptr == functionCallInstruction)
                {
                    // Ignore instructions that are not function calls
                    continue;
                }

                // We can get the called function if this is a direct function call
                const Function* calledFunction = functionCallInstruction->getCalledFunction();
                if (nullptr == calledFunction)
                {
                    continue;
                }

                auto functionCallIterator = functionCalls.find(calledFunction);
                if (functionCalls.end() == functionCallIterator)
                {
                    functionCallIterator = functionCalls.insert(std::make_pair(calledFunction, 0)).first;
                }

                ++functionCallIterator->second;
            }
        }
    }

    return functionCalls;
}

PreservedAnalyses CompileTimeFunctionCallPrinter::run(Module& module, ModuleAnalysisManager& analysisManager)
{
    auto functionCalls = analysisManager.getResult<CompileTimeFunctionCallCounter>(module);
    for (auto& functionCall : functionCalls)
    {
        std::cout << "Function: " << functionCall.first->getName().str() << ", "
                  << "called " << functionCall.second << " times" << std::endl;
    }

    // Assuming we did not change anything of the IR code
    return PreservedAnalyses::all();
}

// Pass registration
extern "C" LLVM_ATTRIBUTE_WEAK ::PassPluginLibraryInfo llvmGetPassPluginInfo()
{
    return {
        LLVM_PLUGIN_API_VERSION, "CompileTimeFunctionCallPrinter", LLVM_VERSION_STRING, [](PassBuilder& passBuilder) {
            // 1. Registration for "opt -passes="compile-time-function-call-counter""
            passBuilder.registerPipelineParsingCallback(
                [&](StringRef name, ModulePassManager& passManager, ArrayRef<PassBuilder::PipelineElement>) {
                    if (name == "compile-time-function-call-counter")
                    {
                        passManager.addPass(CompileTimeFunctionCallPrinter());
                        return true;
                    }

                    return false;
                });

            // 2. Registration for "analysisManager.getResult<CompileTimeFunctionCallCounter>(module)"
            passBuilder.registerAnalysisRegistrationCallback([](ModuleAnalysisManager& analysisManager) {
                analysisManager.registerPass([&]() {
                    return CompileTimeFunctionCallCounter();
                });
            });
        }};
}
