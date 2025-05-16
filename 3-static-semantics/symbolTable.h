// CS4280 Project 3
// Author: Maija Garson
// Date: 04/21/2025
// Description: Header file for SymbolTable class, which manages a table of declared variables.
// The constructor initializes a vector representing the table for variable names. 
// Declares functions to insert a new name (ensuring uniqueness), verify a name's presence, and print all entries.

#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include <string>
#include <vector>

class SymbolTable 
{
	public:
		// Add new name to table. Returns false if name is already in table. 
		// Otherwise, returns true and inserts name.
		bool insert(const std::string &name);
		// Verify a name exists in table. Returns true if name is found.
		bool verify(const std::string &name) const;

		// Output all variable names in table, one per line.
		void print() const;

	private:
		// Vector to hold declared variable names.
		std::vector<std::string> table_;
};

#endif 
