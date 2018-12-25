#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "scanner.h"
#include "token.h"


int StateTable[22][24] = {
				/* 0 	1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21  22   23 */
				/* L    D    =	  <    >    :    +    -    *    /    &    %    .    (    )    ,    {    }    ;    [    ]    !   WS  EOF */

				{  1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  -2,   0,  -1}, // 0
 				{  1, 1, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000}, // 1
 				{1001,   2, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001}, // 2
 				{1002, 1002, 1003, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002}, // 3
/*
 				{1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005,}, // 4
 				{1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006}, // 5
*/
 				{1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003}, // 4
                                {1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004}, // 5

 				{1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007}, // 6
 				{1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008}, // 7
 				{1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009}, // 8
 				{1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010}, // 9
 				{1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 11011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011}, // 10
 				{1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012}, // 11
 				{1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013}, // 12
 				{1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014}, // 13
 				{1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015}, // 14
 				{1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016}, // 15
 				{1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017}, // 16
 				{1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018}, // 17
 				{1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019}, // 18
 				{1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020}, // 19
 				{1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021}, // 20
 				{1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022}  // 21
			};


// char maps for =, <, >, :, +, -, *, /, &, %, ., (, ), ,, {, }, ;, [, ], !
char_m specialChars[] = {
			
			//{input, column}
			{61, 2}, 
			{60, 3}, 
			{62, 4}, 
			{58, 5}, 
			{43, 6}, 
			{45, 7}, 
			{42, 8}, 
			{47, 9}, 
			{38, 10}, 
			{37, 11}, 
			{46, 12}, 
			{40, 13}, 
			{41, 14}, 
			{44, 15}, 
			{123, 16},
			{125, 17}, 
			{59, 18}, 
			{91, 19}, 
			{93, 20}, 
			{33, 21}	
};


//to map final states to tokens
state_m finalStateMap[] = {

  			{-1, EOF_tk}, 
			{-2, ERROR_tk},
  			{1000, ID_tk}, 
			{1001, NUM_tk},
  			{1002, EQUAL_tk}, 
			{1003, LESSTHAN_tk}, 
			{1004, GREATERTHAN_tk},
  			{1007, COLON_tk},
  			{1008, PLUS_tk}, 
			{1009, MINUS_tk}, 
			{1010, ASTERISK_tk}, 
			{1011, SLASH_tk}, 
			{1012, AMP_tk} , 
			{1013, PCT_tk},
  			{1014, PERIOD_tk},
  			{1015, LPAREN_tk}, 
			{1016, RPAREN_tk}, 
			{1017, COMMA_tk}, 
			{1018, LBRACE_tk}, 
			{1019, RBRACE_tk},
  			{1020, SCOLON_tk}, 
			{1021, LBRACKET_tk}, 
			{1022, RBRACKET_tk}
};


//to map keywords
token_t keywordLookupTable[] = {

			{BEGIN_tk, "begin", 0}, 
			{END_tk, "end", 0}, 
			{ITER_tk, "iter", 0},
			{VAR_tk, "var", 0},
			{VOID_tk, "void", 0}, 
			{INT_tk, "int", 0},
			{RETURN_tk, "return", 0}, 
			{READ_tk, "read", 0}, 
			{PRINT_tk, "print", 0},
			{PROGRAM_tk, "program", 0},
			{LET_tk, "let", 0},
			{COND_tk, "cond", 0},
			{THEN_tk, "then", 0}
};


// Mapping Functions
int matchValid(int currentChar) {

			//match char
			if (isalpha(currentChar)) {
				return 0;
			}

			//match digit
			else if (isdigit(currentChar)) {
				return 1;
			}

			//match space
			else if (isspace(currentChar)) {
				return 22;
			}

			//match eof
			else if (currentChar == EOF) {
				return 23;
			}
			
			 else if(currentChar != EOF || !isspace(currentChar) || !isdigit(currentChar) || !isalpha(currentChar)){
				int l;
				//look for match in specialchars
				for (l = 0; l < SPECIALCHARS; l++) 
				{
					if(specialChars[l].input == currentChar)
						return specialChars[l].column;
				}
			}
	return -2;
}


//function to map currentword to tokens
token_t lookup(int state, char *str) {

	token_t returnTk;
	int i;

	for (i = 0; i < TOKENS; i++) 
	{
		if (finalStateMap[i].state == state) 
		{
			if(isupper(str[0])){
				printf("Scanner error: cannot start with an Uppercase\n");
				exit(-1);
			}
				

			returnTk.tkIdentifier = finalStateMap[i].tkEnum;
			returnTk.tkString = str;
			returnTk.lineNo = 999; 
		}
	}

	if (state == 1000) {
		for (i = 0; i < KEYWORDS; i++) 
		{
			if (strcmp(keywordLookupTable[i].tkString, str) == 0) 
			{
				returnTk.tkIdentifier = keywordLookupTable[i].tkIdentifier;
				returnTk.tkString = keywordLookupTable[i].tkString;
				returnTk.lineNo = 999;
			}
		}
	}

return returnTk;
}


//Scanner Function
token_t scan(FILE *file, int *lineNo) {

	token_t returnTk;
	int currentState = 0;
	int nextState = 0;

	returnTk.tkIdentifier = 999;
	returnTk.tkString = NULL;
	returnTk.lineNo = 999;

	int currentChar;
	int stateColumn;				//where currentChar is represented in state table.
	char *currentWord = malloc(9);			//for multiple chars tokens.
	char *currentCharStr = malloc(2);		//for appending char to currentWord

	//while not final and not error state, continue to get chars and create token
	while (currentState < 1000 && currentState > -1) 
	{
		currentChar = fgetc(file);

		//skip comments until space is gotten
		if (currentChar == '#') {
			while (!isspace(currentChar)) 
			{
				currentChar = fgetc(file);
			}
		}

		
		stateColumn = matchValid(currentChar);
  		sprintf(currentCharStr, "%c", currentChar);
		nextState = StateTable[currentState][stateColumn];

		//for invalid char at the start of a token
		if (stateColumn == -2) {

			returnTk.tkIdentifier = ERROR_tk;
			returnTk.lineNo = *lineNo;
			strcpy(currentWord, currentCharStr);
			printf("\nScanner Error: Invalid Character: %c.\n", currentChar);

			returnTk.tkString = currentWord;
			return returnTk;
		}


		//if nextstate is final, eof or an error
		if (nextState > 999 || nextState == -1 || nextState == -2) 
		{
			
			//for eof
			if (nextState == -1) {
				returnTk.tkIdentifier = EOF_tk;
				returnTk.lineNo = *lineNo;
				returnTk.tkString = "EOF";
				return returnTk;
			}
			
			//for special case tokens, build the word, look up token and overwrite default lineNo
			else if (nextState == 1003 || nextState == 1004 || nextState == 1005 || nextState == 1006) {
  				sprintf(currentCharStr, "%c", currentChar);
				strcat(currentWord, currentCharStr);

				returnTk = lookup(nextState, currentWord);
				returnTk.lineNo = *lineNo;
				return returnTk;
			}
			
			//for final states
			else if (nextState > 999) {
				returnTk = lookup(nextState, currentWord);
				returnTk.lineNo = *lineNo;
				ungetc(currentChar, file);
				return returnTk;
			}

			//for error
			else {
				ungetc(currentChar, file);
				int tmp = fgetc(file);
  				sprintf(currentCharStr, "%c", tmp);
				strcat(currentWord, currentCharStr);
				printf("\nScanner Error: Line: %i. Invalid Character: %c. %s Not A Valid Language Construct.\n", *lineNo, currentChar, currentWord);
				returnTk.tkIdentifier = ERROR_tk;
				returnTk.lineNo = *lineNo;
				returnTk.tkString = currentWord;
				return returnTk;
			}
		}


		//if not final state, continue to build token.
		else{
			if(!isspace(currentChar))
				strcat(currentWord, currentCharStr);
			
			//check input length, not greater than 8.
			if (strlen(currentWord) > 8){

				printf("Scanner Error: Input Length Exception. Length of input should be less than 9\n");

				returnTk.tkIdentifier = ERROR_tk;
				returnTk.lineNo = *lineNo;
				returnTk.tkString = "Length";
				return returnTk;
			}

			//for new line
			if (currentChar == '\n')
				(*lineNo)++;

			//transist state
			currentState = nextState;
		}
		
	}
	
return returnTk;

}
