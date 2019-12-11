#include "register_module.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

#include "register_rw.hpp"
#include "init_regs.hpp"

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Constructors *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
RegisterModule::RegisterModule (const std::string &regname)
: _seqnum(0x00)
{
    _name               =   regname;
    try
        {
        _numregs            =   RegisterRW::_regsdict[regname]["register"].size();
        _baseaddr           =   std::stoul(RegisterRW::_regsdict[regname]
                                            ["baseaddr"].get<std::string>(), nullptr, 16);

        _pruregs            =   new pru_register[_numregs];
        _module_request     =   new uint8_t[_numregs];
        _addr               =   new uint8_t[_numregs];
        
        for (int i = 0; i < _numregs; i++)
        {
            _pruregs[i].name        =   RegisterRW::_regsdict[regname]["register"]
                                            [i]["name"].get<std::string>();
            _pruregs[i].baseaddr    =   _baseaddr;
            _pruregs[i].addr        =   std::stoul(RegisterRW::_regsdict[regname]
                                            ["register"][i]["address"].get<std::string>(), nullptr, 16);
            _pruregs[i].rw          =   RegisterRW::_regsdict[regname]["register"]
                                            [i]["mode"].get<std::string>().compare("ro");   
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}

RegisterModule::RegisterModule (std::vector<pru_register*> *rgvec)
: _baseaddr(0x00000000), _seqnum(0x00)
{
    _numregs    =   rgvec->size();
    _pruregs    =   new     pru_register[_numregs];
    for (int i = 0; i < _numregs; i++)
        _pruregs[i]     =   *rgvec->at(i);
}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Deconstructors *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
RegisterModule::~RegisterModule ()
{
    delete[]    _pruregs;
    delete[]    _module_request;
    delete[]    _addr;

}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Virtual method *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
uint8_t RegisterModule::get_seqnum()
{
    return _seqnum;
}

int RegisterModule::getNumOfRegs()
{
    return _numregs;
}

int RegisterModule::get_moduel_reqs_size()
{
    return _numregs;
}

int RegisterModule::get_total_length()
{
    return 5*_numregs;
}

int RegisterModule::get_numofregs()
{
    return _numregs;
}

uint32_t    RegisterModule::get_baseaddr()
{
    return _baseaddr;
}

void    RegisterModule::set_regsval(const uint8_t *buff)
{
    int i, j;
    buff += 3;
    for (i = 0; i < _numregs; i++)
    {
        buff++;
        for (j = 0; j < 4; j++)
            _pruregs[i].val[j] =   *buff++;
    }
}

void    RegisterModule::set_mdvals(const uint8_t *buff)
{
    int i, j;
    for (i = 0; i < _numregs; i++)
    {
        buff++;
        for (j = 0; j < 4; j++)
            _pruregs[i].val[j] =   *buff++;
    }
}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* get and set *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
pru_register    *RegisterModule::get_regs_by_index(int index)
{
    return &_pruregs[index];
}

void    RegisterModule::set_modlue_request(uint8_t *reqs)
{
    for (int i = 0; i < _numregs; i++)
        _module_request[i] = reqs[i];
}

void    RegisterModule::set_seqnum(uint8_t seqnum)
{
    _seqnum     =   seqnum;
}

pru_register    *RegisterModule::get_allreg()
{
    return _pruregs;
}

uint8_t   *RegisterModule::get_total_addr()
{
    return _addr;
}

std::string RegisterModule::get_name()
{
    return _name;
}
/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Debugging *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
