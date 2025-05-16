//CS4280 Proect 1
//Author: Maija Garson
//Date: 03/14/2025
//Description: Contains function to read input from file pointer  and call scanner function to iteratively return processed tokens that are generated from scanner function

#include <cstdio>
#include <fstream>

#include "testScanner.h"
#include "scanner.h"

// Function to parse input from file pointer and return generated tokens
void testScanner(FILE * file)
{

	// Print error and return if file cannot be opened
	if (!file)
	{
		fprintf(stderr, "Error: Unable to open file %s.\n", file);
		return;
	}

	int lineNum = 1; // Tracks current line number
	token tk; // Holds token to be returned by scanner

	do
	{
		// Call scanner function and print contents of token returned by scanner
		tk = scanner(file, lineNum);
		fprintf(stdout, "%s %s %d\n",tokenNames[tk.id], tk.tokenInstance.c_str(), tk.lineNum);

	} while (tk.id != EOFtk); // Stop when EOF token is reached

}
