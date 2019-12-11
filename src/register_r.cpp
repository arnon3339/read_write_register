#include "register_rw.hpp"
#include <iostream>
#include "display_mdrg.hpp"

void		RegisterRW::read_registers()
{
    int i;
    std::string names = _md_list[0] + " ," + _md_list[1] + " ," + _md_list[2]
                        + " ," + _md_list[3] + ", and " + _md_list[4];
    for (i = 0; i < _mdsnum; i++)
        _mdvec->push_back(new RegisterModule(_md_list[i]));
    _tcpclient->start_all(_mdvec,_seqnum);            
    display_mdreq(_tcpclient, names);
    for (i = 0; i < _mdsnum; i++)
    {
        std::cout << std::setw(20) << std::right <<_md_list[i];
        display_readval(_mdvec->at(i));
    }
}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* read register module *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
void		RegisterRW::read_registers(const int argc, const char **argv)
{
    _argc       =   argc;
    _argv       =   argv;
    if (isdigit(*(*(_argv) + 2)) && 
            (_pruid   =   *(*(_argv) + 2) - '0') < 4 && !*(*(_argv) + 3))
    {
        if (_pruid)
            std::cout << "Increasing IP address" << std::endl;
        _argv++;
        read_option();
    }
    else if ((*(*(_argv) + 2) - '0') > 4)
    {
        std::cerr << "Error: Invalid specify _pruid" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        _argv++;
        read_option();

    }
}


/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* read option *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
void    RegisterRW::read_option()
{
    if (_argc == 2)
    {
        read_registers();
    }
    else
    {
        int i, j, k;
        if (**(_argv) == '-')
        {
            if (*(*(_argv) + 1) == 'm' && !*(*(_argv) + 2))
            {
                if (_argc == 3)
                {
                    std::cout << "error: Requre at least one module's name"
                        << std::endl;
                        exit(EXIT_FAILURE);
                }
                else if (_argc == 4)
                {
                    bool isnot_module = true;
                    for (i = 0; i < _mdsnum && 
                        (isnot_module = _md_list[i].compare(*(_argv + 1))); i++);
                    if (!isnot_module)
                    {
                        _mdvec->push_back(new RegisterModule(_md_list[i]));
                        _mdvec->back()->set_seqnum(_seqnum);
                        _tcpclient->set_spmodule(_mdvec->back());
                        _tcpclient->start();
                        display_mdreq(_tcpclient, _md_list[i]);
                        std::cout << std::setw(20) << std::right << _md_list[i];
                        display_readval(_mdvec->back());

                    }
                    else
                    {
                        std::cout << "error: There is no <" << *(_argv + 1) << "> module." 
                            << std::endl;
                            std::cout << "Avaliable modules: " << std::endl;
                            for (i = 0; i < _mdsnum; i++)
                                std::cout << "<" << _md_list[i] << ">" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    bool    isnot_mulmd =   false;
                    for (i = 1; i < _argc - 2 && !isnot_mulmd; i++)
                        for (j = 0; j < _mdsnum; j++)
                            if (!_md_list[j].compare(*(_argv + i)))
                            {
                                if (!_mds_state[j])
                                    _mds_state[j]    =   true;
                                break;
                            }
                            else if (j == _mdsnum - 1)
                            {
                                isnot_mulmd =   true;
                                break;
                            }
                    if (isnot_mulmd)
                    {
                        std::cout << "error: There is no <" << *(_argv + i - 1) << "> module name." << std::endl;
                        std::cout << "Avaliable modules: " << std::endl;
                        for (i = 0; i < _mdsnum; i++)
                            std::cout << "<" << _md_list[i] << ">" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    else
                    {
                        int mdsize = 0;
                        std::string names  =  "";
                        for (i = 0; i < _mdsnum; i++)
                            if (_mds_state[i])
                            {
                                _mdvec->push_back(new RegisterModule(_md_list[i]));
                                names    +=   _md_list[i] + " ,";
                            }
                        _tcpclient->start_all(_mdvec, _seqnum);
                        display_mdreq(_tcpclient,  names);
                        mdsize  =   _mdvec->size();
                        for (i = 0; i < mdsize; i++)
                        {
                            std::cout << std::setw(20) << std::right << _mdvec->at(i)->get_name();
                            display_readval(_mdvec->at(i));
                        }
                    }
                }
                
            }
            else if (*(*(_argv) + 1) == 'r' && !*(*(_argv) + 2))
            {
                if (_argc == 3)
                {
                    std::cout << "error: Requre at least one register's name"
                        << std::endl;
                        exit(EXIT_FAILURE);
                }
                else if (_argc == 4)
                {
                    if (find_register(*(_argv + 1)))
                    {
                        _mdvec->push_back(new RegisterModule(_rgvec));
                        _mdvec->back()->set_seqnum(_seqnum);
                        _tcpclient->set_spmodule(_mdvec->back());
                        _tcpclient->start();
                        display_mdreq(_tcpclient, _rgvec->back()->name);
                        display_readval(_mdvec->back()->get_allreg());
                    }
                    else
                    {
                        std::cout << "error: There is no <" << *(_argv + 1) << "> register." 
                            << std::endl;
                            std::cout << "Avaliable register: " << std::endl;
                            // display_allregs();
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                               
                    if (!found_mdreg(i))
                    {
                        std::cout << "error: There is no <" << *(_argv + i - 1) << "> register." << std::endl;
                        std::cout << "Avaliable registers: " << std::endl;
                            // display_allregs();
                        exit(EXIT_FAILURE);
                    }
                    else
                    {
                        int numvec;
                        pru_register    *prureg;
                        std::string     names = "";
                        _mdvec->push_back(new RegisterModule(_rgvec));
                        _mdvec->back()->set_seqnum(_seqnum);
                        _tcpclient->set_spmodule(_mdvec->back());
                        _tcpclient->start();
                        numvec  =   _mdvec->back()->get_numofregs();
                        prureg  =   _mdvec->back()->get_allreg();
                        display_mdreq(_tcpclient, "Compound");
                            
                        for (i = 0; i < numvec; i++)
                            display_readval(&prureg[i]);
                    }
                }
                
            }
            else
            {
                std::cout << "error: Invalid option.\nplease use <-m> following by <module's name> to read module"
                << ", or <-r> following by <registers's name> to read register."
                    << std::endl;

                exit(EXIT_FAILURE);			
            }
        }
        else
        {
            std::cout << "error: Invalid option.\nplease use <-m> following by <module's name> to read module"
                << ", or <-r> following by <registers's name> to read register."
                    << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}