#pragma once

#include <string>
#include "ExprAST.h"
#include "MapContainers.h"
#include "GlobalPointers.h"
#include "LogFunctions.h"

namespace
{
	class VariableExprAST : public ExprAST
	{
		std::string Name;

	public:
		VariableExprAST(const std::string& Name) : Name(Name) {}

		llvm::Value* codegen() override
		{
			// Look this variable up in the function.
			llvm::Value* V = NamedValues[Name];
			if (!V) return LogErrorV("Unknown variable name");

			// Load the value.
			return Builder->CreateLoad(llvm::Type::getDoubleTy(*TheContext), V, Name.c_str());
		}
		const std::string& getName() const { return Name; }
	};
}

