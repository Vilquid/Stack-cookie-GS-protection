#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stdafx.h"
#include "windows.h"
#include "winsock.h"

// load windows socket
#pragma comment(lib, "wsock32.lib")

// define return message
#define SS_ERROR 1
#define SS_OK 0



void pr(char *string)
{
	char buffer[500] = " ";

	strcpy(buffer, string);

	printf("Copie du buffer.\n");
}

void sError(char *string)
{
	printf("Error of %s\n", string);

	WSACleanup();
}

int main(int argc, char const *argv[])
{
	WORD sock_version;
	WSADATA wsaData;

	int rValue;
	
	char Message[5000] =  " ";
	char buffer[2000] = " ";
	
	u_short local_port;
	local_port = 200;

	// wsock32 initialized for usage
	sock_version = MAKEWORLD(1, 1);
	WSAStartup(sock_version, &wsaData);

	// create sever socket
	socket server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (server_socket == INVALID_SOCKET)
	{
		sError("Failed socket()");

		return SS_ERROR;
	}

	SOCKADDR_IN sin;
	sin.sin_family = PF_INET;
	sin.si_port = htons(local_port);

	sin.sin_addr.s_addr = INADDR_ANY;

	// bind the socket
	rValue = bind(server_socket, (LPSOCKADDR)&sin, sizeof(sin));

	if (rValue == SOCKET_ERROR)
	{
		sError("Failed bind()");
		WSACleanup();

		return SS_ERROR;
	}

	// get socket to listen
	rValue = listen(server_socket, 10);

	if (rValue == SOCKET_ERROR)
	{
		sError("Failed listen()");
		WSACleanup();

		return SS_ERROR;
	}

	// wait for a client to connect
	SOCKET client_socket;
	client_socket = accept(server_socket, NULL, NULL);

	if (client_socket == INVALID_SOCKET)
	{
		sError("Failed accept()");
		WSACleanup();

		return SS_ERROR;
	}

	int bytesRecv = SOCKET_ERROR;

	while (bytesRecv == SOCKET_ERROR)
	{
		// receive the data that is being sent by the client max limit to 5000 bytes
		bytesRecv = recv(client_socket, Message, 5000, 0);
	}

	if (bytesRecv == 0 || bytesRecv == WSAECONNRESET)
	{
		printf("\nConnetion closed.\n");
		exit(0);
	}

	// pass the data received to the function pr
	pr(Message);

	// close client socket
	closesocket(client_socket);

	// close sever socket
	closesocket(server_socket);

	WSACleanup();

	return SS_OK;
}