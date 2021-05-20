#pragma once

#include "Parsers.h"
#include "llvm/Support/raw_ostream.h"

static void HandleDefinition()
{
    if (auto FnAST = ParseDefinition())
    {
        if (auto* FnIR = FnAST->codegen())
        {
            fprintf(stderr, "Read function definition:");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    }
    else // Skip token for error recovery.
        getNextToken();
}

static void HandleExtern()
{
    if (auto ProtoAST = ParseExtern())
    {
        if (auto* FnIR = ProtoAST->codegen())
        {
            fprintf(stderr, "Read extern: ");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
            FunctionProtos[ProtoAST->getName()] = std::move(ProtoAST);
        }
    }
    else // Skip token for error recovery.
        getNextToken();
}

static void HandleTopLevelExpression()
{
    if (CurTok == tok_int)
    {
        if (auto GlobalVariableAST = ParseGlobalVariable())
        {
            GlobalVariableAST->codegen();
            return;
        }
        //else getNextToken();
    }
    else
    {
        // Evaluate a top-level expression into an anonymous function.
        if (auto FnAST = ParseTopLevelExpr())
        {
            FnAST->codegen();
            return;
        }
        //else getNextToken();
    }

    getNextToken(); // Skip token for error recovery.
}
