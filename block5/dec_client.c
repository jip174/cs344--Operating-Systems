/*Justin Phillips
cs344 winter2021
Assignment 5
Code adapted from mine in spring2020 and Ben Brewster lecture 4.2 Network cients*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <dirent.h>
#include <time.h>

void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues


int main(int argc, char *argv[])
{
	//printf("Starting OTP\n");
	int socketFD, portNumber, charsWritten, charsRead, charsWritten2, charsRead2, checkfileName;
	//int stayInWhile = 1;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[100000];
	char buffer2[100000];
	char bufferOut[100000];
	FILE *fileMsg;
	FILE *keyFile;
	char key[100000];
	char msg[100000];
	char decipher[100000];
	char key2[100000];
	char temp[100000];
	//char *requestType;
	//char *userName;
	//int randNum;
	//int keyInt; 
	//int ptInt;
	srand(time(0));
	//char *fileName;
	//time_t oldestTime = 0;

	if (argc < 2) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); } // Check usage & args
	
	//printf("argv1: %s, argv2: %s argv3: %s, argv4: %s\n", argv[1], argv[2], argv[3], argv[4]);
	
	//requestType = argv[1];
	fflush(stdout);
//if (strcmp(argv[1], "get") == 0){ // use the key to decrypt the message
			if(argc < 3){
				printf("Error not enough arg");
			}
			//printf("in get\n");

			// Set up the server address struct
			memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
			portNumber = atoi(argv[3]); // Get the port number, convert to an integer from a string
			serverAddress.sin_family = AF_INET; // Create a network-capable socket
			serverAddress.sin_port = htons(portNumber); // Store the port number
			serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
			if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(2); }
			memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address
			//printf("error check\n");
			// Set up the socket
			socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
			if (socketFD < 0) error("CLIENT: ERROR opening socket");
			
			// Connect to server
			if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
				error("CLIENT: ERROR connecting");
	
//dec_client ciphertext1 mykey 57172 
			//send the filename over to make sure we are connecting to the right server
			memset(temp, '\0', sizeof(temp));
			//temp = argv[0];
			strcpy(temp, argv[0]);
			//printf("arg: %s, temp: %s\n", argv[0], temp);
			checkfileName = send(socketFD, temp, strlen(temp), 0); // Write to the server
			if (checkfileName < 0) error("CLIENT: ERROR writing to socket");
			if (checkfileName < strlen(temp)) printf("CLIENT: WARNING: Not all data written to socket!\n");

			int keySize, msgSize, i;
			char *recentFile;
			keyFile = fopen(argv[2], "r"); //key
			if(keyFile < 0){
				fprintf(stderr, "ERROR: cant open file\n");
				//exit(1);
			}

			memset(buffer, '\0', sizeof(buffer));
			memset(key, '\0', sizeof(key));
			fread(buffer, 70000, 1, keyFile);
			//fread(buffer2, sizeof(key) +1, 1, fileMsg);
			strcpy(key, buffer);
			close(keyFile);
			//key[strcspn(key, "\n")] = '\0';
					
			//printf("key: %s\n", key);
			keySize = strlen(key); // get lengths of our strings to compare
			//printf("keylength: %d\n", keySize);
			// send the key to the server
			charsWritten = send(socketFD, key, strlen(key), 0); // Write to the server
			if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
			if (charsWritten < strlen(key)) printf("CLIENT: WARNING: Not all data written to socket!\n");
			

			// Get return message from server
			memset(buffer2, '\0', sizeof(buffer2)); // Clear out the buffer again for reuse
			charsRead = recv(socketFD, buffer2, sizeof(buffer2) , 0); // Read data from the socket, leaving \0 at end
			if (charsRead < 0) error("CLIENT: ERROR reading from socket");
			
			//printf("CLIENT: I received this from the server: \"%s\"\n", buffer2);
			//strcpy(temp, buffer2);
			//printf("here:%s\n", temp);
			
			fileMsg = fopen(argv[1], "r"); // open the cyphertext file
			memset(buffer2, '\0', sizeof(buffer2));
			memset(msg, '\0', sizeof(msg));
			fread(buffer2, 70000, 1, fileMsg); // search next file to a new buffer
			//fread(buffer2, sizeof(msg) +1, 1, fileMsg);
			strcpy(msg, buffer2);
			close(fileMsg);
			//msg[strcspn(msg, "\n")] = '\0'; // remove the trailing terminAtor
			//printf("Msg: %s\n", msg);
						
			msgSize = strlen(msg); // get length of the msg
			//printf("keylength: %d msglength: %d\n", keySize, msgSize);
			if(keySize < msgSize){ // make sure the message is the correct length
				fprintf(stderr, "CLIENT: ERROR: key shorter then text.\n");
				exit(1);
			}
			int badChar = 0;
			int badChar2 = 0;
			for(i = 0; i < msgSize -1 ; i++){ // validate the charactors
				if((int) msg[i] > 90 || ((int) msg[i] < 65 && (int) msg[i] != 32)){
					//fprintf(stderr, "CLIENT: invalid letters msg.\n");
					//exit(1);
					badChar = 1;
				}
			}


			msgSize = strlen(buffer2);
			//printf("keylength: %d msglength: %d\n", keySize, msgSize);
			if(keySize < msgSize){
				fprintf(stderr, "CLIENT: ERROR: key shorter then text.\n");
				exit(1);
			}

			for(i = 0; buffer2[i] != '\0'; i++){ // check the variables are correct
				if((int) buffer2[i] > 90 || ((int) buffer2[i] < 65 && (int) buffer2[i] != 32)){
					//fprintf(stderr, "CLIENT: invalid letters msg.\n");
					//exit(1);
					badChar2 = 1;
				}
			}
			if (badChar == 1 || badChar2 == 1){
				fprintf(stderr, "CLIENT: invalid letters msg.\n");
				exit(1);
			}

/*
			//printf("key: %s\n", key);
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
					cypherInt = (ptInt - keyInt) %27;
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

			printf("%s\n", bufferOut);*/
			charsWritten2 = send(socketFD, msg, strlen(msg), 0); // Write to the server
			if (charsWritten2 < 0) error("CLIENT: ERROR writing to socket");
			if (charsWritten2 < strlen(msg)) printf("CLIENT: WARNING: Not all data written to socket!\n");
			//printf("check\n");
			// Get return message from server
			memset(buffer2, '\0', sizeof(buffer2) + 1); // Clear out the buffer again for reuse
			charsRead2 = recv(socketFD, buffer2, sizeof(buffer2), 0); // Read data from the socket, leaving \0 at end
			if (charsRead2 < 0) error("CLIENT: ERROR reading from socket");
			//printf("CLIENT: I received this from the server: \"%s\"\n", buffer2);
			int bufflength;
			bufflength = strlen(buffer2) + 2;
			memset(decipher, '\0', sizeof(buffer2) + 1);
			//strcpy(decipher, buffer2);
			//strcat(buffer2, '\n');
			snprintf(decipher, bufflength ,"%s\n", buffer2, '\n');
			//buffer2[bufflength] = '\n';
			printf(decipher);		
			fflush(stdout);
		//}
	
	close(socketFD); // Close the socket
	return 0;
}