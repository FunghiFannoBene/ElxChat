#include "Socket.hpp"

Socket::Socket()
{
	struct sockaddr_in	server_addr;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		std::cerr << "Error: Socket creation" << std::endl;
		return ;
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(4242);
	if (bind(server_socket, (struct sockaddr *)&server_addr,
			sizeof(server_addr)) < 0)
	{
		std::cerr << "Error: binding socket." << std::endl;
		close(server_socket);
		return ;
	}
	if (listen(server_socket, 5) < 0)
	{
		std::cerr << "Error: listen failed." << std::endl;
		close(server_socket);
		return ;
	}
}

Socket::~Socket()
{
	close(server_socket);
}

void Socket::acceptConnection()
{
	struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (true)
	{
        int new_client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (new_client_socket < 0)
		{
            std::cerr << "Errore nell'accettare la connessione!" << std::endl;
            continue;
        }
		std::thread(&Socket::handleClient, this, new_client_socket).detach();
	}
}
void Socket::handleClient(int client_socket)
{
	std::string message;
	char	buffer[1024];
	int		rec_bytes;

	while (true)
	{
		rec_bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
		if(clientErrCheck(rec_bytes, client_socket) == -1)
			return;
		buffer[rec_bytes] = '\0';
		message.append(buffer, rec_bytes);
		if (message.find('\n') != std::string::npos) {
                std::cout << "Messaggio ricevuto dal client: " << message << std::endl;

                const char *response = "Messaggio ricevuto!";
                int bytes_sent = send(client_socket, response, strlen(response), 0);
                if (bytes_sent < 0)
				{
                    std::cerr << "Errore nell'invio dei dati al client!" << std::endl;
                    close(client_socket);
                    client_socket = -1;
                    return;
                }

                // Resetta il messaggio dopo averlo processato
                message.clear();
        }
	}
}

int Socket::clientErrCheck(int b, int &client_socket)
{
	if(b > 0)
		return 0;
	if(b < 0)
		std::cerr << "Errore nella ricezione dei dati dal client!" << std::endl;
	else if(b == 0)
		std::cout << "Il client ha chiuso la connessione.";
    close(client_socket);
	client_socket = -1;
    return(-1);
}

int main()
{
    Socket server;
    server.acceptConnection();
    return 0;
}