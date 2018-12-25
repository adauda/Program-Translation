#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testScanner.h"
#include "token.h"
#include "scanner.h"

char *tokenNames[36] = {
			"EOFTk", 
			"Error", 
			"IDTk", 
			"NumTk", 
			"EqualSignTk",
			"LESSTHANTk", 
			"GREATERTHANTk", 
			"ColontTk",
			"PlusSignTk", 
			"MinusSignTk", 
			"AsteriskTk", 
			"SlashTk", 
			"AmpersandTk",
			"PercentSignTk", 
			"PeriodTk", 
			"LeftParenthesisTk", 
			"RightParenthesisTk", 
			"CommaTk",
			"LeftBraceTk", 
			"RightBraceTk", 
			"SemicolonTk", 
			"LeftBracketTk", 
			"RightBracketTk",

			"KeywordTk",   	//for begin
			"KeywordTk",    //for end
			"KeywordTk", 	//for iter
			"KeywordTk",	//for var
			"KeywordTk",	//for void
			"KeywordTk", 	//for int
			"keywordTk", 	//for return
			"KeywordTk", 	//for read
			"KeywordTk", 	//for print
			"KeywordTk",	//for program
			"KeywordTk",	//for let
			"KeywordTk",	//for cond
			"KeywordTk",	//for then
};

void testScanner (FILE *file){
	token_t tmp;
	int lineNo = 1;

	do{
		tmp = scan(file, &lineNo);

		if(strcmp(tokenNames[tmp.tkIdentifier], "Error") == 0){

			printf("Scanner Error: %s Not Allowed At Line: %i.\n", tmp.tkString, tmp.lineNo);
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("%s %s %i\n", tokenNames[tmp.tkIdentifier], tmp.tkString, tmp.lineNo);
		}

	} while(!feof(file));
}
