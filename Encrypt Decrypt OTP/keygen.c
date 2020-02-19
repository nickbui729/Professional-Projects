
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


int main(int argc, char *argv[]) {
	

	FILE* filedescriptor1 = NULL;
	char output[512];

	srand(time(NULL)); // Seed rand first

	if (argc < 2){

    	fprintf(stderr, "Error. No key length input.\n");
    	exit(1);
    }

    if (argc > 3){

    	if (strcmp(argv[2], ">")){
    		strcpy(output,argv[3]);

			filedescriptor1 = fopen(output , "W");

    		if (filedescriptor1 == NULL){  	
            	fprintf(stderr, "cannot open %s for output\n", output); // error checking for file open
            	exit(1);
        	}

        }

        else{

        	fprintf(stderr,"Invalid input.");   
        	exit(1);
        }

	} 

    	
    

    int i = 0;
    int length;
    int randkey;
    char* keystring;

	length = atoi(argv[1]); // convert the first arg into an int
    keystring = malloc(sizeof(char) * length + 1);
    
	char randomletter[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

    for(i = 0; i < length; i++){ // loop until we reach the max length of key arg

    	randkey = rand() % 27;
    	keystring[i] = randomletter[randkey];

    	//printf(%s, keystring);

    } 

    
    keystring[length] = '\n';
    keystring[length+1] = '\0';

    if (filedescriptor1 != NULL){


    	fprintf(filedescriptor1, keystring, length);
    	fclose(filedescriptor1);
    }

    else{
    
    	fprintf(stdout, keystring);
	}

	free(keystring);
	return 0;

}
