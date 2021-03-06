#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>




void encrpytMessage(char* message, char* key, char* cyphertext){

	char randomletter[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
	

	int i;
	int total;
	char* pmessage;
	char* pkey;


	//fprintf(stdout, "length of message is: %d\n", strlen(message));
	//fprintf(stdout, "length of key is: %d\n", strlen(key));

	for(i=0; i < strlen(message); i++){

		char c1 = message[i];
		char c2 = key[i];

		int index1; 
		int index2; 
		
		pmessage = strchr(randomletter, c1);

		//printf("pMessage: %s\n", pmessage);

		index1 = pmessage - randomletter;
		
		//printf("Index 1: %d\n", index1);

		pkey = strchr(randomletter, c2);
		index2 = pkey - randomletter;
	
		//printf("Index 2: %d\n", index2);

		total = (index1 + index2) % 27;


		cyphertext[i] = randomletter[total];

		//fprintf(stdout, "i is: %d\n", i);



	}

	cyphertext[i] = '\n';
	cyphertext[i+1] = '\0';
	//fprintf(stdout, "length of cyphertext is: %d\n", strlen(cyphertext));


}







void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

int main(int argc, char *argv[])
{

char* keystring;
char* textstring;
char* encryptiontext;
int filelength;


/*
if (strlen(key) != strlen(message)){

	fprintf(stderr,"ERROR: Message and key have different lengths.");
	exit(1);
}

*/

//encrpytMessage(message, key, cyphertext);

//printf("Cypher text is: %s\n", cyphertext);






int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
socklen_t sizeOfClientInfo;
char buffer[256];
char codename;
char returncode; 
struct sockaddr_in serverAddress, clientAddress;

if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args

	// Set up the address struct for this process (the server)
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

	// Set up the socket
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) error("ERROR opening socket");

	// Enable the socket to begin listening
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("ERROR on binding");
	
	listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections

	// Accept a connection, blocking if one is not available until one connects
	sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
	
	while(1){

		establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
		if (establishedConnectionFD < 0) error("ERROR on accept");

		pid_t pid = fork();

		if(pid < 0){
			fprintf(stderr, "cannot fork child.\n");
			exit(1);
		}

		else if(pid == 0){ // Child process

				// Get the message from the client and display it
			
			charsRead = recv(establishedConnectionFD, &codename, 1, 0); // Read the client's message from the socket

			if(charsRead ==0){
				fprintf(stderr,"Did not recieve code(enc server).\n");
			}
			else{
				//fprintf(stdout, "The code is: %c\n", codename);
			}


			if(codename == 'N'){
				int checksend;
				char returncode = 'B';

				checksend = send(establishedConnectionFD, &returncode, 1, 0);

				if(checksend == -1){
					fprintf(stderr, "Error sending data.");
					exit(1);
				}
				else{
					recv(establishedConnectionFD, &filelength, sizeof(int), 0); // recieved file length

					//fprintf(stdout, "The filelength is: %d\n", filelength);
					

					textstring = malloc(sizeof(char) * filelength);
					keystring = malloc(sizeof(char) * filelength);
					encryptiontext = malloc(sizeof(char) * filelength + 1); // for new line
					
					memset(textstring, '\0', filelength);
					memset(keystring, '\0', filelength);
					memset(encryptiontext, '\0', filelength + 1);
					//recv(establishedConnectionFD, textstring, sizeof(char) * filelength, 0);
					//fprintf(stdout, "The textstring1 is: %s\n", textstring);
					//fprintf(stdout, "The length of textstring1 is: %d\n", strlen(textstring));

					int total = 0;
					int recievelength;

					while(total < filelength){
					
						recievelength = recv(establishedConnectionFD, textstring + total, filelength, 0); 
						total = total + recievelength;
			
					}

					total = 0;

					while(total < filelength){
					
						recievelength = recv(establishedConnectionFD, keystring + total, filelength, 0); 
						total = total + recievelength;
						
			
					}


					encrpytMessage(textstring, keystring, encryptiontext);


					int sendlength;
					total = 0;

					while(total < filelength){
			
						sendlength = send(establishedConnectionFD, encryptiontext + total, filelength, 0); // sending text string
						total = total + sendlength;
					

					}			


					close(establishedConnectionFD); 
					close(listenSocketFD);
					exit(0);
				}

			}	 
			else{
				//printf("encryption server return.");
				char returncode = 'B';
				send(establishedConnectionFD, &returncode, 1, 0);
				exit(2);
			}	
		}

		else {

			// In parent
		}
	}

	/*
	if (charsRead < 0) error("ERROR reading from socket");
	printf("SERVER: I received this from the client: \"%s\"\n", codename);

	// Send a Success message back to the client
	charsRead = send(establishedConnectionFD, "I am the server, and I got your message", 39, 0); // Send success back
	if (charsRead < 0) error("ERROR writing to socket");
	close(establishedConnectionFD); // Close the existing socket which is connected to the client
	close(listenSocketFD); // Close the listening socket
	*/

	return 0; 
	
}
