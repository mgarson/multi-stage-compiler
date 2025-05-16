// CS4280 Project 2
// Author: Maija Garson
// Date: 04/13/2025
// Description: Header file for the testTree program. Declares function to recursively print the parse tree in preorder 
// traversal with 4-space indentation per level. For each node, its label and token instance (if available) are printed.

#ifndef _TESTTREE_H_
#define _TESTTREE_H_

#include "node.h"

void printPreorder(Node* root, int level);

#endif
