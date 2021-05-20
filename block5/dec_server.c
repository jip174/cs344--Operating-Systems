/*Justin Phillips
cs344 winter2021
Assignment 5
Code adapted from mine in spring2020 and Ben Brewster lecture 4.2 Network cients */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <dirent.h>
#include <time.h>

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	int listenSocketFD, establishedConnectionFD, portNumber, charsRead, charsRead2, pid, fileNamefd;
	socklen_t sizeOfClientInfo;
	char buffer[100000];
	char buffer2[100000];
	char bufferOut[100000];
	char buffType[1000000];
	char fileName[256];
	//char user[32];
	//char requestType[5];
	char key[100000];
	int keyInt, cypherInt, ptInt, i;
	//DIR* dr1;
	//DIR* dr2;
	//struct dirent* de;
	struct sockaddr_in serverAddress, clientAddress;
	char cwd[100];
	//time_t oldest = 0;
	//time_t current = 0;
	srand(time(0));


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

	while(1){

	// Accept a connection, blocking if one is not available until one connects
	sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
	establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
	if (establishedConnectionFD < 0) error("ERROR on accept");

	/*charsRead = recv(establishedConnectionFD, buffType, sizeof(buffType), 0); // Read the client's message from the socket plainttext
	if (charsRead < 0) error("ERROR reading from socket");
	printf("SERVER: I received this from the client: \"%s\"\n", buffType);
	strcpy(requestType, buffType);
	//printf("type in otpd: %s\n", requestType);
	
	
	// Send a Success message back to the client
	charsRead = send(establishedConnectionFD, "I am the server, and I got your message", 39, 0); // Send success back
	if (charsRead < 0) error("ERROR writing to socket");*/

	//check that we are connected to right client
	int checker = 0;
	char filenm[] = "./dec_client";
	fileNamefd = recv(establishedConnectionFD, fileName, sizeof(fileName), 0); // Read the client's message from the socket plainttext
	if (fileNamefd < 0) error("ERROR reading from socket");
	checker = strncmp(fileName, filenm, 5);
	//printf("SERVER: I received this from the client: \"%s\"\n", fileName);
	if(checker != 0 ){
		printf("SERVER: Connecting to the wrong client %s\n", fileName);
		exit(2);
	}
	//strcpy(user, userName);
	//printf("Name of user: %s\n", user); 
	
		

	pid =fork();
	if(pid < 0){
		fprintf(stderr, "Forking Failed.\n");
	}


	if(pid == 0){
		sleep(2);
		// Get the message from the client and display it

		//if(strcmp(requestType, "post") == 0){
			//printf("post\n");

			//key
			memset(buffer, '\0', sizeof(buffer));
			charsRead = recv(establishedConnectionFD, buffer, sizeof(buffer), 0); // Read the client's message from the socket plainttext
			if (charsRead < 0) error("ERROR reading from socket");
			//printf("SERVER: I received this from the client: \"%s\"\n", buffer);

			// Send a Success message back to the client
			charsRead = send(establishedConnectionFD, "I am the server, and I got your message", 39, 0); // Send success back
			if (charsRead < 0) error("ERROR writing to socket");
			//close(establishedConnectionFD); // Close the existing socket which is connected to the client
			//cypher
			memset(buffer2, '\0', sizeof(buffer2));
			charsRead2 = recv(establishedConnectionFD, buffer2, sizeof(buffer2), 0); // Read the client's message from the socket key
			if (charsRead2 < 0) error("ERROR reading from socket");
			//printf("SERVER: I received this from the client: \"%s\"\n", buffer2);	
			
			//strcpy()
			strcpy(key, buffer);

			for(i = 0; i < (strlen(buffer2)); i++){
					int cypherInt = 0;
					if(key[i] == ' '){
						key[i] = '@'; // changed the space symbol to @ because the value of @ is right before A on the ascii table
					}
					if(buffer2[i] == ' '){
						buffer2[i] = '@'; // changed the space symbol to @ because the value of @ is right before A on the ascii table
					}
					//printf("key: %c msg: %c\n", key[i], buffer2[i]);
					keyInt = (int)key[i]; // convert value of char to and int 
					ptInt = (int)buffer2[i];
					//printf("key: %d msg: %d\n", keyInt, ptInt);
					keyInt -= 64;
					ptInt -= 64;
					cypherInt = ( ptInt- keyInt ) %27;
						if(cypherInt < 0){
							cypherInt += 27;
						}
					cypherInt += 64;
					//printf("cypher: %d\n", cypherInt);
					bufferOut[i] = (char)cypherInt + 0;
					//printf("after: %c: %d\n", bufferOut[i], bufferOut[i]);
					if (bufferOut[i] == '@'){
						bufferOut[i] = ' ';
					} 
				}
			//int addtoBuff= strlen(bufferOut);	
			//bufferOut[addtoBuff +1] = '\n';


			charsRead2 = send(establishedConnectionFD, bufferOut , strlen(bufferOut), 0); // Send success back
			if (charsRead2 < 0) error("ERROR writing to socket");
			close(establishedConnectionFD); // Close the existing socket which is connected to the client
		
		//}
		}
	}
	close(listenSocketFD); // Close the listening socket
	return 0; 
}