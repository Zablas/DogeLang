#pragma once

#include <map>
#include <string>
#include "llvm/IR/Instructions.h"
#include "PrototypeAST.h"

static std::map<std::string, llvm::AllocaInst*> NamedValues;
static std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;
static std::map<char, int> BinopPrecedence;
