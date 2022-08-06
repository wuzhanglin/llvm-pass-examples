// ConstantAdditionCombiner.cpp

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
    /**
     * An analysis to collect all the "add" instructions with constant operands from a function
     */
    struct ConstantAdditionAnalysis : public llvm::AnalysisInfoMixin<ConstantAdditionAnalysis>
    {
        using Result = llvm::SmallVector<llvm::BinaryOperator*, 0>;

        Result run(llvm::Function& function, llvm::FunctionAnalysisManager& analysisManager);

        // A special type used by analysis passes to provide an address that
        // identifies that particular analysis pass type
        static llvm::AnalysisKey Key;
    };

    AnalysisKey ConstantAdditionAnalysis::Key;

    bool IsConstantIntOnly(Instruction& instruction)
    {
        for (Use& operand : instruction.operands())
        {
            if (!isa<ConstantInt>(operand))
                return false;
        }

        return true;
    }

    ConstantAdditionAnalysis::Result ConstantAdditionAnalysis::run(Function& function,
                                                                   FunctionAnalysisManager& analysisManager)
    {
        SmallVector<BinaryOperator*, 0> addInstructions;
        for (BasicBlock& block : function)
        {
            for (Instruction& instruction : block)
            {
                if (!instruction.isBinaryOp())
                    continue;

                if (instruction.getOpcode() != Instruction::BinaryOps::Add)
                    continue;

                if (!IsConstantIntOnly(instruction))
                    continue;

                addInstructions.push_back(&cast<BinaryOperator>(instruction));
            }
        }

        return addInstructions;
    }

    /**
     * An pass to print all the "add" instructions with constant operands from a function
     */
    struct ConstantAdditionPrinterPass : public llvm::PassInfoMixin<ConstantAdditionPrinterPass>
    {
        explicit ConstantAdditionPrinterPass(llvm::raw_ostream& outStream) :
            out(outStream)
        {
        }

        llvm::PreservedAnalyses run(llvm::Function& function, llvm::FunctionAnalysisManager& analysisManager)
        {
            auto& addInstructions = analysisManager.getResult<ConstantAdditionAnalysis>(function);

            out << "Function: " << function.getName() << "\n";
            for (auto& instruction : addInstructions)
            {
                out << *instruction << "\n";
            }

            return PreservedAnalyses::all();
        }

    private:

        llvm::raw_ostream& out;
    };

    struct ConstantAdditionCombinerPass : public llvm::PassInfoMixin<ConstantAdditionCombinerPass>
    {
        llvm::PreservedAnalyses run(llvm::Function& function, llvm::FunctionAnalysisManager& analysisManager);
    };

    void ReplaceAddInstructionWithConstant(BinaryOperator* binaryOperator)
    {
        auto first = cast<ConstantInt>(binaryOperator->getOperand(0));
        auto second = cast<ConstantInt>(binaryOperator->getOperand(1));

        // Get the final value from the "add" instruction
        auto sum = ConstantExpr::getAdd(first, second);

        binaryOperator->replaceAllUsesWith(sum);
        binaryOperator->eraseFromParent();
    }

    PreservedAnalyses ConstantAdditionCombinerPass::run(Function& function, FunctionAnalysisManager& analysisManager)
    {
        auto& addInstructions = analysisManager.getResult<ConstantAdditionAnalysis>(function);
        for (auto binaryOperator : addInstructions)
        {
            ReplaceAddInstructionWithConstant(binaryOperator);
        }

        auto preservedAnalyses = PreservedAnalyses::all();
        preservedAnalyses.abandon<ConstantAdditionAnalysis>();
        return preservedAnalyses;
    }
} // namespace

// Register the passes
extern "C" PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo()
{
    return {LLVM_PLUGIN_API_VERSION, "ConstantAdditionCombiner", "v0.1", [](PassBuilder& passBuilder) {
                passBuilder.registerAnalysisRegistrationCallback([](FunctionAnalysisManager& analysisManager) {
                    analysisManager.registerPass([] {
                        return ConstantAdditionAnalysis();
                    });
                });

                passBuilder.registerPipelineParsingCallback(
                    [](StringRef name, FunctionPassManager& passManager, ArrayRef<PassBuilder::PipelineElement>) {
                        if (name == "constant-addition-printer")
                        {
                            passManager.addPass(ConstantAdditionPrinterPass(outs()));
                            return true;
                        }

                        if (name == "constant-addition-combiner")
                        {
                            passManager.addPass(ConstantAdditionCombinerPass());
                            return true;
                        }

                        return false;
                    });
            }};
}
