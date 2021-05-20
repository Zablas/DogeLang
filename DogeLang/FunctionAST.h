#pragma once

#include <memory>
#include "llvm/IR/Verifier.h"
#include "PrototypeAST.h"
#include "ExprAST.h"
#include "MapContainers.h"
#include "Getters.h"
#include "GlobalPointers.h"

namespace
{
	class FunctionAST
	{
        std::unique_ptr<PrototypeAST> Proto;
        std::unique_ptr<ExprAST> Body;

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto,
            std::unique_ptr<ExprAST> Body)
            : Proto(std::move(Proto)), Body(std::move(Body)) {}

        llvm::Function* codegen()
        {
            // Transfer ownership of the prototype to the FunctionProtos map, but keep a
            // reference to it for use below.
            auto& P = *Proto;
            FunctionProtos[Proto->getName()] = std::move(Proto);
            llvm::Function* TheFunction = getFunction(P.getName());
            if (!TheFunction)
                return nullptr;

            // If this is an operator, install it.
            if (P.isBinaryOp())
                BinopPrecedence[P.getOperatorName()] = P.getBinaryPrecedence();

            // Create a new basic block to start insertion into.
            llvm::BasicBlock* BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
            Builder->SetInsertPoint(BB);

            // Record the function arguments in the NamedValues map.
            NamedValues.clear();
            for (auto& Arg : TheFunction->args())
            {
                // Create an alloca for this variable.
                llvm::AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, Arg.getName());

                // Store the initial value into the alloca.
                Builder->CreateStore(&Arg, Alloca);

                // Add arguments to variable symbol table.
                NamedValues[std::string(Arg.getName())] = Alloca;
            }

            if (llvm::Value* RetVal = Body->codegen())
            {
                // Finish off the function.
                Builder->CreateRet(RetVal);

                // Validate the generated code, checking for consistency.
                verifyFunction(*TheFunction);

                return TheFunction;
            }

            // Error reading body, remove function.
            TheFunction->eraseFromParent();

            if (P.isBinaryOp())
                BinopPrecedence.erase(P.getOperatorName());
            return nullptr;
        }
	};
}

