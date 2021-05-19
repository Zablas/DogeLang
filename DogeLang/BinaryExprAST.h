#pragma once

#include "ExprAST.h"

namespace
{
	class BinaryExprAST : public ExprAST
	{
        char Op;
        std::unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
            std::unique_ptr<ExprAST> RHS)
            : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

        llvm::Value* codegen() override;
	};
}

