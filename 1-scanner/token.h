// CS4280 Proect 1
// Author: Maija Garson
// Description: Header file to define token structure and enumerate different token types

#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>

// Enum that represents different token types
enum tokenID 
{
	tk1 = 0,
	tk2,
	tk3,
	EOFtk
};

// Array of token names where the index corresponds to the token's type
extern const char* tokenNames[];

// Struct to represent a token in the scanner
struct token
{
	tokenID id; // Token type from enum
	std::string tokenInstance; // Represent token's instance (actual string from token)
	int lineNum; // Represent line number where token was found
};



#endif
