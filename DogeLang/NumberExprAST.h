#pragma once

#include "ExprAST.h"
#include "GlobalPointers.h"

namespace
{
	class NumberExprAST : public ExprAST
	{
		double Val;

	public:
		NumberExprAST(double Val) : Val(Val) {}
		llvm::Value* codegen() override { return llvm::ConstantFP::get(*TheContext, llvm::APFloat(Val)); }
	};
}

