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
	int socketFD, portNumber, charsWritten, charsRead, charsWritten2, charsRead2, checkdir, checkfileName;
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
	char cipher[100000];
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
	//otp post user plaintext key port
	//enc_client plaintext key port
	//requestType = argv[1];
	fflush(stdout);
		//printf("in while\n");
	//printf("type:%s\n", requestType);

	//if(strcmp(argv[1], "post") == 0){ //post encrypts text using the key
		//printf("in post\n");
		if(argc < 3){
			printf("Error not enough arg");
		}
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
		//printf("error check\n");

		/*charsWritten = send(socketFD, requestType, strlen(requestType), 0); // Write to the server
		if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
		if (charsWritten < strlen(requestType)) printf("CLIENT: WARNING: Not all data written to socket!\n");
			
		memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
		charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
		if (charsRead < 0) error("CLIENT: ERROR reading from socket");
		printf("CLIENT: I received this from the server: \"%s\"\n", buffer);*/
				

	/*	userName =(argv[2]); // set user name from an argument
		checkdir = mkdir(userName, 0777); // make a directory named after the user
			if(checkdir < 1){
				printf("Unable to create dir\n");
				//exit(1);

			charsWritten = send(socketFD, userName, strlen(userName), 0); // Write to the server
			if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
			if (charsWritten < strlen(userName)) printf("CLIENT: WARNING: Not all data written to socket!\n");
				
			memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
			charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
			if (charsRead < 0) error("CLIENT: ERROR reading from socket");
			printf("CLIENT: I received this from the server: \"%s\"\n", buffer);

		
			}*/

			memset(temp, '\0', sizeof(temp));
			//temp = argv[0];
			strcpy(temp, argv[0]);
			//printf("client temp: %s\n", temp);
			checkfileName = send(socketFD, temp, strlen(temp), 0); // Write to the server
			if (checkfileName < 0) error("CLIENT: ERROR writing to socket");
			if (checkfileName < strlen(temp)) printf("CLIENT: WARNING: Not all data written to socket!\n");

			int keySize, msgSize, i;
			//portNumber = atoi(argv[5]);
			keyFile = fopen(argv[2], "r"); //open the key file to read
			if(keyFile < 0){
				fprintf(stderr, "ERROR: cant open file\n");
				//exit(1);
			}
			memset(buffer, '\0', sizeof(buffer));
			memset(key, '\0', sizeof(key));
			//256 i think is the error
			fread(buffer, 70000, 1, keyFile); // read the file and store in a buffer
			//fread(buffer2, sizeof(key) , 1, fileMsg);
			//read(keyFile, buffer, 256);
			strcpy(key, buffer); // copy to the key variable
			close(keyFile);
			key[strcspn(key, "\n")] = '\0'; // remove the trailing terminator
			//printf("key: %s\n", key);
			//strcpy(key2, key);
			keySize = strlen(key);
			int badCHar2 = 0;
			for(i = 0; msg[i] != '\0'; i++){ // check for bad characters
				if((int)key[i] < 65 && (int)key[i] > 90 || (int)key[i] != 32){
					//fprintf(stderr, "CLIENT: invalid letters.\n");
					//exit(1);
					badCHar2 = 1;
				}
			}

			// Send message to server for key
			charsWritten = send(socketFD, key, strlen(key), 0); // Write to the server
			if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
			if (charsWritten < strlen(key)) printf("CLIENT: WARNING: Not all data written to socket!\n");
			

			// Get return message from server
			memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
			charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
			if (charsRead < 0) error("CLIENT: ERROR reading from socket");
			//printf("CLIENT: I received this from the server: \"%s\"\n", buffer);
			

			fileMsg = fopen(argv[1], "r"); // open the text file
			memset(buffer2, '\0', sizeof(buffer2));
			memset(msg, '\0', sizeof(msg));
			fread(buffer2, 70000, 1, fileMsg); // stre next file to a new buffer
			//fread(buffer2, sizeof(msg) , 1, fileMsg);
			strcpy(msg, buffer2);
			close(fileMsg);
			msg[strcspn(msg, "\n")] = '\0'; // remove the trailing terminAtor
			//printf("Msg: %s\n", msg);
						
			msgSize = strlen(msg); // get length of the msg
			//printf("keylength: %d msglength: %d\n", keySize, msgSize);
			if(keySize < msgSize){ // make sure the message is the correct length
				fprintf(stderr, "CLIENT: ERROR: key shorter then text.\n");
				exit(1);
			}
			int badCHar = 0;
			for(i = 0; i < msgSize -1 ; i++){ // validate the charactors
				if((int) msg[i] > 90 || ((int) msg[i] < 65 && (int) msg[i] != 32)){
					//fprintf(stderr, "CLIENT: invalid letters msg.\n");
					badCHar = 1;
					//exit(1);
				}
			
			}
			if(badCHar == 1 || badCHar2 == 1){
				fprintf(stderr, "CLIENT: invalid letters msg.\n");
				exit(1);
			}
			/*printf("key2: %s\n", key2);
			//printf("string key: %s\n", key);
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
			printf("output: %s\n", bufferOut);*/
			// Send message to server for msg
			charsWritten2 = send(socketFD, msg, strlen(msg), 0); // Write to the server
			if (charsWritten2 < 0) error("CLIENT: ERROR writing to socket");
			if (charsWritten2 < strlen(msg)) printf("CLIENT: WARNING: Not all data written to socket!\n");
			//printf("check\n");
			// Get return message from server
			memset(bufferOut, '\0', sizeof(bufferOut)); // Clear out the buffer again for reuse
			charsRead2 = recv(socketFD, bufferOut, sizeof(bufferOut), 0); // Read data from the socket, leaving \0 at end
			if (charsRead2 < 0) error("CLIENT: ERROR reading from socket");
			//printf("CLIENT: I received this from the server: \"%s\"\n", bufferOut);
			printf(bufferOut);			
			fflush(stdout);
			
			
		//}
	close(socketFD); // Close the socket
	return 0;
}