// CS4280 Project 4
// Author: Maija Garson
// Date: 5/11/2025
// Description: Program that implements the assembly code generation phase of the compiler. It traverses the parse
// tree produced by the parser and emits assembly instructions that  match the UMSL school interpreter to provided
// output. It generates unique temporary variables and labels as needed. It also converts t2 tokens to variables, 
// and t3 tokens to signed integers (the first character determines if it is pos or neg). After generating operations, 
// it writes a STOP instruction and then declares all user-declared variables and generated temporaries to 0 using 
// the symbol table provided. 

#include "codeGen.h"

#include <cstdio>
#include <string>
#include <cctype>
#include <vector>

using namespace std;

// Start OR1
//Global variables
FILE *outFile; // File pointer for output
int tempCount = 0; // Counter for temp variables
int labelCount = 0; // Counter for labels

// Function to return a new temporary variable as a string based on the current tempCount
string newTemp()
{
	// Append current temp count to 'temp'
	string t = "temp" + to_string(tempCount);
	// Increment tempCount for next temp variable
	tempCount++;
	return t;
}

// Function to return a new label as a string based on the current labelCount
string newLabel()
{
	// Append current label count to 'label'
	string l = "label" + to_string(labelCount);
	// Increment labelCount for next label
	labelCount++;
	return l;
}

// Function to convert token type tk3 to a signed integer. 
// Upper case letters return int as positive, lowercase letters return int as negative
int getSignedInt(string &t3)
{
	int intVal;
	int caseVal;
	int ascii;

	ascii = static_cast<int>(t3[0]);

	// Ensure first character starts with letter A-Z or a-z
	if (ascii < 65 || ascii > 122 || (ascii > 90 && ascii < 97)) 
	{
		fprintf(stderr, "ERROR: Expected a letter for the first character of a t3 token.\n");
		exit(1);
	}
	
	// If uppercase, caseVal is positive
	if (ascii >= 65 && ascii <= 90)
		caseVal = 1;
	//If lowercase, caseVal is negative
	if (ascii >= 97 && ascii <= 122)
		caseVal = -1;

	// Zero out first character in token
	t3[0] = '0';

	// Return remaining characters as int, multiplying by caseVal
	intVal = atoi(t3.substr(1).c_str());
	intVal = intVal * caseVal;
	return intVal;
}

// Function to convert token type t2 to a string variable, converting first char ('+') to 'p'
string getVar(const string &tok)
{
	// Return token's substring following first char, while prefixing with 'p'
	return "p" + tok.substr(1);
}

// Function to recursively traverse parse tree and generate assembly while doing so
// n->label represents the Node's label, and corresponds to nonterminals in grammar (S, A, B, C, D, E, F, G)
void generateCode(Node *n)
{
	// S: program entry point
	// Production: S -> A (B B)
	if (n->label == "S")
	{
		// If label not empty start with generating code for first element
		if (n->children[0]-> label != "empty")
			generateCode(n->children[0]);
		// Otherwise, skip to generating code for  ext two chidren (1 would be " for allocation)
		generateCode(n->children[2]);
		generateCode(n->children[3]);
	}
	// A: initialize newly declared variable (tk2) to 0
	// Production: A -> " t2 | empty
	else if (n->label == "A")
	{
		// If not empty, initialize var representing tk2 to 0
		if (n->children[0]->label != "empty")
		{
			string var = getVar(n->children[1]->tk.tokenInstance);
			fprintf(outFile, "LOAD 0\n");
			fprintf(outFile, "STORE %s\n", var.c_str());
		}
	}
	// B: delegate to its single child
	// Production: B -> S | C | D | E | G
	else if (n->label == "B")
		// Generate code for its child
		generateCode(n->children[0]);
	// C: Read user input into variable or negate F
	// Production: C -> # t2 | ! F 
	else if (n->label == "C")
	{ 
		std::string &op = n->children[0]->tk.tokenInstance;
		if (op == "#") // Read
		{
			// Read input into variable
			string var = getVar(n->children[1]->tk.tokenInstance);
			fprintf(outFile, "READ %s\n", var.c_str());
		}
		else if (op == "!") // Negate
		{
			// Generate code for F node and negate its value
			Node *fNode = n->children[1];
			generateCode(fNode);
			fprintf(outFile, "MULT -1\n");
			// If F's child is tk2, store negation back into that variable
			if (fNode->children.size() == 1 && fNode->children[0]->label == "tk2")
			{
				string var = getVar(fNode->children[0]->tk.tokenInstance);
				fprintf(outFile, "STORE %s\n", var.c_str());
			}
			// Otherwise, tk3 (literal), create new temp to store value
			else
			{
				string tmp = newTemp();
				fprintf(outFile, "STORE %s\n", tmp.c_str());
			}	
		}
	}
	// D: write F's value
	// Production: D -> $ F 
	else if (n->label == "D")
	{
		// Generate code for F and write its value
		generateCode(n->children[1]);
		string tmp = newTemp();
		fprintf(outFile, "STORE %s\n", tmp.c_str());
		fprintf(outFile , "WRITE %s\n", tmp.c_str());
	}
	// E: loop - set up initial value, bound, and count; repeat while initial >= bound for count iterations
	// Production: E -> ' F F F B
	else if (n->label == "E")
	{
		// Allocate labels for loop test and end
		string lblEnd = newLabel(); // Jump here to exit loop
		string lblTest = newLabel(); // Jump here to test each iteration
		// Allocate temps for initial value, bound, and iteration count
		string tmp1 = newTemp(); // Initial loop value
		string tmp2 = newTemp(); // Loop bound
		string cntTmp = newTemp(); // Iteration count

		// Generate code for first F (init value) and store its value
		generateCode(n->children[1]);
		fprintf(outFile, "STORE %s\n", tmp1.c_str());

		// Generate code for second F (bound) and store its value
		generateCode(n->children[2]);
		fprintf(outFile, "STORE %s\n", tmp2.c_str());

		// Skip loop if initial - bound < 0
		fprintf(outFile, "LOAD %s\n", tmp1.c_str());
		fprintf(outFile, "SUB %s\n", tmp2.c_str());
		fprintf(outFile, "BRZNEG %s\n", lblEnd.c_str());

		// Generate code for third F (count) and store its value
		generateCode(n->children[3]);
		fprintf(outFile, "STORE %s\n", cntTmp.c_str());

		// Skip if count < 0
		fprintf(outFile, "LOAD %s\n", cntTmp.c_str());
		fprintf(outFile, "BRZNEG %s\n", lblEnd.c_str());

		// Loop start label
		fprintf(outFile, "%s: NOOP\n", lblTest.c_str());

		// Generate code for B (loop body)
		generateCode(n->children[4]);

		// Decrement counter
		fprintf(outFile, "LOAD %s\n", cntTmp.c_str());
		fprintf(outFile, "SUB 1\n");
		fprintf(outFile, "STORE %s\n", cntTmp.c_str());

		// Repeat if counter > 0
		fprintf(outFile, "LOAD %s\n", cntTmp.c_str());
		fprintf(outFile, "BRPOS %s\n", lblTest.c_str());

		// Exit loop
		fprintf(outFile, "%s: NOOP\n", lblEnd.c_str());

	}
	// F: load a variable (t2), or load a signed int (t3), or evaluate addition
	// Production: F -> t2 | t3 | & F F
	else if (n->label == "F")
	{
		// Load a variable 
		if (n->children.size() == 1 && n->children[0]->label == "tk2")
		{
			string var = getVar(n->children[0]->tk.tokenInstance);
			fprintf(outFile, "LOAD %s\n", var.c_str());
		}
		// Load a signed integer
		else if (n->children.size() == 1 && n->children[0]->label == "tk3")
		{
			int val = getSignedInt(n->children[0]->tk.tokenInstance);
			fprintf(outFile, "LOAD %d\n", val);
		}
		// Otherwise, compute F + F
		else
		{
			generateCode(n->children[1]);
			string tmp = newTemp();
			fprintf(outFile, "STORE %s\n", tmp.c_str());
			generateCode(n->children[2]);
			fprintf(outFile, "ADD %s\n", tmp.c_str());
		}
	}
	// G: assign variable (t2) to F's computed value
	// Production: G -> t2 % F
	else if (n->label == "G")
	{
		// Compute F's value
		generateCode(n->children[2]);

		// Store result into LHS variable (t2)
		string var = getVar(n->children[0]->tk.tokenInstance);
		fprintf(outFile, "STORE %s\n", var.c_str());
	}

	
}

// Function to generate assembly code. Initializes counters to 0,
// traverses the parse tree to generate corresponding assembly,
// then writes STOP and initializes all variables and temps to 0 at end
void codeGen(Node *root, FILE *out, const SymbolTable &symTab)
{
	// Set output file
	outFile = out; 
	// Reset counters
	tempCount = 0; 
	labelCount = 0; 

	// Traverse parse tree and generate assembly
	generateCode(root);

	// Generate assembly terminator
	fprintf(outFile, "STOP\n");

	// Initialize all declared variables (t2) to 0
	for (const std::string &v : symTab.getAllIdentifiers())
	{
		string var = getVar(v);
		fprintf(outFile, "%s 0\n", var.c_str());
	}

	// Initialize all temp variables to 0
	for (int i = 0; i < tempCount; i++)
		fprintf(outFile, "temp%d 0\n", i);

}
// End OR1
