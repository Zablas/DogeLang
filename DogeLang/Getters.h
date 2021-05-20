#pragma once

#include <string>
#include "llvm/IR/Function.h"
#include "MapContainers.h"
#include "GlobalPointers.h"

inline llvm::Function* getFunction(std::string Name)
{
    // First, see if the function has already been added to the current module.
    if (auto* F = TheModule->getFunction(Name))
        return F;

    // If not, check whether we can codegen the declaration from some existing
    // prototype.
    auto FI = FunctionProtos.find(Name);
    if (FI != FunctionProtos.end())
        return FI->second->codegen();

    // If no existing prototype exists, return null.
    return nullptr;
}

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
static llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* TheFunction,
    llvm::StringRef VarName) {
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
        TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(llvm::Type::getDoubleTy(*TheContext), nullptr, VarName);
}
