#ifndef OPT_SELECTOR_HPP
#define OPT_SELECTOR_HPP 1

#include <iostream>

#include "register_module.hpp"
#include "pru_register.hpp"

#define MULTIMD 10
#define MDMULRG 11

static const int mdnum = 5;
const std::string modules[] = {"global", "offload", "trigger",
								"alctrl", "aldata"};
int md_state[5]    =   {0};
int mdrg_state[40]  =   {0};

static void display_allmd();
static void display_allreg(RegisterModule *md);

int select_md(const int argc, const char **argv)
{
    int i, j;
    if (**(argv + 1) == '-')
        if (*(*(argv + 1) + 1) == 'm')
        {
            if (argc == 2)
            {
                std::cout << "error: Requre <-m> <module's name>"
                    << std::endl;
                    return -1;
            }
            else if (argc == 3 || *(*(argv + 3)) == '-')
            {
                int isnot_module = 1;
                for (i = 0; i < mdnum && 
                    (isnot_module = modules[i].compare(*(argv + 2))); i++);
                if (!isnot_module)
                    return i;
                else
                {
                    std::cout << "error: There is no <" << *(argv + 2) << "> module." 
                        << std::endl;
                        std::cout << "Avaliable modules: " << std::endl;
                        display_allmd();
                  return -1;
                }
            }
            else
            {
                bool    isnot_mulmd =   false;
                for (i = 2; i < argc && !isnot_mulmd; i++)
                    for (j = 0; j < mdnum; j++)
                        if (!modules[j].compare(*(argv + i)))
                        {
                            if (!md_state[j])
                                md_state[j]    =   1;
                            break;
                        }
                        else if (j == mdnum - 1)
                            isnot_mulmd =   true;
                if (isnot_mulmd)
                {
                    std::cout << "error: There is no <" << *(argv + i - 1) << "> module name." << std::endl;
                    std::cout << "Avaliable modules: " << std::endl;
                        display_allmd();
                    return -1;
                }
                else
                    return MULTIMD;
            }
            
        }
        else
        {
            std::cout << "error: Invalid option.\nplease use <-m> following by <module's name>."
                    << std::endl;
            return -1;				
        }
    else
    {
        std::cout << "error: Invalid option.\nplease use <-m> following by <module's name>."
                << std::endl;
        return -1;			
    }
        
}

int select_rg(const int argc, const char **argv, RegisterModule *md)
{
    int i, j;
    pru_register *pruregs;
    if (argc > 4)
        if (**(argv + 3) == '-')
            if (*(*(argv + 3) + 1) == 'r')
            {
                bool    isnot_mdmulrg =   false;

                pruregs =	md->get_allreg();
                int num_reg	=	md->get_numofregs();

                for (i = 4; i < argc && !isnot_mdmulrg; i++)
                    for (j = 0; j < num_reg; j++)
                        if (!pruregs[j].name.compare(*(argv + i)))
                        {
                            if (!mdrg_state[j])
                                mdrg_state[j]    =   1;
                            break;
                        }
                        else if (j == num_reg - 1)
                            isnot_mdmulrg =   true;
                if (isnot_mdmulrg)
                {
                    std::cout << "error: There is no <" << *(argv + i - 1) << "> register." 
                    << std::endl;
                    std::cout << "Avaliable register: " << std::endl;
                    display_allreg(md);
                    return -1;                    
                }
                else
                    if (argc == 5)
                        return j;
                    else
                        return MDMULRG;
            } 
            else
            {
                std::cout << "error: Invalid option."
                        <<"\nplease use <-r> following by <-register's name>."
                        << std::endl;
                return -1;		
            }
        else
        {
            std::cout << "error: Invalid prefix option."
                        <<"\nplease use <-r> <-register's name>."
                        << std::endl;
            return -1;				
        }
    else
    {
        std::cout << "error: Require at least one register" 
                    <<"option name option <-r> <-register's name>." 
                    << std::endl;
        return -1;
    }
}

void display_allmd()
{
	for (int i = 0; i < mdnum; i++)
		std::cout << "<" << modules[i] << ">" << std::endl;
}

void display_allreg(RegisterModule *md)
{
	int num	=	md->get_numofregs();
	pru_register *pruregs	=	md->get_allreg();
	for (int i = 0; i < num; i++)
		std::cout << "<" << (pruregs++)->name << ">" << std::endl;
}

#endif