#include "node.h"

extern FILE *inputfp;	//global inpute file pointer

node_t *parser();	//parser function prototype

//cfg funtion prototypes
node_t *program();
node_t *block(int depth);
node_t *vars(int depth);
node_t *block(int depth);
node_t *expr(int depth);
node_t *a(int depth);
node_t *m(int depth);
node_t *r(int depth);
node_t *stats(int depth);
node_t *mStat(int depth);
node_t *stat(int depth);
node_t *in(int depth);
node_t *out(int depth);
node_t *ifStatement(int depth);
node_t *loop(int depth);
node_t *assign(int depth);
node_t *ro(int depth);

//node creation function prototype
node_t *createNode(char *name, int depth);
