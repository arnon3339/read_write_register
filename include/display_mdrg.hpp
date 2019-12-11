#ifndef DISPLAY_MDRG_HPP
#define DISPLAY_MDRG_HPP 1

#include <vector>
#include "regs_tcp_client.hpp"
#include "register_module.hpp"


void display_readval(pru_register *prureg);

void display_readval(RegisterModule *md);

void display_mdreq(RegsTCPClient *rgtcp, std::string mdname);

void display_rgreq(RegsTCPClient *rgtcp, std::string rgname);

void display_mulreqs(RegsTCPClient *rgtcp, RegisterModule *md, 
						std::vector<pru_register*> *regsvec);
#endif