// CS4280 Project 4
// Author: Maija Garson
// Date: 05/11/2025
// Description: Header file for static semantic checking. Declares the checkSemantics function, which
// traverses the parse tree to ensure variables follow declaration and usage rules.

#ifndef _STATIC_SEMANTIC_H_
#define _STATIC_SEMANTIC_H_

#include "node.h"
#include "symbolTable.h"

void checkSemantics(Node *root, SymbolTable &table);

#endif
