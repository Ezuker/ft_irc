#include "global.hpp"
#include "Client.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{

	if (argc != 3)
	{
		std::cerr << "wrong arg" << std::endl;
		return (1);
	}
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0)
	{
		std::cerr << "Error: socket() failed" << std::endl;
		return 1;
	}
	Server	myServer(atoi(argv[1]), argv[2], server_sock);
	myServer.startServer();
	return 0;
}
