// CS4280 Project 2
// Author: Maija Garson
// Date: 04/13/2025
// Description: Program that implements a recursive descent parser to build a parse tree based on the grammar 
// defined for this project. The given input is first tokenized, based on this project's lexical definitions,
// by calling the scanner program, and then the parser uses the grammar rules to build the parse tree from
// the tokens. The complete parse tree is returned.

#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "node.h"

// Start OR1

// Global variables to track input file, current token, and current line number
FILE *inputFile; // File pointer to inputfile
token tk; // Current token given by scanner
int lineNum = 1; // Current line number in input

// Function that checks if current token (global) matches the expected token (based on passed token id).
// If it matches, calls scanner function to consume token. Otherwise, prints error and exits. 
void match(tokenID expct)
{
	// If token id matches expected it, call scanner to consume it
	if (tk.id == expct)
		tk = scanner(inputFile, tk.lineNum);
	// If it does not match, print error message and exits
	else
	{
		fprintf(stderr, "PARSER ERROR: Expected token %s instead of %s at line %d\n",
				tokenNames[expct], tk.tokenInstance.c_str(), tk.lineNum);
		exit(1);
	}
}

// Initial parsing function. Initalizes input and line number and then calls scanner function to retrieve first
// token. It then builds the parse tree by calling parseS(), which is the start of the recursive descent parse
// that calls other parsing functions based on the grammar. 
// Once the parsing is compelte, the entire parse tree is returned.
Node* parser(FILE *file)
{
	// Set input file and initialize line counter.
	inputFile = file;
	lineNum = 1;

	// Retrieve first token from scanner.
	tk = scanner(inputFile, lineNum);

	// Recursively build parse tree based on grammar, starting at S.
	Node* root = parseS();

	// Ensure end of input by checking for EOF token. Otherwise, print error message and exit.
	if (tk.id != EOFtk)
	{
		fprintf(stderr, "PARSER ERROR: Extra tokens after parse, token %s at line %d\n",
				tk.tokenInstance.c_str(), tk.lineNum);
		exit(1);
	}

	// Return complete parse tree
	return root;
}

// Function to parse nonterminal S based on production rule S -> A ( B B )
// Creates S node and attaches A node from parseA(), checks for '(', then incrementally
// attaches two B nodes from parseB(), then checks for ')'.
// If any token does not match what is expected, error message is printed and program exits
// Returns complete S parse tree
Node* parseS()
{

	// Create an S node
	Node* sNode = new Node("S");

	// Parse A and attach A node as a child of S
	Node* aNode = parseA();
	sNode->children.push_back(aNode);

	// Expect and consume '(' token and add its node as a child of S. Otherwise, error.
	if (tk.tokenInstance == "(")
	{
		Node* paren1Node = new Node("tk1", tk);
		sNode->children.push_back(paren1Node);
		match(tk1);
	}
	else 
	{
		fprintf(stderr, "PARSER ERROR: Expected '(' instead of %s at line %d\n",
				tk.tokenInstance.c_str(), tk.lineNum);
		exit(1);
		return nullptr;
	}
	
	// Parse two B nodes and attach them as a child of S
	Node* bNode1 = parseB();
	sNode->children.push_back(bNode1);

	Node* bNode2 = parseB();
	sNode->children.push_back(bNode2);

	// Expect and consume ')' token and add its node as a child of S. Otherwise, error.
	if (tk.tokenInstance == ")")
	{
		Node* paren2Node = new Node("tk1", tk);
		sNode->children.push_back(paren2Node);
		match(tk1);
	}
	else
	{
		fprintf(stderr, "PARSER ERROR: Expected ')' instead of %s at line %d\n",
				tk.tokenInstance.c_str(), tk.lineNum);
		exit(1);
		return nullptr;
	}

	// Return parse tree for S
	return sNode;
}

// Function to parse nonterminal A based on production rule A -> " tk2 | empty
// Creates A node. If next tokens match the production rule attaches token nodes to A. Otherwise, attaches empty node.
// If any expected token is missing, prints error message and exits.
// Returns complete A parse tree.
Node* parseA()
{
	// Create an A node.
	Node* aNode = new Node("A");

	// Expect and consume '"' token and add its node as a child of A.
	if (tk.tokenInstance == "\"")
	{
		Node* tk1Node = new Node("tk1");
		tk1Node->tk = tk;
		aNode->children.push_back(tk1Node);
		match(tk1);

		// Expect and consume tk2 token and add its node as a child of A. Otherwise, error.
		if (tk.id == tk2)
		{
			Node* tk2Node = new Node("tk2", tk);
			aNode->children.push_back(tk2Node);
			match(tk2);
		}
		else
		{
			fprintf(stderr, "PARSER ERROR: Expected tk2 instead of %s at line %d in A\n",
					tk.tokenInstance.c_str(), tk.lineNum);
			exit(1);
			return nullptr;
		}
	}
	// Otherwise, empty token. Create empty token node and add as a child of A.
	else 
	{
		Node* emptyNode = new Node("empty");
		aNode->children.push_back(emptyNode);
	}

	// Return parse tree for A
	return aNode;
}

// Function to parse nonterminal B based on production rule B -> S | C | D | E | G
// Creates B node and then selects the appropriate production based on the current token.
// If token does not match any of the expected production rules, prints error message and exits.
// Returns complete B parse tree.
Node* parseB()
{
	// Create a B node.
	Node* bNode = new Node("B");

	// Select appropriate next parse based on current token.
	if (tk.tokenInstance == "\"" || tk.tokenInstance == "(") // Matches S rule.
	{
		// Parse S and attach as a child of B.
		Node* sNode = parseS();
		bNode->children.push_back(sNode);
	}
	else if (tk.id == tk1 && (tk.tokenInstance == "#" || tk.tokenInstance == "!")) // Matches C rule.
	{
		// Parse C and attach as a child of B.
		Node* cNode = parseC();
		bNode->children.push_back(cNode);
	}
	else if (tk.id == tk1 && tk.tokenInstance == "$") // Matches D rule.
	{
		// Parse D and attach as a child of B.
		Node* dNode = parseD();
		bNode->children.push_back(dNode);
	}
	else if (tk.id == tk1 && tk.tokenInstance == "'") // Matches E rule.
	{
		// Parse E and attach as a child of E.
		Node* eNode = parseE();
		bNode->children.push_back(eNode);
	}
	else if (tk.id == tk2) // Matches G rule.
	{
		// Parse G and attach as a child of G.
		Node* gNode = parseG();
		bNode->children.push_back(gNode);
	}
	else // Token does not match any rules for production.
	{
		// Print error message and exit.
		fprintf(stderr, "PARSER ERROR: Unexpected token '%s' at line %d in B.\n",
				tk.tokenInstance.c_str(), tk.lineNum);
		exit(1);
		return nullptr;
	}

	// Return parse tree for B.
	return bNode;
}

// Function to parse nonterminal C based on production rule C -> # tk2 | ! F 
// Creates C node and then selects the appropriate production based on the current token.
// If token does not match any of the expected production rules, prints error message and exits.
// Returns complete C parse tree.
Node* parseC()
{
	// Create C node.
	Node* cNode = new Node("C");

	// Select production based on current token.
	if (tk.tokenInstance == "#") // Matches '#'
	{
		// Consume tk1 token and attach its node as child of C.
		Node* tk1Node = new Node("tk1", tk);
		cNode->children.push_back(tk1Node);
		match(tk1);

		// Expect and consume tk2 token and attach its node as child of C.
		if (tk.id == tk2)
		{
			Node* tk2Node = new Node("tk2", tk);
			cNode->children.push_back(tk2Node);
			match(tk2);
		}
		// Otherwise, print error message and exit.
		else
		{
			fprintf(stderr, "PARSER ERROR: Expected t2 after '#', but got '%s' instead at line %d\n",
					tk.tokenInstance.c_str(), tk.lineNum);
			exit(1);
			return nullptr;
		}
	}
	else if (tk.tokenInstance == "!") // Matches '!'
	{
		// Consume tk1 token and attach its node as a child of C.
		Node* tk1Node = new Node("tk1", tk);
		cNode->children.push_back(tk1Node);
		match(tk1);

		// Parse F and attach as a child of C.
		Node* fNode = parseF();
		cNode->children.push_back(fNode);
	}
	else // Token does not match any rules for production.
	{
		// Print error messaage and exit.
		fprintf(stderr, "PARSER ERROR: Invalid transition with token '%s' at line %d\n",
				tk.tokenInstance.c_str(), tk.lineNum);
		exit(1);
		return nullptr;
	}

	// Return parse tree for C.
	return cNode;
}
// Function to parse nonterminal D based on production rule D -> $ F
// Creates D node. Consumes '$' token and attempts to parse F if token matches. Otherwise, prints error message and exits.
// Returns complete D parse tree.
Node* parseD()
{
	// Create D node.
	Node* dNode = new Node("D");
	
	// Consume tk1 token and attach its node as child of D.
	Node* tk1Node = new Node("tk1", tk);
	dNode->children.push_back(tk1Node);
	match(tk1);

	// If token matches F production, parse F and attach as child of D.
	if (tk.id == tk2 || tk.id == tk3 || tk.tokenInstance == "&")
	{
		Node* fNode = parseF();
		dNode->children.push_back(fNode);
	}
	// Otherwise, print error message and exit.
	else
	{
		fprintf(stderr, "PARSER ERROR: Invalid transition in D with token %s at line %d\n",
				tk.tokenInstance.c_str(), tk.lineNum);
		exit(1);
		return nullptr;
	}
	
	// Return parse tree for D.
	return dNode;
}

// Function to parse nonterminal E based on production rule E -> ' F F F B
// Creates E node. Consumes ''' token and attempts to incrementally parse F three times, then B if tokens match production.
// Otherwise, prints error message and exits.
// Returns complete E parse tree.
Node*  parseE()
{
	// Create E node.
	Node* eNode = new Node("E");

	// Consume tk1 token and attach its node as child of E.
	Node* tk1Node = new Node("tk1", tk);
	eNode->children.push_back(tk1Node);
	match(tk1);

	// If token matches F production, parse F three times and attach each node as child of E.
	if (tk.id == tk2 || tk.id == tk3 || tk.tokenInstance == "&")
	{
		Node* fNode1 = parseF();
		eNode->children.push_back(fNode1);

		Node* fNode2 = parseF();
		eNode->children.push_back(fNode2);

		Node* fNode3 = parseF();
		eNode->children.push_back(fNode3);
	}
	// Otherwise, print error message and exit.
	else
	{
		fprintf(stderr, "PARSER ERROR: Invalid transition in E with token '%s' at line %d\n",
				tk.tokenInstance.c_str(), tk.lineNum);
		exit(1);
		return nullptr;
	}

	// Parse B and attach as child of E.
	Node* bNode = parseB();
	eNode->children.push_back(bNode);

	// Return parse tree for E.
	return eNode;
}

// Function to parse nonterminal F based on production rule F -> tk2 | tk3 | & F F
// Creates F node and then selects the appropriate production based on the current token.
// If token does not match any of the expected production rules, prints error message and exits.
// Returns complete F parse tree.
Node* parseF()
{
	// Create F node.
	Node* fNode = new Node("F");

	// Select appropriate production based on token
	if (tk.id == tk2) // Matches tk2
	{
		// Consume tk2 token and add its node as a child of F.
		Node* tk2Node = new Node("tk2", tk);
		fNode->children.push_back(tk2Node);
		match(tk2);
	}
	else if (tk.id == tk3) // Matches tk3
	{
		// Consume tk3 token and add its node as a child of F.
		Node* tk3Node = new Node("tk3", tk);
		fNode->children.push_back(tk3Node);
		match(tk3);
	}
	else if (tk.tokenInstance == "&") // Matches '&'
	{
		// Consume tk1 token and add its node as a child of F.
		Node* ampNode = new Node("tk1", tk);
		fNode->children.push_back(ampNode);
		match(tk1);

		// Ensure token matches F rule.
		if (tk.id == tk2 || tk.id == tk3 || tk.tokenInstance == "&")
		{
			// Parse F and attach as a child of F.
			Node* fNode1 = parseF();
			fNode->children.push_back(fNode1);
		}
		// Otherwise, print error message and exit.
		else 
		{
			fprintf(stderr, "PARSER ERROR: Expected tk2 or tk3 after '&', but got '%s' instead at line %d\n",
					tk.tokenInstance.c_str(), tk.lineNum);
			exit(1);
			return nullptr;
		}

		// Ensure token matches F rule.
		if (tk.id == tk2 || tk.id == tk3 || tk.tokenInstance == "&")
		{
			// Parse F and attach as a child of F.
			Node* fNode2 = parseF();
			fNode->children.push_back(fNode2);
		}
		// Otherwise, print error message and exit.
		else
		{
			fprintf(stderr, "PARSER ERROR: Expected tk2 or tk3, but got '%s' instead at line %d\n",
					tk.tokenInstance.c_str(), tk.lineNum);
			exit(1);
			return nullptr;
		}
	}
	else // Token does not match any production.
	{
		// Print error message and exit.
		fprintf(stderr, "PARSER ERROR: Invalid transition in F with token '%s' at line %d\n", 
				tk.tokenInstance.c_str(), tk.lineNum);
		exit(1);
		return nullptr;
	}

	// Return parse tree for F.
	return fNode;
}

// Function to parse nonterminal G based on production rule G -> tk2 % F
// Creates a G node, then expects and consumes a tk2 token, followed by '%' token, then attempts to parse F.
// If any tokens do not match production, prints error message and exits.
// Returns the complete G parse tree.
Node* parseG()
{
	// Creates G node.
	Node* gNode = new Node("G");

	// Expect and consume tk2 token and add its node as a child of G.
	if (tk.id == tk2)
	{
		Node* tk2Node = new Node("tk2", tk);
		gNode->children.push_back(tk2Node);
		match(tk2);
	}
	// Otherwise, print error message and exit.
	else
	{
		fprintf(stderr, "PARSER ERROR: Invalid transition in G with token '%s' at line %d\n",
				tk.tokenInstance.c_str(), tk.lineNum);
		exit(1);
		return nullptr;
	}

	// Expect '%' and consume the tk1 token and add its node as a child of G.
	if (tk.tokenInstance == "%")
	{
		Node* percNode = new Node("tk1", tk);
		gNode->children.push_back(percNode);
		match(tk1);

		// Parse F and attach as child of G.
		Node* fNode = parseF();
		gNode->children.push_back(fNode);
	}
	// Otherwise, print error message and exit.
	else
	{
		fprintf(stderr, "PARSER ERROR: Expected '%%' in G rule, got '%s' instead at line %d\n",
			       tk.tokenInstance.c_str(), tk.lineNum);
		exit(1);	
		return nullptr;
	}
	
	// Return parse tree for G.
	return gNode;
}

// End OR1
