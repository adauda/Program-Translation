#ifndef SCANNER_H
#define SCANNER_H
#include "token.h"

#define SPECIALCHARS 20
#define TOKENS 25
#define KEYWORDS 12

#define MAXIDLEN 8
#define LOOKAHEAD 1

extern token_t returnTk;



typedef struct mapChar{

	int input;	//character
	int column;	//in dfa state table

}char_m;



typedef struct mapState{

	int state;
	tok_id tkEnum;

}state_m;



//Scanner function
token_t scan(FILE *file, int *lineNo);

//Token and Keyword mapping function
token_t lookup(int state, char *searchString);

//Character mapping function
int matchValid(int currentChar);

#endif
