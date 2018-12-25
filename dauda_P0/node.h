#ifndef NODE_H
#define NODE_H

typedef struct nodeList{
	char data[500];
	struct nodeList *next;
}node_l;

typedef struct node{
	node_l	*list;
	struct node *lptr, *rptr;
	int size;
}node_t;

#endif
