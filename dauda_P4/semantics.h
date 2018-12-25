//stack and semantic operations definition
//
#ifndef SEMANTICS_H
#define SEMANTICS_H
#define MAXSTACKSIZE 100	
#include "token.h"
#include "node.h"

extern FILE *outfile;

// temporary var and label operations
void tempArrInit(void);
char *newTemp(void);
char *newLab(void);

//stack operation
void push(token_t stack_tk);
void pop(int scopeStart);
void stackInit(void);

//variable or stack search operations
int find(char *var);
int varExists(char *var);

//main semantic checking function
void semanticCheck(node_t *node, int count);

#endif

