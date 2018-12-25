#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "node.h"
#include "traversals.h"

void traversePreorder(node_t *root, int level){
	node_l *pre;

	if(root != NULL){
		printf("%*c", level * 2, ' ' );
		printf("%d %s", root->size, root->list->data);

		pre = root->list;

		// print data while there is next data. 
		while(pre->next){
			printf(" %s", pre->next->data);
			pre = pre->next;
		}
		printf("\n");

		traversePreorder(root->lptr, level + 1);
		traversePreorder(root->rptr, level + 1);

	}
}

void traverseInorder(node_t *root, int level){
	node_l *pt;

	if(root != NULL){
		traverseInorder(root->lptr, level + 1);

		printf("%*c", level * 2, ' ');
		printf("%d %s", root->size, root->list->data);

		pt = root->list;

		// print data while there is next data. 
		while(pt->next){
			printf(" %s", pt->next->data);
			pt = pt->next;
		}
		printf("\n");

		traverseInorder(root->rptr, level + 1);
	}
}

void traversePostorder(node_t *root, int level){
	node_l *pt;

	if(root != NULL){
		traversePostorder(root->lptr, level + 1);
		traversePostorder(root->rptr, level + 1);

		printf("%*c", level * 2, ' ');
		printf("%d %s", root->size, root->list->data);

		pt = root->list;

		// print data while there is next data. 
		while(pt->next){
			printf(" %s ", pt->next->data);
			pt = pt->next;
		}
		printf("\n");
	}
}

