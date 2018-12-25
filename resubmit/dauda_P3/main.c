
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "node.h"
#include "testTree.h"
#include "semantics.h"

int main(int argc, char* argv[]){

	//implicit file extension
  	static const char *EXTENSION = ".fs18";

	/* Parse tree */
        node_t *tree;

	//stack initialization
	stackInit();

  	if (argc > 2){
		printf("Usage:\n");
                printf("./statSem\t\t - Invokes program and user is prompted for input.\n");
                printf("\t\t\t Keyboard simulated EOF terminates input.\n\n");
                printf("./statSem < filename\t - Invokes program and input is read via stdin file redirection.\n");
                printf("\t\t\t filename must be precise including extension (if any).\n\n");
                printf("./statSem filename\t - Invokes program and input is read directly from file.\n");
                printf("\t\t\t filename should not include implicit extension (.fs18)\n\n");
                exit(EXIT_FAILURE);
	}

  	else if(argc == 1){
    		inputfp = stdin;
		printf("Enter Input:\n");

		/* Call parser to build tree */
		if(!feof(inputfp)){
				//testScanner(inputfp);
				tree = parser();
		}

		 if(!(inputfp)){
                        perror("System error file not found");
                        exit(EXIT_FAILURE);
                }

		/* Check Semantics */
 		semanticCheck(tree, 0);
		printf("Semantic Check Complete\n");

		/* Print tree */
		printf("Parse Tree\n");
                preOrderPrint(tree);
                fclose(inputfp);

	}
  	else{
    		char *fileName = malloc(sizeof(argv[1]) + sizeof(EXTENSION));
    		strncat(fileName, argv[1], strlen(argv[1]));
    		strncat(fileName, EXTENSION, strlen(EXTENSION));
    		inputfp = fopen(fileName, "r");

    		if (inputfp == NULL){
			perror("Program error ");
			printf("Usage:\n");
                	printf("./statSem [filename without the extension]\n");
                	printf("./statSem < [filename with the extension]\n");
               		printf("./statSem then press enter then type the program\n");
      			exit(EXIT_FAILURE);
    		}

		/* Call Parser and build tree */
                if(!feof(inputfp)){
                        tree = parser();
                }

		/* Check Semantics */
		semanticCheck(tree, 0);
		printf("Semantic Check Complete\n");

                /* Print Tree */
		printf("Parse Tree:\n");
                preOrderPrint(tree);
                fclose(inputfp);
        }

  return 0;
}
