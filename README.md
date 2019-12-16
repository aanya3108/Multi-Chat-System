# Multi-Chat-System


The IPC multichat system.

It takes two program codes. The first one constitutes of the server code and the second one is client code. The server code opens a socket file using the AF_LINUX protocol. The server code runs on a seperate terminal and creates a thread for each client as soon as it connects using the client code. The client code for each client runs seperately on a terminal.
	
	Compilation:

		Run the Makefile using make command. Run the server code on a terminal and the client code, each on a different terminal using ./server and ./client respectively.

	Testing:

		The user should give input as the IdOfTheUser<space>Message.
		The id of the user, to which th message is to be sent, is displayed on the server terminal as soon as the user is connected to the seocket.
		The idOfTheUser is -1 for broadcasting of message to every client, except himself.
		The output displays error, if user does not exist.

	Output:

		On the client side: The message is recieved as user:<sendingUserID> message:<theMessageUserID>
		On the server side: <messageUserId> <message>


	Error Values:
			
		1. Checks if the client exists or not.
			i. It is intimated on both sides - server and client.
		
		2. The client fileDescriptor is able to recieve or not, or if the client is offline.

		3. Code checks if the file exists on which the socket is opened for the client side code, or not.

		4. When the server is closed, every client exits.

		5. Checks for Null messages.

		6. Checks for Invalid users.

		7. File is delelted before the local creates a file for the socket (if the file is present before) .

	Implementation :
		Server :- 
			- using create(), Create TCP socket.
			- using bind(), Bind the socket to server address.
			- using listen(), put the server socket in a passive mode, where it waits for the client to approach the server to make a connection
			- using accept(), At this point, connection is established between client and server, and they are ready to transfer data.	
			- Go back to Step 3.
		Client :-
			- create TCP socket
			- connect newly created client to server

		### closing the file descriptors and sockets after use have been given extra attention ###
