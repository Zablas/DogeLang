#pragma once

#include "llvm/IR/Value.h"

namespace
{
	class ExprAST
	{
	public:
		virtual ~ExprAST() = default;

		virtual llvm::Value* codegen() = 0;
	};
}

