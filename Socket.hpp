#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>



class Socket
{
	private:
		int server_socket;
	public:
		Socket();
		~Socket();
		void acceptConnection();
		void handleClient(int client_socket);
		int clientErrCheck(int b, int &client_socket);

};

#endif
