// CS4280 Program 3
// Author: Maija Garson
// Date: 04/21/2025
// Description: Implementation of the SymbolTable class for checking static semantics.
// Includes methods to insert new variables, verify existing ones in the table, and print
// all entries of the table.

#include "symbolTable.h"

#include <algorithm>
#include <cstdio>

// Function to insert new variable name into the table. Returns true if insertion is
// successful and false if variable name was already in table.
bool SymbolTable::insert(const std:: string &name)
{
	// Start OR1
	// If name is found in the table, returns false.
	if (std::find(table_.begin(), table_.end(), name) != table_.end())
			return false;
	// Otherwise, adds name to table and returns true.		
	table_.push_back(name);
	return true;
}

// Function to check whether a variable name is already in the table.
// Returns true if name is found in table, false if name is not found.
bool SymbolTable::verify(const std::string &name) const
{
	return std::find(table_.begin(), table_.end(), name) != table_.end();
}

// Function to print all variable names in the table, one per line.
void SymbolTable::print() const
{
	for(std::vector<std::string>::const_iterator it = table_.begin(); it != table_.end(); ++it)
		std::printf("%s\n", it->c_str());
}
// End OR1
