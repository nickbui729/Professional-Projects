/********************************************************

Nicholas Bui

CS 344 WINTER 2019

PROJECT #3 - SMALLSH

---This program will create a small shell with functions 
	similar to that of the bash unix shell. The built in 
	commands "cd", "exit", "status", and "#" will be 
	implemented. All other calls will be handled by the
	exec function---

*********************************************************/

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

#define MAX_PID 1000
#define INT_MAX -1

// Global variables

bool ignoreBG = false;
bool backgroundstatus = false;
pid_t pidArray[MAX_PID];
int pidcount = 0;
int status;
int signum;

/**************************************

FUNCTION: void executeCMD

Takes in char**args and int k as input.
This function will run and handle error
checking for our forked processes.

**************************************/
void executeCMD(char** args, int k){
		
		// Organization taken from lecture 3.1

		pid_t spawnPid = -5;
		int childExitMethod = -5;

		spawnPid = fork();
		
		if (spawnPid == -1){

			perror("Hull Breach!\n");
			exit(1);
		}
		
		// Redirection taken from: https://stackoverflow.com/questions/11515399/implementing-shell-in-c-and-need-help-handling-input-output-redirection

		else if (spawnPid == 0){

			int filedescriptor0, filedescriptor1; // File descriptors
			int in =0;
			int out =0; // Tracks for "<" or ">" redirection
			char input[512]; // Store input file 
			char output[512]; // Store output file 
			int i;
			

    		for(i=0; args[i] != NULL; i++){ // Check for where "<" or ">" are and then set the value to NULL 
    		

        		if(strcmp(args[i],"<")==0){
        		       
            		args[i]=NULL;
            		strcpy(input,args[i+1]);
            		in=2; // Means that we have input redirection           
        		}               

        		else if (strcmp(args[i],">")==0){
              	
            		args[i]=NULL;
            		strcpy(output,args[i+1]);
            		out=2; // Means that we have output redirection
        		}         
   		    }

   		    if(out==0 && backgroundstatus){
   		    	filedescriptor1 = open("/dev/null", O_WRONLY); // background command standard input redirection
   		    	

   		    	if(filedescriptor1 < 0){
            		
            		printf("Couldn't open the output file dev/null."); // error checking for file open
            		fflush(stdout);
            		status = 1;
            		exit(1);
        		}           

        		dup2(filedescriptor1, STDOUT_FILENO); // set fd for stdout
        		close(filedescriptor1);
   			
   		    }

   		    else if(out){
  
				if ((filedescriptor1 = creat(output , 0644)) < 0) {
            		
            		printf("cannot open %s for output\n", output); // error checking for file open
            		fflush(stdout);
            		status = 1;
            		exit(1);
        		}           

        		dup2(filedescriptor1, STDOUT_FILENO); 
        		close(filedescriptor1);
   			
   			}

   		    if(in==0 && backgroundstatus){
   		    	filedescriptor0 = open("/dev/null", O_RDONLY);
   		    	
   		    	if (filedescriptor0 < 0){ // Error checking for creating file descriptor
            	
            		printf("Couldn't open input file dev/null.\n");
            		fflush(stdout);
            		status = 1;
            		exit(1);
				}           
        		
        		dup2(filedescriptor0, STDIN_FILENO); // set file descriptor for stdin
				close(filedescriptor0);	

   		    }

			else if(in){
              		

        		if ((filedescriptor0 = open(input, O_RDONLY, 0)) < 0) { // Error checking for creating file descriptor
            
            		printf("cannot open %s for input\n", input);
            		fflush(stdout);
            		status = 1;
            		exit(1);
				}           
        		
        		dup2(filedescriptor0, STDIN_FILENO); 
				close(filedescriptor0);

    		}	

   			
   			execvp(*args, args); // using execvp as advised in lecture

			printf("badfile: no such file or directory.\n");
			fflush(stdout);

			status = 1; // tracking status throughout program
			printf("the status after badfile is: %d\n", status);
			fflush(stdout);
			exit(1);
				
		}

		else{ // parent function

			int childstatus;

			if (!backgroundstatus){

				waitpid(spawnPid, &childstatus, 0); // wait for child processes

				if(WIFEXITED(childstatus)) {

					status = WEXITSTATUS(childstatus); // set status to exit status 
			
				}
				else if(WIFSIGNALED(childstatus)){\

					signum = WTERMSIG(childstatus); // track signal termnitation status

				}
			
			}

			else{
                
				pidArray[pidcount++] = spawnPid;
				printf("background pid is: %d\n", spawnPid);
				fflush(stdout);
				waitpid(spawnPid, &status, WNOHANG);
			}
		}					    		
}

/**************************************

FUNCTION: void changeDirectory

Takes in char**args and int k as input.

This function handles the "cd" command 
with its various arguments. The cd command
will support both relative and absolute paths.

**************************************/

void changeDirectory(char** args, int k){
		
		int rc; // using this for easier error checking
			
		if (k > 1) { 
			char cwd[1024];		

			if(args[1][0] != '/'){ 
				getcwd(cwd, 1024);
				strcat(cwd, "/");
				strcat(cwd, args[1]);
				
				rc = chdir(cwd);
				if (rc !=0){ // error checking for valid directory
					printf("cannot change to directory %s\n", args[1]);
					fflush(stdout);
				}
			}
			else{
				
				rc = chdir(args[1]); 
				if (rc !=0){
					printf("cannot change to directory %s\n", args[1]);
					fflush(stdout);
				}
			}
		}
		else{
			rc = chdir(getenv("HOME"));			
			if (rc !=0){
				printf("cannot change to home directory!\n");
				fflush(stdout);
			}
		}

}

/**************************************

FUNCTION: void exitCleaner

This function loops through our pidArray
and deletes all processes when the "exit"
command is called from the command prompt.

**************************************/

void exitCleaner(){

	int i;

	// loops through pidArray and cleans up pids
	for(i=0; i<pidcount; i++){
		
		if (pidArray[i] != INT_MAX){
			kill(pidArray[i], SIGKILL);
		}
	}

	exit(0);

}

/**************************************

FUNCTION: displayStatus

Simple function to display our status if 
"status" was requested at the command prompt.

**************************************/

void displayStatus(){

	if(signum != 0){
		printf("terminated by signal %d\n", signum);
	}
	else {
		printf("exit value %d\n", status);	
	}
	
}

/**************************************

FUNCTION: void executeCMD

Takes in into singo as input.

This function catches the control-c signal. It also 
prints to the screen the termination signal after
user hits control-C.

**************************************/

void catchSIGINT(int signo){

	char str[1000];

	sprintf(str, "\nterminated by signal %d\n", signo); 
	fflush(stdout);
	write(1, str, strlen(str)); // lecture says to use write function over printf due to reentrancy

}

/**************************************

FUNCTION: void executeCMD

Takes in int signo as input.

This functions catches the control-Z signal. Uses
the ignoreBG bool to keep track of foreground/background
states. 

**************************************/

void catchSIGTSTP(int signo){
	
	// this function is formatted according to lecture 3.3
	// also using the write function over printf to handle reentrancy.

	if ( ignoreBG == false){
		char* message = "\nEntering foreground-only mode (& is now ignored)\n"; // this instance of "control-Z" enters foreground-only mode
		write(STDOUT_FILENO, message, 52);
		fflush(stdout);
		ignoreBG = true;
	}
	else{
		char* message = ("\nExiting foreground-only mode\n"); // this instance of "control-Z" exits foreground-only mode
		write(STDOUT_FILENO, message, 31);
		fflush(stdout);
        ignoreBG = false;
	} 
}

/**************************************

FUNCTION: void executeCMD

This function is run before every command prompt.
Checks for the child status as prints out background pid
and exit values.

**************************************/

void checkchildstatus(void){

	int childstatus;
	int i;
	int rc; // using rc variable again for easier error checking 

		for(i=0; i < pidcount; i++){
		
			rc = waitpid(pidArray[i], &childstatus, WNOHANG);

			if(rc > 0){
				rc = WIFEXITED(childstatus); // normal exit will display background pid and exit value	
				if(rc > 0){
					printf("background pid %d is done: exit value %d\n", pidArray[i], WEXITSTATUS(childstatus));
					fflush(stdout); 
					status = WEXITSTATUS(childstatus);

				}

				else { 
					rc = WIFSIGNALED(childstatus); // if exited by signal dispaly background pid as well as signal
					if(rc > 0){
						printf("background pid %d is done: terminated by signal %d\n", pidArray[i], WTERMSIG(childstatus)); 				
						fflush(stdout);
						status = WEXITSTATUS(childstatus);
						signum = WTERMSIG(childstatus);
					}
				}				
			}
		}		
}

/**************************************

Our MAIN function. 

**************************************/
int main(){

	// Variables
	char input[2048];
	char input1[2048];
	char* token;
	int argnum;
	int running = 1;
	int i = 0;
	char* command;
	char *args[512];	

	struct sigaction cc = {0}; // STRUCT for "control-C" SIGINT
	struct sigaction cz = {0}; // STRUCT FOR "control-Z" SIGTSTP

    // initialize our signals according to lecture 3.3

	cc.sa_handler = catchSIGINT;
	sigfillset(&cc.sa_mask);
	cc.sa_flags = 0;
	sigaction(SIGINT, &cc, NULL);

	cz.sa_handler = catchSIGTSTP;
	sigfillset(&cz.sa_mask);
	cz.sa_flags = 0;
	sigaction(SIGTSTP, &cz, NULL);


	for (i = 0; i < MAX_PID; i++) {
		pidArray[i] = INT_MAX;
	}

	do {
		checkchildstatus();
		printf(": "); // START of cmd prompt
		fflush(stdout); // always flush after printing
		fgets(input, 2048, stdin); // Get user input
		
		// replace our new lanes with the NULL byte

		if (input[strlen(input) -1] == '\n') {
			input[strlen(input) -1] = '\0';
		}

		// handle black space input 

		if (input[0] == '\n' || input[0] == '\0') {
			continue;
		}

		// handles "$$" (only works on "$$" if it is at the very end)

		char *p;
		pid_t pid;

		p = strstr(input, "$$");
	
		if (p != NULL) {
			pid = getpid();
			sprintf(p, "%d", pid);
		}
		
		// handles background processes by changing global variables	

		if (input[strlen(input) -1] == '&') {
			input[strlen(input) -1] = '\0';
			
			if (ignoreBG) {
				backgroundstatus = false;
			}
			else {			
				backgroundstatus = true;
			}		
		}
        else {
			backgroundstatus = false;
		}
 	
		strcpy(input1, input);
		
		// initialize the args array

		for (i = 0; i < 512; i++) {
			args[i] = NULL;
		}

		int argcnt = 0;

    	// Populate arguments into args and do some string parsing

		token = strtok(input, " ");
		while (token != NULL) {
			args[argcnt++] = token;
			token = strtok(NULL, " ");
		}
		token = args[0];

		// handles built in commands

   		if (strcmp(token,"status") == 0) {		
			displayStatus(); // call this function to display status
		}	
		else if (strcmp(token,"exit") == 0) {
			exitCleaner();	// call this function to exit
		}
		else if (strcmp(token,"cd") == 0) {
			changeDirectory(args, argcnt);	// call this function to handle changing directories	
		}
		else if (strncmp(token,"#",1) ==0) { // do nothing for comments
        	
		}
		else {	
			executeCMD(args, argcnt); // fork if not built in command							
		}
 
	} while(running);

	return 0;
}