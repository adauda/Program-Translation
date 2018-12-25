
#ifndef TOKEN_H
#define TOKEN_H

typedef enum tokenId { 	
			EOF_tk,
			ERROR_tk, 
			ID_tk, 
			NUM_tk, 
			EQUAL_tk, 
			LESSTHAN_tk, 
			GREATERTHAN_tk, 
			COLON_tk, 
			PLUS_tk, 
			MINUS_tk, 
			ASTERISK_tk, 
			SLASH_tk, 
			AMP_tk, 
			PCT_tk, 
			PERIOD_tk, 
			LPAREN_tk, 
			RPAREN_tk, 
			COMMA_tk, 
			LBRACE_tk, 
			RBRACE_tk, 
			SCOLON_tk, 
			LBRACKET_tk, 
			RBRACKET_tk, 
			BEGIN_tk, 
			END_tk, 
			ITER_tk, 
			VAR_tk, 
			VOID_tk, 
			INT_tk, 
			RETURN_tk, 
			READ_tk, 
			PRINT_tk, 
			PROGRAM_tk, 
			LET_tk, 
			COND_tk,
			THEN_tk
} tok_id;


typedef struct token{
	tok_id tkIdentifier; 
	char *tkString; 
	int lineNo;
} token_t;
						
#endif
