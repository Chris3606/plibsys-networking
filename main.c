#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plibsys.h>
#include <psocket.h>

const int MAX_MESSAGE_LENGTH = 1024;
const int SERVER_PORT = 5432;

int do_server_things()
{
	char buffer[MAX_MESSAGE_LENGTH + 1]; // Supports messages up to max length (plus null character that terminates the string, since we're sending text)

	PSocketAddress* addr;
	PSocket* sock;

	// Binding socket to local host (we are a server, the appropriate port.  Typically this will always be a localhost port, because we are going to listen to this)
	if ((addr = p_socket_address_new("127.0.0.1", SERVER_PORT)) == NULL)
		return 1;

	// Create socket
	if ((sock = p_socket_new(P_SOCKET_FAMILY_INET, P_SOCKET_TYPE_STREAM, P_SOCKET_PROTOCOL_TCP, NULL)) == NULL)
	{
		// Failed to create socket -- cleanup
		p_socket_address_free(addr);

		return 2;
	}

	// Bind to local host (server) socket
	if (!p_socket_bind(sock, addr, FALSE, NULL))
	{
		// Couldn't bind socket, cleanup
		p_socket_address_free(addr);
		p_socket_free(sock);

		return 3;
	}

	// Listen for incoming connections on localhost SERVER_PORT
	if (!p_socket_listen(sock, NULL))
	{
		// Couldn't start listening, cleanup
		p_socket_address_free(addr);
		p_socket_close(sock, NULL);

		return 4;
	}

	// Forever, try to accept incoming connections.
	while (1)
	{
		// Blocks until connection accept happens by default -- this can be changed
		PSocket* con = p_socket_accept(sock, NULL);

		if (con != NULL)
		{
			// Send "Message from server" to the client, and terminate their connection.
			strcpy(buffer, "Message from server");
			p_socket_send(con, buffer, strlen(buffer), NULL);
			
			p_socket_close(con, NULL);
		}
		else
			printf("Can't make con, tried and failed...\n");
	}

	// Cleanup
	p_socket_address_free(addr);
	p_socket_close(sock, NULL);

	return 0;
}

int do_client_things()
{
	char buffer[MAX_MESSAGE_LENGTH + 1]; // Supports messages up to max message length characters long (plus null character)

	PSocketAddress* addr;
	PSocket* sock;

	// Construct address for server.  Since the server is assumed to be on the same machine for the sake of this program, the address is loopback, but typically this would be an external address.
	if ((addr = p_socket_address_new("127.0.0.1", SERVER_PORT)) == NULL)
		return 1;

	// Create socket
	if ((sock = p_socket_new(P_SOCKET_FAMILY_INET, P_SOCKET_TYPE_STREAM, P_SOCKET_PROTOCOL_TCP, NULL)) == NULL)
	{
		// Can't construct socket, cleanup and exit.
		p_socket_address_free(addr);

		return 2; 
	}


	// Connect to server.
	if (!p_socket_connect(sock, addr, NULL))
	{
		// Couldn't connect, cleanup.
		p_socket_address_free(addr);
		p_socket_free(sock);

		return 4;
	}

	// Receive our message and print.
	pssize sizeOfRecvData = p_socket_receive(sock, buffer, sizeof(buffer) - 1, NULL);
	buffer[sizeOfRecvData] = '\0'; // Set null character 1 after message

	printf("We received %s", buffer);

	// Cleanup
	p_socket_address_free(addr);
	p_socket_close(sock, NULL);

	return 0;
}

int main()
{
	p_libsys_init();

	int inputChar;

	printf("Type s to make this a server instance, or c for client instance:");
	inputChar = getchar();

	int runResult;
	if (inputChar == 's')
		runResult = do_server_things();
	else
		runResult = do_client_things();

	p_libsys_shutdown();

	return runResult;
}
