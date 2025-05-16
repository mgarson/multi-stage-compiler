// CS4280 Project 1
// Author: Maija Garson
// Description: Main function that validates command-line arguments and calls function to iteratively process tokens

#include <cstdio>
#include "testScanner.h"

int main(int argc, char *argv[])
{
	FILE* file = nullptr;
	// If only 1 argument on command line, read from keyboard input
	if (argc == 1)
		file = stdin;

	// If 2 arguments given, set file pointer to given file
	else if (argc == 2)
		file = fopen(argv[1], "r");
	// Else not correct input on command line, print error message, usage, and return
	else
	{
		fprintf(stderr, "No file given.\nUsage: ./P1 <filename>\n");
		return 1;
	}
	// Call testScanner function to run with file pointer passed. Will either parse input file given or keyboard input
	testScanner(file);

	return 0;
}
