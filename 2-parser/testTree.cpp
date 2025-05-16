// CS4280 Project 2
// Author: Maija Garson
// Date: 04/13/2025
// Description: Program to implement a preorder traversal of the tree which will format and print the nodes of the tree. For each node,
// its label and token instance are printed (if available) on their corresponding lines in the parse tree. Each level of the tree is
// indented by 4 spaces for clarity.

#include <stdio.h>
#include <string>

#include "testTree.h"

// Start OR2

// Function to recursively  print parse tree in preorder traversal with 4-space indentation per level.
// For each node, its label and token instance (if available) are printed. 
void printPreorder(Node* root, int level)
{
	// Base case for recursion. Return current node if it is null.
	if (root == nullptr) return;

	// Print current node's label with 4 times indentation based on level.
	printf("%*s%s", level * 4, "", root->label.c_str());

	// If node includes token instance, print instance.
	if (!root->tk.tokenInstance.empty())
		printf(" %s", root->tk.tokenInstance.c_str());

	printf("\n");

	// Recursively print each child node, incrementing level to increase indentaion
	for (Node* child : root->children)
		printPreorder(child, level + 1);
}

// End OR2
