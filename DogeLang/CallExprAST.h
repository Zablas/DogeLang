#pragma once

#include <vector>
#include <string>
#include "ExprAST.h"
#include "GlobalPointers.h"
#include "Getters.h"
#include "LogFunctions.h"

namespace
{
	class CallExprAST : public ExprAST
	{
        std::string Callee;
        std::vector<std::unique_ptr<ExprAST>> Args;

    public:
        CallExprAST(const std::string& Callee,
            std::vector<std::unique_ptr<ExprAST>> Args)
            : Callee(Callee), Args(std::move(Args)) {}

        llvm::Value* codegen() override
        {
            // Look up the name in the global module table.
            llvm::Function* CalleeF = getFunction(Callee);
            if (!CalleeF)
                return LogErrorV("Unknown function referenced");

            // If argument mismatch error.
            if (CalleeF->arg_size() != Args.size())
                return LogErrorV("Incorrect # arguments passed");

            std::vector<llvm::Value*> ArgsV;
            for (unsigned i = 0, e = Args.size(); i != e; ++i)
            {
                ArgsV.push_back(Args[i]->codegen());
                if (!ArgsV.back())
                    return nullptr;
            }

            return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
        }
	};
}

