
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "node.h"
#include "testTree.h"

int main(int argc, char* argv[]){

  	static const char *EXTENSION = ".fs18";

	/* Parse tree */
        node_t *tree;

  	if (argc > 2){
		printf("Usage:\n");
                printf("./frontEnd\t\t - Invokes program and user is prompted for input.\n");
                printf("\t\t\t Keyboard simulated EOF terminates input.\n\n");
                printf("./frontEnd < filename\t - Invokes program and input is read via stdin file redirection.\n");
                printf("\t\t\t filename must be precise including extension (if any).\n\n");
                printf("./frontEnd filename\t - Invokes program and input is read directly from file.\n");
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

		/* Print tree */
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
                	printf("./frontEnd [filename without the extension]\n");
                	printf("./frontEnd < [filename with the extension]\n");
               		printf("./frontEnd then press enter then type the program\n");
      			exit(EXIT_FAILURE);
    		}

		/* Call Parser and build tree */
                if(!feof(inputfp)){
                        tree = parser();
                }
                /* Print Tree */
                preOrderPrint(tree);
                fclose(inputfp);
        }


/*    
	if(!feof(inputfp))
		testScanner(inputfp);

    	if (feof(inputfp))
      		fclose(inputfp);
*/

  return 0;
}
