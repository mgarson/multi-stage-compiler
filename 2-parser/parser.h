// CS4280 Project 2
// Author: Maija Garson
// Date: 04/13/2025
// Description: Header file for the parser program. Declares the main parser function and the functions corresponding to nonterminal
// production rules based off of the grammar for this project, which will use recursive descent parsing to build a parse tree.
// It also declares the match function that is used to validate tokens during parsing.

#ifndef _PARSER_H_
#define _PARSER_H_

#include <cstdio>

#include "node.h"
#include "token.h"

Node* parser(FILE* file);

Node* parseS();
Node* parseA();
Node* parseB();
Node* parseC();
Node* parseD();
Node* parseE();
Node* parseF();
Node* parseG();

void match(tokenID expct);

#endif
