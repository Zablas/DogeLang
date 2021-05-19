#pragma once

#include <string>
#include "ExprAST.h"

namespace
{
	class VariableExprAST : public ExprAST
	{
		std::string Name;

	public:
		VariableExprAST(const std::string& Name) : Name(Name) {}

		llvm::Value* codegen() override;
		const std::string& getName() const { return Name; }
	};
}

