//CS4280 Project 1
//Author: Maija Garson
//Date: 03/14/2025
//Description: Contains functions and structure to read input from a file, process characters while determining their types, and use an finite state automation table to classify characters and generate different tokens based on their found states.

// Start 01
#include "scanner.h"
#include <fstream>
#include <cctype>
#include <string>

//Finite State Automation table driver to classify tokens
int fsaTable[6][6] = {
	{0, 5, 1, -1, 1004, 3}, 
	{-2, -2, -2, 2, -2, -2},
	{1003, 1003, 1003, 2, 1003, 1003},
	{-3, -3, -3, 4, -3, -3},
	{1002, 1002, 1002, 4, 1002, 1002},
	{1001, 1001, 1001, 1001, 1001, 1001}
};

// Function to determine different types of character. These values
// will help to determine different states in FSA table
int getCharType(char c)
{
	if (isdigit(c)) return 3; 
	if (isalpha(c)) return 2; 
	if (c == '!' || c == '\"' || c == '#' || c == '$'|| c == '&' ||
		c == '%' || c == '\'' || c == '(' || c == ')') return 1;
	if (c == '\n' || c == ' ' || c == '\t') return 4;
	if (c == '+') return 5;
	if (c == '*') return 6;
	return -1;
}

// Function to process and create token. Uses parameters to determine token
// values and returns token with its values based on these parameters
token processToken(std::string tokenInstance, int tokenType, int lineNum)
{
	token newToken;
	newToken.tokenInstance = tokenInstance;
	newToken.lineNum = lineNum;

	// Assign token ID based on given token type
	switch (tokenType)
	{
		case 1: newToken.id = tk1; break;
		case 2: newToken.id = tk2; break;
		case 3: newToken.id = tk3; break;
		case 4: newToken.id = EOFtk; break;
		default: newToken.id = EOFtk; break;
	}


	return newToken; // Return processed token with passed instance, type, and line number
}

// Function to read and tokenize input 
token scanner(FILE *file, int &lineNum)
{
	int state = 0; // Represents state in FSA table, starting state will always be 0
	std::string currentToken; // String to hold current token being formed
	char c; // Current character being processed
	int charType; // Type of current character

	//Loop through file character by character, stopping when there is no input left 
	while ((c = fgetc(file)) != EOF)
	{
		// Increment line number if new line character is found
		if (c == '\n')
			lineNum++;

		// Use helper function to find character type of c
		charType = getCharType(c);

		// Skip if char type is 4, as this is ws, tabs and new line. Ensures ws and blank lines do 
		// not affect tokenization
		if (charType == 4)
			continue;
		
		// Print error message and skip if char type is -1, as this is an invalid character for our input
		if (charType == -1)
		{
			fprintf(stderr, "SCANNER ERROR: %c is not a valid character for input.\n", c);
			continue;
		}

		// Determine if char type represents the start of a comment, *
		if (charType == 6)
		{
			bool validComment = false;
			while ((c=fgetc(file)) != EOF)
			{
				// Process comment, consuming characters, until * is found, meaning end of comment 
				{
				
					if(getCharType(c) == 6)
					{
						validComment = true; // Only set to true when end of comment is found
						break;
					}
				}
				// If no more input, there was an issue processing the file at this point. Print message and exit loop
			//	else
				{
			//		fprintf(stderr, "ERROR PROCESSING FILE\n");
			//		break;
				}
			}

			// Check bool to see if comment properly terminated. If not, print error message and break out of entire loop. At this point, we would be at EOF
			if(!validComment)
			{
				fprintf(stderr, "SCANNER ERROR: Comment not properly terminated.\n");
				break;
			}

			// Skip rest of loop at this point to skip processing comments as tokens
			continue;
					
		}
		// Update state in fsa table based on current state and char type
		state = fsaTable[state][charType];
		// Add char to current token being built
		currentToken.push_back(c);
		
		// Lookahead to determine if token has reached end state
		char nextCh = fgetc(file);
		if (nextCh != EOF)
			ungetc(nextCh, file);
		int  nextType = getCharType(nextCh);

		// Ensure lookahead is not EOF or ws/tab/new line
		if (nextCh != EOF || nextType != 4)
		{
			
			// Determine the next state in the fsa table based on on lookahead
			int nextType = getCharType(nextCh);
			int nState = fsaTable[state][nextType];

			// If next state is negative, means table reached an error state and current token being created is not valid.
			// Print error message, reset state and current token, and skip rest of loop
			if (nState < 0)
			{
				fprintf(stderr, "SCANNER ERROR: Invalid char %c to create current token.\n", c);
				state = 0;
				currentToken = "";
				continue;
			}

		
			// Deteremine if next state is end state of any of our 3 types of tokens
			// If it is, process the token using helper function, reset state and current token, and return processed token
			if (nState == 1001 ||  nState == 1002 || nState == 1003)
			{	
				token result = processToken(currentToken, nState-1000, lineNum);
				currentToken = "";
				state = 0;
				return result;
			}
		}

	}

	// If we exit loop, means end of input so return EOF token
	return processToken(currentToken, 4, lineNum);
}
// End O1




