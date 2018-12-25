#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "scanner.h"
#include "token.h"

FILE *inputfp = NULL;

/* declare token_t and lineNo to be used in scanner calls within CFG functions */
token_t tk;
int lineNo = 1;

/* Array of token literals mapped to token enums for error reprting purposes */
static const char *tokenNames[] = {
		[EOF_tk] = "EOF_tk", [ERROR_tk] = "ERROR_tk", [IDENT_tk] = "IDENT_tk", [NUM_tk] = "NUM_tk",

		[EQUAL_tk] = "EQUAL_tk", [LESSTHAN_tk] = "LESSTHAN_tk", [GREATERTHAN_tk] = "GREATERTHAN_tk", [COLON_tk] = "COLON_tk", [PLUS_tk] = "PLUS_tk", 
		[MINUS_tk] = "MINUS_tk", [ASTERISK_tk] = "ASTERISK_tk", [SLASH_tk] = "SLASH_tk", [AMP_tk] = "AMP_tk", [PCT_tk] = "PCT_tk",
		[PERIOD_tk] = "PERIOD_tk",

		[LPAREN_tk] = "LPAREN_tk", [RPAREN_tk] = "RPAREN_tk", [COMMA_tk] = "COMMA_tk", [LBRACE_tk] = "LBRACE_tk", [RBRACE_tk] = "RBRACE_tk",
                [SCOLON_tk] = "SCOLON_tk", [LBRACKET_tk] = "LBRACKET_tk", [RBRACKET_tk] = "RBRACKET_tk",

		[BEGIN_tk] = "BEGIN_tk", [END_tk] = "END_tk", [ITER_tk] = "ITER_tk", [VAR_tk] = "VAR_tk", [VOID_tk] = "VOID_tk", 
		//[INT_tk] = "INT_tk",
                [RETURN_tk] = "RETURN_tk", [READ_tk] = "READ_tk", [PRINT_tk] = "PRINT_tk", [PROGRAM_tk] = "PROGRAM_tk", [LET_tk] = "LET_tk",
		[COND_tk] = "COND_tk", [THEN_tk] = "THEN_tk"
};

//parser call
node_t *parser(){

        /* Root node to be returned as tree */
        node_t *node;

        /* request token from scanner (all sub-functions expect fresh token) */
        tk = scan(inputfp, &lineNo);

        /* call first CFG function and assign its return to root node */
        node = program();

        /* EOF signals successful parse */
        if(tk.tkIdentifier != EOF_tk){
        /* Anything returned other than EOF means parse failed */
                printf("Parser Error: Parse Failed. Expected EOF_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                exit(EXIT_FAILURE);
        }
        return node;
}


/* program() looks for program keyword to signal start of valid program */
node_t *program(){
        /* Tree depth is 0 here */
        int depth = 0;
        /* Create tree node for this call */
        node_t *node = createNode("<program>", depth);
        /* Look for expected program token */
        if(tk.tkIdentifier == VOID_tk){                      /* Predict <program> -> void <vars> <block> */
                /* Get fresh token (consume VOID_tk) for next function */
                tk = scan(inputfp, &lineNo);
                /* call child functions and populate children with returned nodes */
                node->child1 = vars(depth);
                node->child2 = block(depth);
                return node;
        }
        /* If token other than expected encountered, raise error with line number and exit */
        else{
                printf("Parser Error: Invalid Token, Expected VOID_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                exit(EXIT_FAILURE);
        }
}


node_t *vars(int depth){
        depth++;
        node_t *node = createNode("<vars>", depth);
        if(tk.tkIdentifier == LET_tk)
	{          /* Predict <vars> -> let Identifier = Integer <vars> */
                tk = scan(inputfp, &lineNo);
                if(tk.tkIdentifier == IDENT_tk){
                        /* Store non-syntactic token */
                        node->token = tk;
                        tk = scan(inputfp, &lineNo);
                        if(tk.tkIdentifier == EQUAL_tk){
				tk = scan(inputfp, &lineNo);
				if(tk.tkIdentifier == NUM_tk){
					node->token2 = tk;
					tk = scan(inputfp, &lineNo);
					node->child1 = vars(depth);
					return node;
				}
				else
				{
					printf("Parser Error: Invalid Token, Expected INT_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
					exit(EXIT_FAILURE);
				}
			
			}
			else
			{
				printf("Parser Error: Invalid Token, Expected EQUAL_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
				exit(EXIT_FAILURE);
			}
			return node;
                }	
                else{
                        printf("Parser Error: Invalid Token, Expected IDENT_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
			exit(EXIT_FAILURE);
                }
        }
	else{
		return NULL;                            /* Predict <vars> -> empty */
	}
}

node_t *block(int depth){
        depth++;
        node_t *node = createNode("<block>", depth);

        if(tk.tkIdentifier == BEGIN_tk){        /* Predict <block> -> begin <vars> <stats> end */
                tk = scan(inputfp, &lineNo);

                node->child1 = vars(depth);
                node->child2 = stats(depth);

                if (tk.tkIdentifier == END_tk){
                        tk = scan(inputfp, &lineNo);
                        return node;
                }
                else{
                        printf("Parser Error: Invalid Token, Expected END_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                        exit(EXIT_FAILURE);
                }
        }
        else{
                printf("Parser Error: Invalid Token, Expected BEGIN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                exit(EXIT_FAILURE);
        }
}


node_t *expr(int depth){
        depth++;
        node_t *node = createNode("<expr>", depth);

        node->child1 = a(depth);                                /* Predict <expr> -> <A> */

        if(tk.tkIdentifier == ASTERISK_tk){                     /* Predict <expr> -> <A> * <expr> */
                node->token = tk;
                tk = scan(inputfp, &lineNo);
                node->child2 = expr(depth);
                return node;
        }
	else if(tk.tkIdentifier == SLASH_tk){                     /* Predict <expr> -> <A> / <expr> */
                node->token = tk;
                tk = scan(inputfp, &lineNo);
                node->child2 = expr(depth);
                return node;
        }
        else{
                return node;
        }
}


node_t *a(int depth){
        depth++;
        node_t *node = createNode("<A>", depth);

        node->child1 = m(depth);                        /* Predict <A> -> <M> */
        if(tk.tkIdentifier == PLUS_tk){                 /* Predict <A> -> <M> + <A> */
                node->token = tk;
                tk = scan(inputfp, &lineNo);
                node->child2 = a(depth);
                return node;
        }
        else if (tk.tkIdentifier == MINUS_tk){          /* Predict <A> -> <M> - <A> */
                node->token = tk;
                tk = scan(inputfp, &lineNo);
                node->child2 = a(depth);
                return node;
        }
        else{
                return node;
        }
}

node_t *m(int depth){
        depth++;
        node_t *node = createNode("<M>", depth);

        if(tk.tkIdentifier == MINUS_tk){                  /* Predict <M> -> - <M> */
                node->token = tk;
                tk = scan(inputfp, &lineNo);
                node->child1 = m(depth);
                return node;
        }
        else{
                node->child1 = r(depth);                /* Predict <M> -> <R> */
                return node;
        }
}


node_t *r(int depth){
        depth++;
        node_t *node = createNode("<R>", depth);

        if(tk.tkIdentifier == LPAREN_tk){               /* Predict <R> -> (<expr>) */
                tk = scan(inputfp, &lineNo);
                node->child1 = expr(depth);
                if(tk.tkIdentifier == RPAREN_tk){
                        tk = scan(inputfp, &lineNo);
                        return node;
                }
                else{
                        printf("Parser Error: Invalid Token, Expected RPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                        exit(EXIT_FAILURE);
                }
        }
        else if(tk.tkIdentifier == IDENT_tk){           /* Predict <R> -> Identifier */
                node->token = tk;
                tk = scan(inputfp, &lineNo);
                return node;
        }
        else if(tk.tkIdentifier == NUM_tk){             /* Predict <R> -> Integer */
                node->token = tk;
                tk = scan(inputfp, &lineNo);
                return node;
        }
        else{
                printf("Parser Error: Invalid Token, Expected LPAREN_tk, IDENT_tk or INT_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                exit(EXIT_FAILURE);
        }
}

node_t *stats(int depth){                               /* k=0 No prediction */
        depth++;                                        /* <stats> -> <stat> <mStat> */

        node_t *node = createNode("<stats>", depth);
        node->child1 = stat(depth);
        node->child2 = mStat(depth);
        return node;
}

node_t *mStat(int depth){
        depth++;
        node_t *node = createNode("<mStat>", depth);
        /*
          Predict <mStat> -> <stat> <mStat>
          process mStat follow to determine empty prediction 
         */
        if(tk.tkIdentifier == READ_tk || tk.tkIdentifier == PRINT_tk || tk.tkIdentifier == BEGIN_tk ||
        tk.tkIdentifier == COND_tk || tk.tkIdentifier == ITER_tk || tk.tkIdentifier == IDENT_tk){
                node->child1 = stat(depth);
                node->child2 = mStat(depth);
                return node;
        }
        else{
                return NULL;                            /* Predict <mStat> -> empty */
        }
}


node_t *stat(int depth){
        depth++;
        node_t *node = createNode("<stat>", depth);

        if(tk.tkIdentifier == READ_tk){                 /* Predict <stat> -> <in> */
                node->child1 = in(depth);
                return node;
        }
        else if(tk.tkIdentifier == PRINT_tk){           /* Predict <stat> -> <out> */
                node->child1 = out(depth);
                return node;
        }
        else if(tk.tkIdentifier == BEGIN_tk){           /* Predict <stat> -> <block> */
                node->child1 = block(depth);
                return node;
        }
        else if(tk.tkIdentifier == COND_tk){        /* Predict <stat> -> <if> */
                node->child1 = ifStatement(depth);
                return node;
        }
        else if(tk.tkIdentifier == ITER_tk){           /* Predict <stat> -> <loop> */
                node->child1 = loop(depth);
                return node;
        }
        else if(tk.tkIdentifier == IDENT_tk){           /* Predict <stat> -> <assign> */
                node->child1 = assign(depth);
                return node;
        }
	else{
                printf("Parser Error: Invalid Token, Expected READ_tk, PRINT_tk, START_tk, DECISION_tk, WHILE_tk or IDENT_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                exit(EXIT_FAILURE);
        }
}


node_t *in(int depth){                          /* k=0 No prediction */
        depth++;                                /* <in> -> read ( Identifier ) : */
        node_t *node = createNode("<in>", depth);

        if(tk.tkIdentifier == READ_tk){
                tk = scan(inputfp, &lineNo);

                if(tk.tkIdentifier == LPAREN_tk){
                        tk = scan(inputfp, &lineNo);

                        if(tk.tkIdentifier == IDENT_tk){
                                node->token = tk;
                                tk = scan(inputfp, &lineNo);

                                if(tk.tkIdentifier == RPAREN_tk){
                                        tk = scan(inputfp, &lineNo);
                                        
					if(tk.tkIdentifier == COLON_tk){
                                        	tk = scan(inputfp, &lineNo);
						return node;
                                        
                                	}
					else
					{
						printf("Parser Error: Invalid Token, Expected COLON_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                        			exit(EXIT_FAILURE);
					}
                                }
                                else
				{
                                        printf("Parser Error: Invalid Token, Expected RPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                                        exit(EXIT_FAILURE);
                                }
                        }
                        else
			{
                                printf("Parser Error: Invalid Token, Expected IDENT_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                                exit(EXIT_FAILURE);
                        }
                }
                else
		{
                        printf("Parser Error: Invalid Token, Expected LPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                        exit(EXIT_FAILURE);
                }
        }
        else{
                printf("Parser Error: Invalid Token, Expected READ_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                exit(EXIT_FAILURE);
        }
}

node_t *out(int depth){                                 /* k=0 No prediction */
        depth++;                                        /* <out> -> print ( <expr>  ) : */
        node_t *node = createNode("<out>", depth);

        if(tk.tkIdentifier == PRINT_tk){
                tk = scan(inputfp, &lineNo);

                if(tk.tkIdentifier == LPAREN_tk){
                        tk = scan(inputfp, &lineNo);
                        node->child1 = expr(depth);
                        
			if(tk.tkIdentifier == RPAREN_tk){
				tk = scan(inputfp, &lineNo);

				if(tk.tkIdentifier == COLON_tk){
                                	tk = scan(inputfp, &lineNo);
					return node;
				}
				else
				{
					printf("Parser Error: Invalid Token, Expected COLON_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                        		exit(EXIT_FAILURE);
				}

			}
			else
			{
				printf("Parser Error: Invalid Token, Expected RPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                        	exit(EXIT_FAILURE);
			}
                }
                else{
                        printf("Parser Error: Invalid Token, Expected LPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                        exit(EXIT_FAILURE);
                }
        }
        else{
                printf("Parser Error: Invalid Token, Expected PRINT_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                exit(EXIT_FAILURE);
        }
}


node_t *ifStatement(int depth){                         /* k=0 No prediction */
        depth++;                                        /* <if> -> cond ( <expr> <RO> <expr> ) <stat> */
        node_t *node = createNode("<if>", depth);

        if(tk.tkIdentifier == COND_tk){
                tk = scan(inputfp, &lineNo);

                if(tk.tkIdentifier == LPAREN_tk){
                        tk = scan(inputfp, &lineNo);
                        node->child1 = expr(depth);
                        node->child2 = ro(depth);
                        node->child3 = expr(depth);

                        if(tk.tkIdentifier == RPAREN_tk){
                                tk = scan(inputfp, &lineNo);
                                node->child4 = stat(depth);
                                return node;
                        }
                        else{
                                printf("Parser Error: Invalid Token, Expected RPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                                exit(EXIT_FAILURE);
                        }
                }
                else{
                        printf("Parser Error: Invalid Token, Expected LPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                        exit(EXIT_FAILURE);
                }
        }
        else{
                printf("Parser Error: Invalid Token, Expected COND_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                exit(EXIT_FAILURE);
        }
}

node_t *loop(int depth){                        /* k=0 No prediction */
        depth++;                                /* <loop> -> iter ( <expr> <RO> <expr> ) <stat> */
        node_t *node = createNode("<loop>", depth);

        if(tk.tkIdentifier == ITER_tk){
                tk = scan(inputfp, &lineNo);

                if(tk.tkIdentifier == LPAREN_tk){
                        tk = scan(inputfp, &lineNo);
                        node->child1 = expr(depth);
                        node->child2 = ro(depth);
                        node->child3 = expr(depth);

                        if(tk.tkIdentifier == RPAREN_tk){
                                tk = scan(inputfp, &lineNo);
                                node->child4 = stat(depth);
                                return node;
                        }
                        else{
                                printf("Parser Error: Invalid Token, Expected RPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                                exit(EXIT_FAILURE);
                        }
                }
                else{
                        printf("Parser Error: Invalid Token, Expected LPAREN_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                        exit(EXIT_FAILURE);
                }

        }
        else{
                printf("Parser Error: Invalid Token, Expected ITER_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                exit(EXIT_FAILURE);
        }
}


node_t *assign(int depth){                                      /* k=0 No prediction */
        depth++;                                                /* <assign> -> Identifier  = <expr> : */
        node_t *node = createNode("<assign>", depth);

        if(tk.tkIdentifier == IDENT_tk){
                node->token = tk;
                tk = scan(inputfp, &lineNo);

                if(tk.tkIdentifier == EQUAL_tk){
                        tk = scan(inputfp, &lineNo);
                        node->child1 = expr(depth);

                        if(tk.tkIdentifier == COLON_tk){
                                tk = scan(inputfp, &lineNo);
                                return node;
                        }
                        else{
                                printf("Parser Error: Invalid Token, Expected COLON_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                                exit(EXIT_FAILURE);
                        }
                }
                else{
                        printf("Parser Error: Invalid Token, Expected EQUAL_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                        exit(EXIT_FAILURE);
                }
        }
        else{
                printf("Parser Error: Invalid Token, Expected IDENT_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
                exit(EXIT_FAILURE);
        }
}

node_t *ro(int depth){
        depth++;
        node_t *node = createNode("<RO>", depth);
        if(tk.tkIdentifier == LESSTHAN_tk){                    /* Predict <RO> -> < */
                node->token = tk;
                tk = scan(inputfp, &lineNo);
                if(tk.tkIdentifier == EQUAL_tk){                /* Predict <RO> -> <= */
                        /* Store secondary non-syntactic token */
                        node->token2 = tk;
                        tk = scan(inputfp, &lineNo);
                        return node;
                }
                return node;
        }
        else if(tk.tkIdentifier == GREATERTHAN_tk){               /* Predict <RO> -> > */
                node->token = tk;
                tk = scan(inputfp, &lineNo);
                if(tk.tkIdentifier == EQUAL_tk){                /* Predict <RO> -> >= */
                        node->token2 = tk;
                        tk = scan(inputfp, &lineNo);
                        return node;
                }
                return node;
        }
        else if(tk.tkIdentifier == EQUAL_tk){                /* Predict <RO> -> == */
                node->token = tk;
                tk = scan(inputfp, &lineNo);
                return node;
        }
	 else if(tk.tkIdentifier == EQUAL_tk){               /* Predict <RO> -> = */
                node->token = tk;
                tk = scan(inputfp, &lineNo);
                return node;
        }
        else{
            printf("Parser Error: Invalid Token, Expected DBLLARROW_tk, DBLRARROW_tk, DBLEQUAL_tk or EQUALBANG_tk -- %s Received At Line %d\n", tokenNames[tk.tkIdentifier], lineNo);
            exit(EXIT_FAILURE);
        }
}


/* New node creation function */
node_t *createNode(char *name, int depth){
        node_t *newNode;
        newNode = malloc(sizeof(node_t));

        if(newNode == NULL){
                printf("Node Allocation Error. Out Of Memory\n");
                exit(EXIT_FAILURE);
        }

        strcpy(newNode->name, name);
        newNode->depth = depth;

        newNode->child1 = NULL;
        newNode->child2 = NULL;
        newNode->child3 = NULL;
        newNode->child4 = NULL;

        return newNode;
}

