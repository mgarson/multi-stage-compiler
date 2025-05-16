//CS4280 Project 2
//Author: Maija Garson
//Date: 04/13/2025
//Description: Main program to implement scanning and parsing of different types of input. First processes command line arguments to 
//determine if input should be read from given file or standard input. It then uses the parser function to create a tree. Within the
//parser function, the scanner is called to classify the input into different types of tokens based on the given lexical definitions.
//These tokens are then passed to the parser, which uses the project grammar to create the parse tree. The tree is then printed.

#include <stdio.h>

#include "parser.h"
#include "testTree.h"

int main(int argc, char *argv[])
{
	FILE* file = nullptr;
        // If only 1 argument on command line, read from standard input (keyboard)
	if (argc == 1)
	{
		file = stdin;
		setvbuf(stdin, NULL, _IONBF, BUFSIZ);
	}

        // If 2 arguments given, set file pointer to given file
        else if (argc == 2)
	{
		file = fopen(argv[1], "r");

		// If file is null, invalid file. Print error message and return
		if (file == nullptr)
		{
			fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
			return 1;
		}
	}
        // Else not correct input on command line, print error message, usage, and return
        else
	{
		fprintf(stderr, "No file given.\nUsage: ./P2 <filename>\n");
		return 1;
	}
	
	// Call parser function to recursively create parse tree from given input
	Node* tree = parser(file);
	// Print parse tree
	printPreorder(tree, 0);

	// If file initially given, close file
	if (file != stdin)
		fclose(file);

	return 0;
}


