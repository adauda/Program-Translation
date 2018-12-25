#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"

token_t stack[MAXSTACKSIZE];	//initialize arry of tokens as stack
static int stackVars = 0;	//total number of vars on the stack
static int scopeStart = 0;	//used to track/assign scope starting point on stack
int i = 0;			//loop counter var

//initialize all stack location strings to prevent seg fault
void stackInit(void){
	for(i = 0; i < MAXSTACKSIZE + 1; i++){
		stack[i].tkString = "";
	}
	return;
}

//stack push
void push(token_t stack_tk){
	//if stack is full, warn stack overflow
	if(stackVars == MAXSTACKSIZE){
		printf("Stack is full\n");
		exit(EXIT_FAILURE);
	}
	//check for variable scope existence before inserting
	for(i = scopeStart; i < stackVars; i++){
		if(strcmp(stack[i].tkString, stack_tk.tkString) == 0){
			printf("%s Already Declared In This Scope On Line: %d\n", stack_tk.tkString, stack[i].lineNo);
			exit(EXIT_FAILURE);
		}
	}	
	//push token to the stack
	stack[stackVars] = stack_tk;
	/*
	for(i = 0; i < stackVars; i++){
		printf("%i, %s\n", i, stack[i].tkString);
	}
	*/
	stackVars++;		//increment number of variables on stack to keep stack size current
}

//pop scope vars when called
void pop(int scopeStart){
	for(i = stackVars; i > scopeStart; i--){
		//decrement number of variables on stack to simulate removal and keep stack size current
		stackVars--;
		stack[i].tkString = NULL;
	}
}

//make sure variable doesn't exisit in the local scope stack before pushing onto stack
int find(char *var){
	//search current scope for var string and return position
	for(i = stackVars; i > scopeStart - 1; i--){
		//printf("stackVar: %i, scopeStart: %i\n", stackVars, scopeStart);
		//printf("find token on stack: %s\n", stack[i].tkString);
		if(strcmp(stack[i].tkString, var) == 0){
			return stackVars - 1 - i;
		}
	}
	//not found
	return -1;
}

//make sure variable exists in scope(or parent) before use
int varExists(char *var){
	//search currnt scope for var string and return position
	for(i = stackVars - 1; i > - 1; i--){
		if(strcmp(stack[i].tkString, var) == 0){
			return stackVars - 1 - i;
		}
	}
	//not found
	return -1;
}

void semanticCheck(node_t *node, int count){
	//empty tree
	if(node == NULL){
		return;
	}
	//process program node
	if(strcmp(node->name, "<program>") == 0){
		//start of program no vars
		int numVars = 0;
		//process expected possible number of children if not null (<program> can have two child nodes)
		if(node->child1 != NULL)
		semanticCheck(node->child1, numVars);
		if(node->child2 != NULL)
		semanticCheck(node->child2, numVars);
	}
/*
	//process vars node
	else if(strcmp(node->name, "<vars>") == 0){
		int found;
		if(count > 0){
			found = find(node->token.tkString);
                        if(found == -1 || found > count){
                                push(node->token);
                                count += 1;
                        }
                        else if(found < count){
                                printf("'%s' 3 Is Already Defined Within This Scope On Line: %d\n", node->token.tkString, node->token.lineNo);
                                exit(EXIT_FAILURE);
                        }
		}
		if(node->child1 != NULL)
                semanticCheck(node->child1, count);
	}
*/

	//process vars node
	else if(strcmp(node->name, "<vars>") == 0){
		int found;
		//scopeStart = stackVars;
		found = find(node->token.tkString);
		//printf("found: %i, count: %i\n", found, count);
		//if not found in scope or defined gloabally, push onto stack
		if(found == -1 || found > count){
			push(node->token);
			count += 1;
		}
		//erro/exit if already defined within scope
		else if (found < count){
			printf("'%s' Is Already Defined Within This Scope On Line: %d\n", node->token.tkString, node->token.lineNo);
                        exit(EXIT_FAILURE);
		}
		//process expectes possible number of children if not null(<vars> can have one child node)
		if(node->child1 != NULL)
		semanticCheck(node->child1, count);
	}
	//process block node
	else if(strcmp(node->name, "<block>") == 0){
		//new block resets scope and number of variables in scope
		int numVars = 0;
		//set scope = TOS
		scopeStart = stackVars;
		//process expected possible number of children if not null(<block> can have two child nodes)
		if(node->child1 != NULL)
		semanticCheck(node->child1, numVars);
		if(node->child2 != NULL)
		semanticCheck(node->child2, numVars);
		//pop scope variables before exit
		//pop(scopeStart);
	}

	else if(strcmp(node->name, "<exp>") == 0){
		//process as correct prediction using info from parse
		if(node->token.tkIdentifier == ASTERISK_tk){
			if(node->child1 != NULL)
				semanticCheck(node->child1, count);
			if(node->child2 != NULL)
				semanticCheck(node->child2, count);	
		}
		if(node->token.tkIdentifier == SLASH_tk){
			if(node->child1 != NULL)
                                semanticCheck(node->child1, count);
                        if(node->child2 != NULL)
                                semanticCheck(node->child2, count);
		}
		else{
                        if(node->child1 != NULL)
                        semanticCheck(node->child1, count);
                }
	}
	else if(strcmp(node->name, "<A>") == 0){
                if(node->token.tkIdentifier == PLUS_tk){
                        if(node->child1 != NULL)
                        semanticCheck(node->child1, count);
                        if(node->child2 != NULL)
                        semanticCheck(node->child2, count);
                }
                if(node->token.tkIdentifier == MINUS_tk){
                        if(node->child1 != NULL)
                        semanticCheck(node->child1, count);
                        if(node->child2 != NULL)
                        semanticCheck(node->child2, count);
                }
                else{
                        if(node->child1 != NULL)
                        semanticCheck(node->child1, count);
                }
        }

	else if(strcmp(node->name, "<M>") == 0){
                if(node->token.tkIdentifier == MINUS_tk){
                        if(node->child1 != NULL)
                        semanticCheck(node->child1, count);
                }
                else{
                        if(node->child1 != NULL)
                        semanticCheck(node->child1, count);
                }
        }
	
	else if(strcmp(node->name, "<R>") == 0){
                /* Check to make sure identifier was declared before use */
                if(node->token.tkIdentifier == IDENT_tk){
                        int exists = varExists(node->token.tkString);
                        if(exists == -1){
                                printf("Variable '%s' On Line: %d Was Not Declared In This Scope.\n", node->token.tkString, node->token.lineNo);
                                exit(EXIT_FAILURE);
                        }
                }
                else{
                        if(node->child1 != NULL)
                        semanticCheck(node->child1, count);
                }
        }

	else if(strcmp(node->name, "<in>") == 0){
                int exists = varExists(node->token.tkString);
                if(exists == -1){
                        printf("Variable '%s' On Line: %d Was Not Declared In This Scope.\n", node->token.tkString, node->token.lineNo);
                        exit(EXIT_FAILURE);
                }
        }
	
	else if(strcmp(node->name, "<out>") == 0){
                if(node->child1 != NULL)
                semanticCheck(node->child1, count);
        }

	else if(strcmp(node->name, "<if>") == 0){
                if(node->child1 != NULL)
                semanticCheck(node->child1, count);
                if(node->child3 != NULL)
                semanticCheck(node->child3, count);
                if(node->child4 != NULL)
                semanticCheck(node->child4, count);
        }

	else if(strcmp(node->name, "<assign>") == 0){
                int exists = varExists(node->token.tkString);
                if(exists == -1){
                        printf("Variable '%s' On Line: %d Was Not Declared In This Scope.\n", node->token.tkString, node->token.lineNo);
                        exit(EXIT_FAILURE);
                }
                if(node->child1 != NULL)
                semanticCheck(node->child1, count);
        }

	//catch all process productions with only non-terminals
	 else{
                if(node->child1 != NULL)
                semanticCheck(node->child1, count);
                if(node->child2 != NULL)
                semanticCheck(node->child2, count);
                if(node->child3 != NULL)
                semanticCheck(node->child3, count);
                if(node->child4 != NULL)
                semanticCheck(node->child4, count);
        }
        return;

}
