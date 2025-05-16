// CS4280 Project 3
// Author: Maija Garson
// Date: 04/21/2025
// Description: Program that implements static semantic checking on the parse tree produced by the parser.
// It recursively traverses each node in the tree to ensure variable declaration and usage rules are followed.
// Variables (tk2 tokens) are declared through either C nodes (using "#") or A nodes (using "\""). The variable is 
// then inserted into the symbol table. This program ensures variables are not redeclared or undefined by 
// printing an error message and exiting in these cases.

#include "staticSemantic.h"

#include <cstdio>
#include <cstdlib>


// Function to check static semantics. Recursively traverses the parse tree to ensure variables are declared and
// used properly. Variables (tk2 tokens) can be declared through either nodes A (using "\"") or C using ("#") and 
// inserted into the symbol table. If a variable is redefined, prints error and exits. If an undeclared variable
// is attempted to be used prints error and exits.
void checkSemantics(Node *node, SymbolTable &table)
{
	// Start OR1 
	// Base case: return if null node.
	if (!node)
		return;

	// Declaration through C node. Ensure first child is "#" and next child is tk2 token.
	if (node->label == "C" && node->children.size() == 2 
			&& node->children[0]->tk.tokenInstance == "#"
			&& node->children[1]->label == "tk2")
	{
		// Set tk2 token instance to variable var 
		const std::string &var = node->children[1]->tk.tokenInstance;
		// Insert var into symbol table if it is not already in the table.
		if(!table.insert(var))
		{
			// Print error and exit if var already in symbol table.
			fprintf(stderr, "Error: redefinition of variable %s\n", var.c_str());
			exit(1);
		}
	}
	// Declaration through A node. Ensure first child is "\"" and next child is tk2 token.
	else if (node->label == "A" && node->children.size() == 2
			&& node->children[0]->tk.tokenInstance == "\""
			&& node->children[1]->label == "tk2")
	{
		// Set tk2 token instance to variable var.
		const std::string &var = node->children[1]->tk.tokenInstance;
		// Insert var into symbol table if it is not already in the table.
		if(!table.insert(var))
		{
			// Print error and exit if var already in symbol table.
			fprintf(stderr, "Error: redifinition of variable %s\n", var.c_str());
			exit(1);
		}
	}
	// Variable reference when tk2 node is encountered.
	else if (node->label == "tk2")
	{
		// Set tk2 token instance to variable var.
		const std::string &var = node->tk.tokenInstance;
		// Ensure var was already declared by verifying symbol table.
		if(!table.verify(var))
		{
			// Print error and exit if var not in symbol table.
			fprintf(stderr, "Error: use of undefined variable %s\n", var.c_str());
			exit(1);
		}
	}

	// Recursively check static semantics for each child in parse tree
	for (Node *child : node->children)
		checkSemantics(child, table);  
	// End OR1
}
