/*Justin Phillips
cs344 winter2021
Assignment 5
Code adapted from mine in spring2020 and Ben Brewster lecture 4.2 Network cients*/
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
	//char buffType[1000000];
	char msg[100000];
	char fileName[256];
	//char user[32];
	//char requestType[5];
	char key2[100000];
	int keyInt, cypherInt, ptInt, i, checkdir, randNum;
	//DIR* dr1;
	//DIR* dr2;
	//struct dirent* de;
	struct sockaddr_in serverAddress, clientAddress;
	//char cwd[100];
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
	if (charsRead < 0) error("ERROR writing to socket");

	charsRead = recv(establishedConnectionFD, userName, sizeof(userName), 0); // Read the client's message from the socket plainttext
	if (charsRead < 0) error("ERROR reading from socket");
	printf("SERVER: I received this from the client: \"%s\"\n", userName);
	strcpy(user, userName);
	//printf("Name of user: %s\n", user);
	
	
	// Send a Success message back to the client
	charsRead = send(establishedConnectionFD, "I am the server, and I got your message", 39, 0); // Send success back
	if (charsRead < 0) error("ERROR writing to socket");*/
	memset(fileName, '\0', sizeof(fileName));
	int filelt  = 0;
	int checker = 0;
	int checker2 = 0;
	char filenm[] = "./enc_client";
	char filenm2[] = "enc_client";
	//printf("%s\n",filenm);
	fileNamefd = recv(establishedConnectionFD, fileName, sizeof(fileName), 0); // Read the client's message from the socket plainttext
	filelt = strlen(fileName);
	if (fileNamefd < 0) error("ERROR reading from socket");
	checker = strncmp(fileName, filenm, 5);
	checker2 = strncmp(fileName, filenm2, 5);
	//printf("%d:, d2:%d\n", checker, checker2);
	//printf("SERVER: I received this from the client: %s , fl: %d\n", fileName, filelt);
	if(checker != 0){
		printf("SERVER: Connecting to the wrong client %s\n", fileName);
		exit(2);
	}

	pid =fork();
	if(pid < 0){
		fprintf(stderr, "Forking Failed.\n");
	}


	if(pid == 0){
		sleep(2);
		// Get the message from the client and display it

		//if(strcmp(requestType, "post") == 0){
			//printf("post\n");
			//key is buffer
			memset(buffer, '\0', sizeof(buffer));
			charsRead = recv(establishedConnectionFD, buffer, sizeof(buffer), 0); // Read the client's message from the socket plainttext
			if (charsRead < 0) error("ERROR reading from socket");
			//printf("SERVER: I received this from the client: \"%s\"\n", buffer);

			// Send a Success message back to the client
			charsRead = send(establishedConnectionFD, "I am the server, and I got your message", 39, 0); // Send success back
			if (charsRead < 0) error("ERROR writing to socket");
			//close(establishedConnectionFD); // Close the existing socket which is connected to the client
			// buffer2 is text
			memset(buffer2, '\0', sizeof(buffer2));
			charsRead2 = recv(establishedConnectionFD, buffer2, sizeof(buffer2), 0); // Read the client's message from the socket key
			if (charsRead2 < 0) error("ERROR reading from socket");
			//printf("SERVER: I received this from the client: \"%s\"\n", buffer2);	
			memset(key2, '\0', sizeof(key2));
			memset(msg, '\0', sizeof(msg));
			strcpy(key2, buffer);
			strcpy(msg, buffer2);
		
			for(i = 0; i < (strlen(msg)); i++){
					int cypherInt = 0;
					if(key2[i] == ' '){
						key2[i] = '@'; // changed the space symbol to @ because the value of @ is right before A on the ascii table
					}
					if(msg[i] == ' '){
						msg[i] = '@'; // changed the space symbol to @ because the value of @ is right before A on the ascii table
					}
					//printf("key: %c msg: %c\n", key2[i], msg[i]);
					keyInt = (int)key2[i]; // convert value of char to and int 
					ptInt = (int)msg[i];
					//printf("key: %d msg: %d\n", keyInt, ptInt);
					keyInt -= 64;
					ptInt -= 64;
					//printf("2key: %d msg: %d\n", keyInt, ptInt);
					cypherInt = (keyInt + ptInt) % 27;
					//printf("cypher: %d\n", cypherInt);
					cypherInt += 64;
					bufferOut[i] = (char)cypherInt + 0;
					//printf("cypher: %c\n", bufferOut[i]);
					if (bufferOut[i] == '@'){
						bufferOut[i] = ' ';
					} 
					
				}
				//int addtoBuff= strlen(msg);	
				//bufferOut[addtoBuff +1] = '\0';
			//printf("output: %s\n", bufferOut);

			charsRead2 = send(establishedConnectionFD, bufferOut , strlen(bufferOut), 0); // Send success back
			if (charsRead2 < 0) error("ERROR writing to socket");
			close(establishedConnectionFD); // Close the existing socket which is connected to the client
			
		//}
		}
	}
	close(listenSocketFD); // Close the listening socket
	return 0; 
}