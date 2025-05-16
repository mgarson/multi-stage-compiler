//CS4280 Project 3
//Author: Maija Garson
//Date: 04/21/2025
//Description: Header file that declares the Node class, which is used to represent nodes in the parse tree. Each Node includes a label
//(indicates nonterminal or terminal node), the associated token for terminal nodes (nonterminal nodes are given nonterm token id),
//and a vector containing pointers to any child nodes.

#ifndef _NODE_H_
#define _NODE_H_

#include <vector>

#include "token.h"

class Node {
	public: 
		std::string label; // Label of node, either nonterminal rule or terminal token 
		token tk; // Token for terminal nodes
		std::vector<Node*> children; // Children nodes

		// Constructor for nonterminal node
		// Initializes node with passed label and assigns values to token that indicates it is nonterminal 
		// (nonterminal token id, blank token instance, and invalid line number). Does not require token as parameter.
		Node(const std::string& lbl) : label(lbl) {
			tk.id = nonterm;
			tk.tokenInstance = "";
			tk.lineNum = -1;
		}

		// Constructor for terminal node
		// Initializes node with passed label and sets token to token that is passed as parameter. Token required as parameter.
		Node(const std::string& lbl, const token &t)
			: label(lbl), tk(t) {}

};

#endif 
