#pragma once

#include "ExprAST.h"
#include "LogFunctions.h"
#include "GlobalPointers.h"
#include "Getters.h"

namespace
{
	class UnaryExprAST : public ExprAST
	{
        char Opcode;
        std::unique_ptr<ExprAST> Operand;

    public:
        UnaryExprAST(char Opcode, std::unique_ptr<ExprAST> Operand)
            : Opcode(Opcode), Operand(std::move(Operand)) {}

        llvm::Value* codegen() override
        {
            llvm::Value* OperandV = Operand->codegen();
            if (!OperandV)
                return nullptr;

            llvm::Function* F = getFunction(std::string("unary") + Opcode);
            if (!F)
                return LogErrorV("Unknown unary operator");

            return Builder->CreateCall(F, OperandV, "unop");
        }
	};
}

