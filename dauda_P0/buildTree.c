#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "node.h"
#include "buildTree.h"

node_t *buildTree(FILE *file)
{
	
	node_t *s, *root;
	int i = 0, line = 1;
	int length;
	int search;

	root = NULL;

	char data;
	char buffer[1028];
	memset(buffer, '\0', sizeof(buffer));

	// if file exist
	if(file)
	{
		// while not end of file
		while( (data = fgetc(file)) != EOF)
		{
				
				// increase line number on a new line
				if(data == '\n')
					line++;

				if(isspace(data))
				{
					
					length = strlen(buffer);
					search = 0;

					// search tree to see if node already present
					search = searchTree(root, length, buffer);

					// If node doesn't exits, create it and insert
					if(!search)
					{
						// create a node if needed
						s = (node_t *) malloc(sizeof(node_t));
						s->list = (node_l *) malloc(sizeof(node_l));
						s->size = length;

						s->lptr = NULL;
						s->rptr = NULL;

						strcpy(s->list->data, buffer);
						s->list->next = NULL;

						// Insert into tree
						if(root == NULL)
							root = s;
						else
							root = insertTree(root, s);
					}

					// Clear buffer;
					memset(buffer, '\0', sizeof(buffer));
					i = 0;					
				}
				else
				{
					buffer[i] = data;
					i++;
				}
		}
	}
	fclose(file);

	return root;

}

node_t *insertTree(node_t *x, node_t *y){

	if(x->rptr == NULL && y->size > x->size)
		x->rptr = y;
	else if(x->rptr != NULL && y->size > x->size)
		insertTree(x->rptr, y);

	if(x->lptr == NULL && y->size < x->size)
		x->lptr = y;
	else if(x->lptr != NULL && y->size < x->size)
		insertTree(x->lptr, y);

	return x;
}

int searchTree(node_t *x, int k, char data[]){
	node_t *tmpo;
	node_l *d;

	tmpo = x;
	while(tmpo != NULL)
	{
		// if node is found, insert data.
		if(k == tmpo->size)
		{
			d = tmpo->list;
			// if duplicate is found return
			if(strcmp(d->data, data) == 0)
				return 0;

			while(d->next)
			{
				d = d->next;
				if(strcmp(d->data, data) == 0)
					return 0;
			}

			d->next = (node_l *) malloc(sizeof(node_l));
			strcpy(d->next->data, data);
			return 1;
		}
		
		if(k < tmpo->size)
			tmpo = tmpo->lptr;
		else
			tmpo = tmpo->rptr;
	}
	
	return 0;
}
