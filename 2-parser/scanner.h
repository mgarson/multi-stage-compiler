// CS4280 Project 2
// Author: Maija Garson
// Date: 04/13/2025
// Description: Header file for scanner program that declares functions that will scan in and process tokens. 
// Also defines structure that represents finite state automation table to assist with token transitions.

#ifndef _SCANNER_H_
#define _SCANNER_H

#include <fstream>
#include <string>

#include "token.h"

// Function to scan in from input and return token
token scanner(FILE *, int &lineNum);

// Function to determine char's type
int getCharType(char c);

// Function to process token and return processed token based on passed paramaters, which respectively represent instance, token type, and line number. 
token processToken(std::string tokenInstance, int tokenType, int lineNum);

// Table representing finite state automation driver to map token states
extern int fsaTable[6][6];

#endif
