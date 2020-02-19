#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/stat.h>

void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[256];
	FILE* textfd = NULL;
	FILE* keyfd = NULL;
	FILE* outputfd = NULL;

	size_t filelength; 

    struct stat textstat, keystat;
    char* keystring;
    char* textstring;
    char* encryptiontext;
    int i;


	if (argc < 4) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); } // Check usage & args


	textfd = fopen(argv[1] , "r");

	if (textfd == NULL){  	
        fprintf(stderr, "cannot open %s\n", argv[1]); // error checking for file open
        exit(1);
    }

    if (stat(argv[1], &textstat) < 0) {
    	fprintf(stderr,"cannot get size of textfile");
    	exit(1);
    }

	keyfd = fopen(argv[2] , "r");

    if (keyfd == NULL){  	
        fprintf(stderr, "cannot open %s\n", argv[2]); // error checking for file open
        exit(1);
    }
      
    if (stat(argv[2], &keystat) < 0) {
    	fprintf(stderr,"cannot get size of keyfile");
    	exit(1);
    }

    if(keystat.st_size < textstat.st_size){
    	fprintf(stderr, "error. key is too small.");
    	exit(1);
    }

    if(argc > 5){
    	outputfd = fopen(argv[5] , "w");

		if (outputfd == NULL){  	
        	fprintf(stderr, "cannot open outputfile: %s\n", argv[5]); // error checking for file open
        	exit(1);
    	}
	}

    filelength = textstat.st_size;

    //fprintf(stdout, "The file length is %d\n", filelength);

    keystring = malloc(sizeof(char) * filelength);
	textstring = malloc(sizeof(char) * filelength);

    memset(keystring, '\0', filelength);
    memset(textstring, '\0', filelength);

    if(fread(textstring, 1, filelength, textfd) != filelength) {
    	fprintf(stderr, "unable to read text file\n");
    	exit(1);

    } 

	else{

		textstring[filelength-1] = '\0';

		for(i=0; i<filelength-1; i++){
    		char c1 = textstring[i];

    		if(!(c1 == ' ' || (c1 >= 'A' && c1 <= 'Z'))) {
    			fprintf(stderr,"invalid characters in file\n");
    			exit(1);
    		}
    	}

    }

    if(fread(keystring, 1, filelength, keyfd) != filelength) {
    	fprintf(stderr, "unable to read key file\n");
    	exit(1);

    } 

    else{

    	keystring[filelength] = '\0';

    	for(i=0; i<filelength-1; i++){
    		char c1 = keystring[i];

    		if(!(c1 == ' ' || (c1 >= 'A' && c1 <= 'Z'))) {
    			fprintf(stderr,"invalid characters in file\n");
    			exit(1);
    		}
    	}

    }

   	//fprintf(stdout, "The text string is: %s\n", textstring);
   //	fprintf(stdout, "The key string is: %s\n", keystring);

   	

	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[3]); // Get the port number, convert to an integer from a string
	
	//fprintf(stdout, "Port number is: %d", portNumber);

	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) error("CLIENT: ERROR opening socket");
	
	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){ // Connect socket to address
		fprintf(stderr, "CLIENT: ERROR connecting");
		exit(1);
	}

	// Making sure we are connecting to the correct server
	char codename = 'N';
	char returncode = ' ';

	send(socketFD, &codename, 1, 0);
	
	recv(socketFD, &returncode, 1, 0);

	//fprintf(stdout, "The return code is: %c\n", returncode);
		
	if(returncode != 'B'){
		//close(socketFD);
		fprintf(stderr, "error: otp_enc cannot use otp_dec_d.\n");
		exit(2);
	}

	else{

		send(socketFD, &filelength, sizeof(int), 0); // sending length of file

		int total = 0;
		int sendlength;


		while(total < filelength){
			
			sendlength = send(socketFD, textstring + total, filelength, 0); 
			total = total + sendlength;
		

		}
		
		total = 0;

		while(total < filelength){
			
			sendlength = send(socketFD, keystring + total, filelength, 0); 
			total = total + sendlength;
		

		}
		


		encryptiontext = malloc(filelength + 1);
		memset(encryptiontext, '\0', filelength + 1);

		total = 0;
		int recievelength;

		while(total < filelength){
			
			recievelength = recv(socketFD, encryptiontext + total, filelength, 0); // sending text string
			total = total + recievelength;
		

		}
		

		if(outputfd != NULL){
			fprintf(outputfd, encryptiontext, filelength);	

		}

		else{
			fprintf(stdout, encryptiontext, filelength);
		}

		close(outputfd);
		close(keyfd);
		close(textfd);
		
		free(keystring);
		free(textstring);
		free(encryptiontext);

	} 



	/* Get input message from user
	printf("CLIENT: Enter text to send to the server, and then hit enter: ");
	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
	fgets(buffer, sizeof(buffer) - 1, stdin); // Get input from the user, trunc to buffer - 1 chars, leaving \0
	buffer[strcspn(buffer, "\n")] = '\0'; // Remove the trailing \n that fgets adds

	// Send message to server
	charsWritten = send(socketFD, buffer, strlen(buffer), 0); // Write to the server
	if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
	if (charsWritten < strlen(buffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");

	// Get return message from server
	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
	charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
	if (charsRead < 0) error("CLIENT: ERROR reading from socket");
	printf("CLIENT: I received this from the server: \"%s\"\n", buffer);
 
	close(socketFD); // Close the socket

	*/

	return 0;

}
