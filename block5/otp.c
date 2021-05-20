

		//printf("in while\n");
	//printf("type:%s\n", requestType);

	if(strcmp(argv[1], "post") == 0){ //post encrypts text using the key
		printf("in post\n");
		if(argc < 5){
			printf("Error not enough arg");
		}
			// Set up the server address struct
		memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
		portNumber = atoi(argv[5]); // Get the port number, convert to an integer from a string
		serverAddress.sin_family = AF_INET; // Create a network-capable socket
		serverAddress.sin_port = htons(portNumber); // Store the port number
		serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
		if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(2); }
		memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address
		printf("error check\n");
		// Set up the socket
		socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
		if (socketFD < 0) error("CLIENT: ERROR opening socket");
		
		// Connect to server
		if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
			error("CLIENT: ERROR connecting");
		//printf("error check\n");

		charsWritten = send(socketFD, requestType, strlen(requestType), 0); // Write to the server
		if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
		if (charsWritten < strlen(requestType)) printf("CLIENT: WARNING: Not all data written to socket!\n");
			
		memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
		charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
		if (charsRead < 0) error("CLIENT: ERROR reading from socket");
		printf("CLIENT: I received this from the server: \"%s\"\n", buffer);
				

		userName =(argv[2]); // set user name from an argument
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

		
			}

			int keySize, msgSize, i;
			//portNumber = atoi(argv[5]);
			keyFile = fopen(argv[4], "r"); //open the key file to read
			if(keyFile < 0){
				fprintf(stderr, "ERROR: cant open file\n");
				//exit(1);
			}
			memset(buffer, '\0', sizeof(buffer));
			memset(key, '\0', sizeof(key));
			fread(buffer, 256, 1, keyFile); // read the file and store in a buffer
			//read(keyFile, buffer, 256);
			strcpy(key, buffer); // copy to the key variable
			close(keyFile);
			key[strcspn(key, "\n")] = '\0'; // remove the trailing terminator
			//printf("key: %s\n", key);
			strcpy(key2, key);
			keySize = strlen(key);
			for(i = 0; msg[i] != '\0'; i++){ // check for bad characters
				if((int)key[i] < 65 && (int)key[i] > 90 || (int)key[i] != 32){
					fprintf(stderr, "CLIENT: invalid letters.\n");
					exit(1);
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
			printf("CLIENT: I received this from the server: \"%s\"\n", buffer);
			

			fileMsg = fopen(argv[3], "r"); // open the text file
			memset(buffer2, '\0', sizeof(buffer2));
			memset(msg, '\0', sizeof(msg));
			fread(buffer2, 256, 1, fileMsg); // stre next file to a new buffer
			strcpy(msg, buffer2);
			close(fileMsg);
			msg[strcspn(msg, "\n")] = '\0'; // remove the trailing terminAtor
			//printf("Msg: %s\n", msg);
						
			msgSize = strlen(msg); // get length of the msg
			//printf("keylength: %d msglength: %d\n", keySize, msgSize);
			if(keySize < msgSize){ // make sure the message is the correct length
				fprintf(stderr, "CLIENT: ERROR: key shorter then text.\n");
				//exit(1);
			}

			for(i = 0; i < msgSize -1 ; i++){ // validate the charactors
				if((int) msg[i] > 90 || ((int) msg[i] < 65 && (int) msg[i] != 32)){
					fprintf(stderr, "CLIENT: invalid letters msg.\n");
					//exit(1);
				}
			}
			//printf("key2: %s\n", key2);
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
			printf("output: %s\n", bufferOut);
			// Send message to server for msg
			charsWritten2 = send(socketFD, bufferOut, strlen(bufferOut), 0); // Write to the server
			if (charsWritten2 < 0) error("CLIENT: ERROR writing to socket");
			if (charsWritten2 < strlen(bufferOut)) printf("CLIENT: WARNING: Not all data written to socket!\n");
			printf("check\n");
			// Get return message from server
			memset(buffer2, '\0', sizeof(buffer2)); // Clear out the buffer again for reuse
			charsRead2 = recv(socketFD, buffer2, sizeof(buffer2) - 1, 0); // Read data from the socket, leaving \0 at end
			if (charsRead2 < 0) error("CLIENT: ERROR reading from socket");
			printf("CLIENT: I received this from the server: \"%s\"\n", buffer2);
						
			fflush(stdout);
			
			
		}else if (strcmp(argv[1], "get") == 0){ // use the key to decrypt the message
			if(argc < 4){
				printf("Error not enough arg");
			}
			printf("in get\n");

			// Set up the server address struct
			memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
			portNumber = atoi(argv[4]); // Get the port number, convert to an integer from a string
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

			charsWritten = send(socketFD, requestType, strlen(requestType), 0); // Write to the server
				if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
				if (charsWritten < strlen(requestType)) printf("CLIENT: WARNING: Not all data written to socket!\n");
			
			memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
			charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
			if (charsRead < 0) error("CLIENT: ERROR reading from socket");
			printf("CLIENT: I received this from the server: \"%s\"\n", buffer);

			userName =(argv[2]);

			charsWritten = send(socketFD, userName, strlen(userName), 0); // Write to the server
			if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
			if (charsWritten < strlen(userName)) printf("CLIENT: WARNING: Not all data written to socket!\n");
			
			memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
			charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
			if (charsRead < 0) error("CLIENT: ERROR reading from socket");
			printf("CLIENT: I received this from the server: \"%s\"\n", buffer);
				
			int keySize, msgSize, i;
			char *recentFile;
			keyFile = fopen(argv[3], "r");
			memset(buffer, '\0', sizeof(buffer));
			memset(key, '\0', sizeof(key));
			fread(buffer, 256, 1, keyFile);
			strcpy(key, buffer);
			close(keyFile);
			key[strcspn(key, "\n")] = '\0';
					
			//printf("key: %s\n", key);
			keySize = strlen(key); // get lengths of our strings to compare
			//printf("keylength: %d\n", keySize);
			
			// Get return message from server
			memset(buffer2, '\0', sizeof(buffer2)); // Clear out the buffer again for reuse
			charsRead2 = recv(socketFD, buffer2, sizeof(buffer2) - 1, 0); // Read data from the socket, leaving \0 at end
			if (charsRead2 < 0) error("CLIENT: ERROR reading from socket");
			
			printf("CLIENT: I received this from the server: \"%s\"\n", buffer2);
			strcpy(temp, buffer2);
			//printf("here:%s\n", temp);
			
			msgSize = strlen(buffer2);
			//printf("keylength: %d msglength: %d\n", keySize, msgSize);
			if(keySize < msgSize){
				fprintf(stderr, "CLIENT: ERROR: key shorter then text.\n");
				exit(1);
			}

			for(i = 0; buffer2[i] != '\0'; i++){ // check the variables are correct
				if((int) buffer2[i] > 90 || ((int) buffer2[i] < 65 && (int) buffer2[i] != 32)){
					fprintf(stderr, "CLIENT: invalid letters msg.\n");
					//exit(1);
				}
			}


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

			printf("%s\n", bufferOut);
			fflush(stdout);
		}
	
	close(socketFD); // Close the socket
	return 0;
}