#pragma once
#include "ExprAST.h"

namespace
{
	class IntegerAST : public ExprAST
	{
		std::string Name;
		int Value;

	public:
		IntegerAST(const std::string& Name, int Value)
			: Name(Name), Value(Value) {}

		llvm::Value* codegen() override
		{
			return nullptr;
		}
	};
}

