// CS4280 Project 4
// Author: Maija Garson
// Date: 05/11/2025
// Description:Header file for the code generation program. Declares the codeGen function which traverses
// the parse tree and generates assembly instructions into the given output file, using the provided symbol
// table to initialize user-declared variables. It also initializes all temporary variables.

#ifndef _CODE_GEN_H_
#define _CODE_GEN_H_

#include <cstdio>
#include "node.h"
#include "symbolTable.h"

// Generates assembly from the passed root of the parse tree. Writes output to passed file pointer 'out'.
// Uses passed symbol table to declare all user-declared variables. 
void codeGen(Node *root, FILE *out, const SymbolTable &symTab);

#endif
