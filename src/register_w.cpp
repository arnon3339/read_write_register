#include "register_rw.hpp"
#include <iostream>
#include "display_mdrg.hpp"
union gen_val
{
    uint32_t    u32addr;
    uint8_t     u8addr[4];
}val;

void    RegisterRW::write_registers()
{
    int i, j, k, l, namesize = 0;
    std::string name, value = "";
    std::string text = "Input a register's name: ";
    std::cout << text;
    _tcpclient->connect();
    while (getline(std::cin, _rg.name))
    {
        if (find_wregister(&_rg))
        {
            std::cout << "Found " << name << " register\n";
            do
            {
                std::cout << "Fill value: ";
            } while (getline(std::cin, value) && check_inval(_rg.name, value));
            val.u32addr         =       std::stoi(value);
            for (i = 0; i < 4; i++)
            {
                _rg.val[i]     =   val.u8addr[i];
            }
            _rg.seqnum  =   0x00;
            _tcpclient->set_wreg(&_rg);
            _tcpclient->send();
            std::cout << std::endl;
            std::cout << text;
            for (j = 0; j < namesize && _wregs_name[j].compare(_rg.name); j++);
                if (!namesize || j == namesize)
                {
                    _wregs_name[namesize++] =   _rg.name;
                    collect_wrg();
                }
        }
        else
        {
            if (!_rg.rw)
                std::cerr <<_rg.name << " register is read only" << " register!" << std::endl;
            else
                std::cerr << "Not found " << "<" <<_rg.name << ">" << " register!" << std::endl;
            std::cout << std::endl;
            std::cout << text;
        }
        
    }
    _tcpclient->disconnect();
    _mdvec->push_back(new RegisterModule(_rgvec));
    _mdvec->back()->set_seqnum(_seqnum);
    _tcpclient->set_module(_mdvec->back());
    _tcpclient->start();
}

bool    RegisterRW::find_wregister(pru_register *rg)
{
    int i,j;
    int numregs;
    bool    isnot_reg   =   true;
    for (i = 0; i < _mdsnum && isnot_reg; i++)
    {
        numregs =  _regsdict[_md_list[i]]["register"].size();
        for (j = 0; j < numregs && (isnot_reg = 
            _regsdict[_md_list[i]]["register"][j]["name"]
            .get<std::string>().compare(rg->name)); j++);
    }
    if (!isnot_reg)
    {
        rg->rw =    _regsdict[_md_list[i - 1]]["register"]
                        [j]["mode"].get<std::string>().compare("ro");
        if (!rg->rw) return false;
        rg->baseaddr    =   std::stoul(_regsdict[_md_list[i - 1]]["baseaddr"].get<std::string>(), nullptr, 16);
        rg->addr        =   std::stoul(_regsdict[_md_list[i - 1]]["register"]
                                [j]["address"].get<std::string>(), nullptr, 16);
        rg->length      =   0;
    }
    return !isnot_reg;
}

void		RegisterRW::write_register(const int argc, const char **argv)
{
_argc       =   argc;
    _argv       =   argv;
    int board               =   0;
    if (isdigit(*(*(_argv) + 2)) && 
            (board   =   *(*(_argv) + 2) - '0') < 4 && !*(*(_argv) + 3))
    {
        if (board)
            std::cout << "Increasing IP address" << std::endl;
        _argv++;
        write_option();
    }
    else if ((*(*(_argv) + 2) - '0') > 4)
    {
        std::cerr << "Error: Invalid specify board" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        _argv++;
        write_option();

    }
}


bool    RegisterRW::check_inval(std::string name, std::string value)
{
    return std::stoi(value) > 10;
}

void    RegisterRW::collect_wrg()
{
    _rgvec->push_back(new pru_register);
    *_rgvec->back()     =       _rg;
}

void    RegisterRW::write_option()
{
    int i;
    if (_argc == 2)
    {
        write_registers();
    }
    else
    {
        _rg.name    =   *_argv;
        if (find_wregister(&_rg))
        {
            if (*(*_argv + 1) && !check_inval(_rg.name, *(_argv + 1)))
            {
                val.u32addr         =       std::stoi(*(_argv + 1));
                for (i = 0; i < 4; i++)
                {
                    _rg.val[i]     =   val.u8addr[i];
                }
                _rg.seqnum  =   0x00;
                _tcpclient->connect();
                _tcpclient->set_wreg(&_rg);
                _tcpclient->send();
                _tcpclient->disconnect();
            }
        }
        else
        {
            if (!_rg.rw)
                std::cerr <<_rg.name << " register is read only" << " register!" << std::endl;
            else
                std::cerr << "Not found " << "<" <<_rg.name << ">" << " register!" << std::endl;
            std::cout << std::endl;            
            exit(EXIT_FAILURE);
        }
        
    }
}
