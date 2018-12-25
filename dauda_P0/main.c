#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "node.h"
#include "buildTree.h"
#include "traversals.h"

int main(int argc, char *argv[])
{

	FILE *f;
	char *fName, *fpr, *ino, *pos;

	//if file is not given, read from the keyboard.
	if(argc == 1){

		printf("To enter input: Start typing.\n"); 
		printf("To End: Press enter, then press Control + D\n");
		printf("\n");

		f = stdin;
	}
	//read from file, if correct file name is given
	else{
		//memory allocation for filename
		fName = malloc(strlen(argv[1] + 4));
		strcpy(fName, argv[1]);
		strcat(fName, ".fs18");
		f = fopen(fName, "r");
		
		//allocate memory and create filename for preorderfile
		fpr = malloc(strlen(argv[1] + 4));
                strcpy(fpr, argv[1]);
                strcat(fpr, ".preorder");

		//allocate memory and create filename for inorderfile
		ino = malloc(strlen(argv[1] + 4));
                strcpy(ino, argv[1]);
                strcat(ino, ".inorder");

		//allocate memory and create filename for postorderfile
		pos = malloc(strlen(argv[1] + 4));
                strcpy(pos, argv[1]);
                strcat(pos, ".postorder");
	}
	
	//if an invalid file or command line is given, print error message
	if(!f){
		printf("Error! Usage: [%s] [filename without extension]\n", argv[0]);
		printf("Make sure you enter correct filename\n");
		return 1;
	}
	
	// build the tree
	node_t *root = buildTree(f);

	// traverse tree
	//
	//if filename is not given, that is input from keyboard.
	if (argc == 1){
		printf("Output files: out.preorder, out.inorder, and out.postorder\n");

                freopen("out.preorder","w",stdout);
                traversePreorder(root,0);
                fclose(stdout);

                freopen("out.inorder","w",stdout);
               	traverseInorder(root,0);
                fclose(stdout);

                freopen("out.postorder","w",stdout);
                traversePostorder(root,0);
                fclose(stdout);
	}

	// if a filename is given.
	else{
		printf("Output files: %s, %s, and %s\n", fpr, ino, pos);

		freopen(fpr,"w",stdout);
		traversePreorder(root,0);
		fclose(stdout);

		freopen(ino,"w",stdout);
                traverseInorder(root,0);
                fclose(stdout);

		freopen(pos,"w",stdout);
                traversePostorder(root,0);
                fclose(stdout);
	}
	

	return 0;
}
