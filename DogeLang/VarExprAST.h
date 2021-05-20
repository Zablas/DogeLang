#pragma once

#include <string>
#include <vector>
#include "ExprAST.h"
#include "GlobalPointers.h"
#include "Getters.h"
#include "MapContainers.h"

namespace
{
	class VarExprAST : public ExprAST
	{
		std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> VarNames;
		std::unique_ptr<ExprAST> Body;

	public:
		VarExprAST(
			std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> VarNames,
			std::unique_ptr<ExprAST> Body)
			: VarNames(std::move(VarNames)), Body(std::move(Body)) {}

		llvm::Value* codegen() override
		{
			std::vector<llvm::AllocaInst*> OldBindings;

			llvm::Function* TheFunction = Builder->GetInsertBlock()->getParent();

			// Register all variables and emit their initializer.
			for (unsigned i = 0, e = VarNames.size(); i != e; ++i)
			{
				const std::string& VarName = VarNames[i].first;
				ExprAST* Init = VarNames[i].second.get();

				// Emit the initializer before adding the variable to scope, this prevents
				// the initializer from referencing the variable itself, and permits stuff
				// like this:
				//  var a = 1 in
				//    var a = a in ...   # refers to outer 'a'.
				llvm::Value* InitVal;
				if (Init)
				{
					InitVal = Init->codegen();
					if (!InitVal)
						return nullptr;
				}
				else // If not specified, use 0.0.
					InitVal = llvm::ConstantFP::get(*TheContext, llvm::APFloat(0.0));

				llvm::AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, VarName);
				Builder->CreateStore(InitVal, Alloca);

				// Remember the old variable binding so that we can restore the binding when
				// we unrecurse.
				OldBindings.push_back(NamedValues[VarName]);

				// Remember this binding.
				NamedValues[VarName] = Alloca;
			}

			// Codegen the body, now that all vars are in scope.
			llvm::Value* BodyVal = Body->codegen();
			if (!BodyVal)
				return nullptr;

			// Pop all our variables from scope.
			for (unsigned i = 0, e = VarNames.size(); i != e; ++i)
				NamedValues[VarNames[i].first] = OldBindings[i];

			// Return the body computation.
			return BodyVal;
		}
	};
}

