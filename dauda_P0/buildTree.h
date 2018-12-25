#ifndef BUILDTREE_H
#define BUILDTREE_H

//Prototypes for utility functions 
node_t *buildTree(FILE *fp);
node_t *insertTree(node_t *x, node_t *y);
int searchTree(node_t *z, int j, char data[]);

#endif
