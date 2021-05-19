#pragma once

#include <string>
#include <vector>
#include <cassert>
#include "llvm/IR/Function.h"

namespace
{
	class PrototypeAST
	{
        std::string Name;
        std::vector<std::string> Args;
        bool IsOperator;
        unsigned Precedence; // Precedence if a binary op.

    public:
        PrototypeAST(const std::string& Name, std::vector<std::string> Args,
            bool IsOperator = false, unsigned Prec = 0)
            : Name(Name), Args(std::move(Args)), IsOperator(IsOperator),
            Precedence(Prec) {}

        llvm::Function* codegen();
        const std::string& getName() const { return Name; }

        bool isUnaryOp() const { return IsOperator && Args.size() == 1; }
        bool isBinaryOp() const { return IsOperator && Args.size() == 2; }

        char getOperatorName() const {
            assert(isUnaryOp() || isBinaryOp());
            return Name[Name.size() - 1];
        }

        unsigned getBinaryPrecedence() const { return Precedence; }
	};
}

