#pragma once

#include <string>
#include "llvm/Support/Error.h"

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number
static int CurTok;
static llvm::ExitOnError ExitOnErr;
