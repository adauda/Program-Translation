
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "testScanner.h"

int main(int argc, char* argv[]){

  	FILE *inputfp = NULL; 
  	static const char *EXTENSION = ".fs18";

  	if (argc > 2){
    		printf("Usage:\n");
		printf("./scanner [filename without the extension]\n");
		printf("./scanner < [filename with the extension]\n");
		printf("./scanner then press enter then type word then press enter\n");
    		exit(EXIT_FAILURE);
	}

  	else if(argc == 1){
    		inputfp = stdin;
		printf("Enter Input:\n");

		if(!feof(inputfp)){
				testScanner(inputfp);
		}

		 if(!(inputfp)){
                        perror("System error file not found");
                        exit(EXIT_FAILURE);
                }

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
                	printf("./scanner [filename without the extension]\n");
                	printf("./scanner < [filename with the extension]\n");
               		printf("./scanner then press enter then type word then press enter\n");
      			exit(EXIT_FAILURE);
    	}

    
	if(!feof(inputfp))
		testScanner(inputfp);

    	if (feof(inputfp))
      		fclose(inputfp);
  }

  return 0;
}
