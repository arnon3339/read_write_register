#include <boost/asio.hpp>
#include <iostream>
#include <vector>

#include "wp3read_module.hpp"
#include "display.hpp"

int main(int argc, char const *argv[])
{
	std::string info 	= 	"global register module";
	int	i				=	0;
	int mdmode			=	-1;
	int rgmode			=	-1;
	int	port			= 	49153;
	char host[] 		= 	"192.168.0.100";
	char name[] 		= 	"TCP connection";
	uint8_t	*request	=	0;
	int		reqlength	=	0;
	Wp3ReadModule	*wp3read;
	RegisterModule		*regmd;
	boost::asio::io_service ios;
	RegsTCPClient	regstcp(ios, name, host, port);
	pru_register *pruregs;
	int	numregs			=	0;
	if (argc == 2 && *(*(argv + 1) + 1) != 'm')
		if (**(argv + 1) == '-')
			if (*(*(argv + 1) + 1) == 'a')
			{
				wp3read		=	new	Wp3ReadModule(&regstcp);
				display_mdreq(&regstcp, "");
				ios.run();
				for (i = 0; i < 3; i++)
					display_readval(wp3read->_mdvec->at(i));
				std::cout << "Successfully reading all modules." << std::endl;
				return 0;
			}
			else
			{
				std::cout << "error: Invalid option, please use <-a> to read all modules\n"
							<< ", or <-m> <module's name>  to read specific module\n"
							<< ", or <-m> <module's name> <-r> <register's name>" 
							<< " to read specific register from a module."
							<< std::endl;
				return 0;
			}
		else
		{
				std::cout << "error: Invalid prefix option, please use <-a> to read all modules\n"
							<< ", or <-m> <module's name>  to read specific module\n"
							<< ", or <-m> <module's name> <-r> <register's name>" 
							<< " to read specific register from a module."
							<< std::endl;
			return 0;
		}
	else if (argc > 1)
		if((mdmode = select_md(argc, argv)) != -1)
		{
			if (mdmode != MULTIMD)
				regmd	=	new RegisterModule(mdmode);
			if (argc == 3)
			{
				regstcp.set_module(regmd);
				info	=	modules[mdmode] + " register module";
			}
			else if (mdmode == MULTIMD)
			{
				int n = 0;
				std::vector<RegisterModule*> mdvec;
				for (i = 0; i < 5; i++)
					if (md_state[i])
						mdvec.push_back(new RegisterModule(i));

				regstcp.start_all(&mdvec, 'c');
				for (i = 0; i < mdvec.size(); i++)
				{
					std::cout << mdvec[i]->get_name() << " module: \n";
					display_readval(mdvec[i]);
					std::cout << std::endl;
				}
				std::cout << "Successfully reading modules." << std::endl;
				return 0;
				
			}
			else if ((rgmode = select_rg(argc, argv, regmd)) != -1)
			{
				if (rgmode == MDMULRG)
				{
					numregs	=	regmd->get_numofregs();
					std::vector<pru_register*> rgvec;
					for(i = 0; i < numregs; i++)
						if (mdrg_state[i])
							rgvec.push_back(regmd->get_regs_by_index(i));

					regstcp.start_all(&rgvec);
					display_mulreqs(&regstcp, regmd, &rgvec);

					for (i = 0; i < rgvec.size(); i++)
						display_readval(rgvec[i]);

					return 0;
				}
				else
				{
					pruregs		=	regmd->get_regs_by_index(rgmode);
					regstcp.set_reg(pruregs);
					info		=	pruregs->name + " register from " + 
					modules[mdmode] + " module";	
					
				}
				
			}
			else
				return 0;
		}
		else
			return 0;
	else if (argc > 5)
	{
		std::cout << "error: Too many aguments for reading pru." << std::endl;
		return 0;
	}
	else
	{
		regmd	=	new RegisterModule();
		regstcp.set_module(regmd);
	}

	regstcp.start();
	ios.run();

	if (argc == 1|| argc == 3)
	{
		display_mdreq(&regstcp, regmd->get_name());
		display_readval(regmd);
	}
	else
	{
		display_rgreq(&regstcp, pruregs->name);
		display_readval(pruregs);
	}

	std::cout <<"\nSuccessfully read " << info << "." << std::endl;
	return 0;
}
