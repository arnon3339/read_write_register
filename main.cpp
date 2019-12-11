#include <boost/asio.hpp>
#include <iostream>
#include <vector>

#include "register_rw.hpp"

int main(int argc, char const *argv[])
{
	int	port			= 	49153;
	char host[] 		= 	"192.168.0.101";
	boost::asio::io_service ios;
	RegsTCPClient	*regstcp	=	new RegsTCPClient(ios, host, port);
	RegisterRW		*rw_regs	=	new RegisterRW(regstcp);

	if (argc > 1 && *(*(argv + 1)) == '-' && *(*(argv + 1) + 1) == 'r')
	{
		argv++;
		rw_regs->read_registers(argc, argv);
		ios.run();
	}
	else if (argc > 1 && *(*(argv + 1)) == '-' && *(*(argv + 1) + 1) == 'w')
	{
		argv++;	
		rw_regs->write_register(argc, argv);	
		ios.run();
	}
	else if (argc == 1)
	{
		rw_regs->read_registers();
		ios.run();
		return 0;
	}
	else
	{
		std::cerr << "Error: invalid mode to read or write registers!" << std::endl; 
		return -1;
	}
	return 0;
}
