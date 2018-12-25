
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "node.h"
#include "semantics.h"

FILE *outfile;		//extern .asm file pointer

int main(int argc, char* argv[]){

	//implicit file extension
  	static const char *EXTENSION = ".fs18";

	/* Parse tree */
        node_t *tree;

	// Stack initialization
	stackInit();

	// Temp var array initialization
	tempArrInit();

  	if (argc > 2){
		printf("Usage:\n");
                printf("./comp\t\t - Invokes program and user is prompted for input.\n");
                printf("\t\t\t Keyboard simulated EOF terminates input.\n\n");
                printf("./comp < filename\t - Invokes program and input is read via stdin file redirection.\n");
                printf("\t\t\t filename must be precise including extension (if any).\n\n");
                printf("./comp filename\t - Invokes program and input is read directly from file.\n");
                printf("\t\t\t filename should not include implicit extension (.fs18)\n\n");
                exit(EXIT_FAILURE);
	}

  	else if(argc == 1){
    		inputfp = stdin;
		// no input/redirection output file creation
		char *outName = "out.asm";
		outfile = fopen(outName, "w");

		/* unlikely, but covers stdin open failure */
        	if(inputfp == NULL){
                	fprintf(stderr, "Unable To Open stdin: %s\n", strerror(errno));
                	exit(EXIT_FAILURE);
        	}
		
		/* Ask user for input if not redirected */
        	if (isatty(0)){
                	printf("\nNo Filename Given. \nEnter Input: ");
        	}

                /* Call parser to build tree */
                if(!feof(inputfp)){
                        tree = parser();
                }

                /* Check Semantics */
                semanticCheck(tree, 0);
        	printf("Semantic Check Complete. Code Generated in %s\n", outName);

                fclose(inputfp);
                fclose(outfile);
        }
  	else{
    		char *fileName = malloc(sizeof(argv[1]) + sizeof(EXTENSION));
		char *outName = malloc(strlen(argv[1]) + strlen(".asm") + 2);
 		if(fileName == NULL){
                        printf("Unable To Allocate Memory For Input File Pointer\n");
                        exit(EXIT_FAILURE);
                }

                /* implicit named input file handling */
    		strncat(fileName, argv[1], strlen(argv[1]));
    		strncat(fileName, EXTENSION, strlen(EXTENSION));
    		inputfp = fopen(fileName, "r");

		 /* output file creation for implicitly named input file */
        	strncat(outName, argv[1], strlen(argv[1]));
        	strncat(outName, ".asm", strlen(".asm"));
                outfile = fopen(outName, "w");

		/* covers input file open failure */
        	if (inputfp == NULL){
                	fprintf(stderr, "Unable To Open '%s%s': %s\n", argv[1], EXTENSION, strerror(errno));
                	exit(EXIT_FAILURE);
        	}

	 	/* Call Parser and build tree */
                if(!feof(inputfp)){
                        tree = parser();
                }

                /* Check Semantics and Generate Code */
                semanticCheck(tree, 0);
        	printf("Semantic Check Complete. Code Generated in %s\n", outName);

                fclose(inputfp);
                fclose(outfile);
        }

  return 0;
}
