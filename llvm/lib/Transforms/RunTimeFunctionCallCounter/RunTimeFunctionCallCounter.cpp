// RunTimeFunctionCallCounter.cpp

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"

#include <iostream>
#include <map>

using namespace llvm;

#define DEBUG_TYPE "runtime-function-call-counter"

namespace helper
{
    Constant* CreateGlobalVariable(Module& module, StringRef globalVariableName);
    bool CountFunctionCallsInModule(Module& module);
} // namespace helper

// A LLVM Pass to count the function calls at runtime
class RunTimeFunctionCallCounter : public PassInfoMixin<RunTimeFunctionCallCounter>
{
public:

    PreservedAnalyses run(Module& module, ModuleAnalysisManager&)
    {
        bool changed = helper::CountFunctionCallsInModule(module);
        return changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
    }
};

// Pass registration
extern "C" LLVM_ATTRIBUTE_WEAK ::PassPluginLibraryInfo llvmGetPassPluginInfo()
{
    return {
        LLVM_PLUGIN_API_VERSION, "Run-Time Function Call Counter", LLVM_VERSION_STRING, [](PassBuilder& passBuilder) {
            passBuilder.registerPipelineParsingCallback(
                [](StringRef name, ModulePassManager& passManager, ArrayRef<PassBuilder::PipelineElement>) {
                    if (name == "runtime-function-call-counter")
                    {
                        passManager.addPass(RunTimeFunctionCallCounter());
                        return true;
                    }

                    return false;
                });
        }};
}

Constant* helper::CreateGlobalVariable(Module& module, StringRef globalVariableName)
{
    auto& context = module.getContext();

    // This will insert a declaration into module
    Constant* newGlobalVariable = module.getOrInsertGlobal(globalVariableName, IntegerType::getInt32Ty(context));

    // This will change the declaration into definition (and initialize to 0)
    GlobalVariable* initializedGlobalVariable = module.getNamedGlobal(globalVariableName);
    initializedGlobalVariable->setLinkage(GlobalValue::CommonLinkage);
    initializedGlobalVariable->setAlignment(MaybeAlign(4));
    initializedGlobalVariable->setInitializer(ConstantInt::get(context, APInt(32, 0)));

    return newGlobalVariable;
}

bool helper::CountFunctionCallsInModule(Module& module)
{
    auto& context = module.getContext();

    // Function name to IR variable map that holds the call counters
    StringMap<Constant*> callCounterMap;

    // Function name to IR variable map that holds the function names
    StringMap<Constant*> functionNameMap;

    // Step 1. For each function in the module, inject the code for call-counting
    for (Function& function : module)
    {
        if (function.isDeclaration())
        {
            continue;
        }

        // Get an IR builder and set the insertion point to the top of the function
        IRBuilder<> counterBuilder(&*function.getEntryBlock().getFirstInsertionPt());

        // Create a global variable to count the calls to this function
        std::string counterName = "counter_" + function.getName().str();
        Constant* counterVariable = helper::CreateGlobalVariable(module, counterName);
        callCounterMap[function.getName()] = counterVariable;

        // Create a global variable to hold the name of this function
        Constant* functionName = counterBuilder.CreateGlobalStringPtr(function.getName(), "name_" + function.getName());
        functionNameMap[function.getName()] = functionName;

        // Inject instruction to increment the call count each time this function executes
        LoadInst* counteCurrentValue = counterBuilder.CreateLoad(counterVariable);
        Value* counterNextValue = counterBuilder.CreateAdd(counterBuilder.getInt32(1), counteCurrentValue);
        counterBuilder.CreateStore(counterNextValue, counterVariable);

        // Let the opt tool print out some debug information
        // (Visible only if we pass "-debug" to the command and have an assert build.)
        LLVM_DEBUG(dbgs() << "Instrumented: " << function.getName() << "\n");
    }

    // Stop here if there is no function definition in this module
    if (callCounterMap.size() == 0)
    {
        return false;
    }

    // Step 2. Inject the declaration of "printf()"
    //
    // Create (or get) the following declaration in the IR module:
    //    declare i32 @printf(i8*, ...)
    //
    // It corresponds to the following C declaration:
    //    int printf(char*, ...)
    PointerType* printfArgType = PointerType::getUnqual(Type::getInt8Ty(context));
    FunctionType* printfFunctionType = FunctionType::get(IntegerType::getInt32Ty(context),
                                                         printfArgType,
                                                         /*IsVarArgs=*/true);
    FunctionCallee printfCallee = module.getOrInsertFunction("printf", printfFunctionType);

    // Step 3. Inject a global variable that will hold the printf format string
    Constant* formatString = ConstantDataArray::getString(context, "Function: %s, called %d times\n");
    Constant* formatStringVariable = module.getOrInsertGlobal("", formatString->getType());
    dyn_cast<GlobalVariable>(formatStringVariable)->setInitializer(formatString);

    // Step 4. Define a printf wrapper that will print the results
    //
    // Define `PrintfWrapper` that will print the results stored in functionNameMap
    // and callCounterMap. It is equivalent to the following C++ function:
    // ```
    //     void PrintfWrapper()
    //     {
    //          for (auto &item : functions)
    //          {
    //              printf("Function: %s, called %d times\n", item.name, item.count);
    //          }
    //     }
    // ```
    // ("item.name" comes from functionNameMap, "item.count" comes from callCounterMap.)
    FunctionType* printfWrapperType = FunctionType::get(Type::getVoidTy(context),
                                                        {},
                                                        /*IsVarArgs=*/false);
    Function* printfWrapperFunction =
        dyn_cast<Function>(module.getOrInsertFunction("PrintfWrapper", printfWrapperType).getCallee());
    BasicBlock* enterBlock = BasicBlock::Create(context, "enter", printfWrapperFunction);
    IRBuilder<> printfWrapperBuilder(enterBlock);

    Value* formatStringPtr = printfWrapperBuilder.CreatePointerCast(formatStringVariable, printfArgType);
    for (auto& item : callCounterMap)
    {
        Constant* functionName = functionNameMap[item.first()];
        LoadInst* counterValue = printfWrapperBuilder.CreateLoad(item.second);
        printfWrapperBuilder.CreateCall(printfCallee, {formatStringPtr, functionName, counterValue});
    }

    printfWrapperBuilder.CreateRetVoid();

    // Step 5. Call `PrintfWrapper` at the very end of this module
    appendToGlobalDtors(module, printfWrapperFunction, /*Priority=*/0);

    return true;
}
