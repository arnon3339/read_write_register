#include "display_mdrg.hpp"
#include <iostream>
#include <iomanip>

#include "regs_tcp_client.hpp"
#include "register_module.hpp"



/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* display the register values *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
void display_readval(pru_register *prureg)
{
	int j;
	std::cout << prureg->name << ": ";
	for (j = 0; j < 4; j++)
		printf("0x%.2X ", prureg->val[j]);
	std::cout << std::endl;
}

void display_readval(RegisterModule *md)
{
	std::cout << std::endl;
	pru_register *pruregs	=	md->get_allreg();
	int				numregs	=	md->get_numofregs();
	int i,j;
	std::cout << "Values: \n";
	for (i = 0; i < numregs; i++)
	{
		std::cout << std::setw(30) << std::left << pruregs[i].name << ":\t";
		for (j = 0; j < 4; j++)
			printf("0x%.2X ", pruregs[i].val[j]);
		std::cout << std::endl;
	}
	std::cout << std::endl;	
}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* display the client request *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
void display_mdreq(RegsTCPClient *rgtcp, std::string mdname)
{
	int 		rqlength	=	rgtcp->get_reqslength();
	u_int8_t	*req		=	rgtcp->get_reqs();

	int j;
	printf("\n");
	std::cout << mdname << " module request: \n";
	for (j = 0; j < 3; j++)
	{
			printf("0x%.2X\t", req[j]);
	}
	printf("\n");
	for (; j < rqlength - 1; j++)
	{
			printf("0x%.2X\t", req[j]);
			if (j && !((j - 2)%5))
				printf("\n");			
	}
	printf("0x%.2X\t", req[j]);
	printf("\n\n");
}

void display_rgreq(RegsTCPClient *rgtcp, std::string rgname)
{
	int 		rqlength	=	rgtcp->get_reqslength();
	u_int8_t	*req		=	rgtcp->get_reqs();
	printf("\n");
	std::cout << rgname << " register request: ";
	for (int i = 0; i < rqlength; i++)
	{
			printf("0x%.2X\t", req[i]);
	}
	printf("\n\n");
}

void display_mulreqs(RegsTCPClient *rgtcp, RegisterModule *md, 
						std::vector<pru_register*> *regsvec)
{
	int 		rqlength	=	rgtcp->get_reqslength();
	u_int8_t	*req		=	rgtcp->get_reqs();
	int j;
	printf("\n");
	
	for (int i = 0; i < regsvec->size(); i++)
	{
		std::cout << regsvec->at(i)->name << ", ";
	}
	std::cout << "of " << md->get_name() << " module request:\n";
	for (j = 0; j < 3; j++)
	{
			printf("0x%.2X\t", req[j]);
	}
	printf("\n");
	for (; j < rqlength - 1; j++)
	{
			printf("0x%.2X\t", req[j]);
			if (j && !((j - 2)%5))
				printf("\n");			
	}
	printf("0x%.2X\t", req[j]);
	printf("\n\n");
}