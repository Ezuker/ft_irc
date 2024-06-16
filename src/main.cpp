#include "global.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <exception>

void	checkArgv(const char *port, const char *pw)
{
	std::string s_port = port;
	for (size_t i = 0; i < s_port.size(); i++)
		if (!std::isdigit(s_port[i]))
			throw std::logic_error("Need alpha character in port");
	if (strtold(port, 0x0) > 65535)
		throw std::logic_error("Port can't be superior to 65535");
	std::string s_pw = pw;
	if (s_pw.size() > 256)
		throw std::logic_error("Password can't be longer than 256");
}

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
	try
	{
		checkArgv(argv[1], argv[2]);
		Server	myServer(atoi(argv[1]), argv[2], server_sock);
		myServer.startServer();
	}
	catch(const std::logic_error& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}
