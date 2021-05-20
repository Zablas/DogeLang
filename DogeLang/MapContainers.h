#pragma once

#include <map>
#include <string>
#include "llvm/IR/Instructions.h"
#include "PrototypeAST.h"

static std::map<std::string, llvm::AllocaInst*> NamedValues;
static std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

/// BinopPrecedence - This holds the precedence for each binary operator that is defined.
static std::map<char, int> BinopPrecedence;
