#include "register_module.hpp"
#include <iostream>
#include <iomanip>
#include <map>
#include "init_regs.hpp"

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Constructors *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
RegisterModule::RegisterModule ()
    : _numregs(13), _baseaddr(0x20000000), _seqnum(0x00), _name("global register")
{
    _pruregs            =   new pru_register[_numregs];
    _module_request     =   new uint8_t[_numregs];
    _addr               =   new uint8_t[_numregs];
    _pruregs[0].name    = "date_code";              _pruregs[1].name    = "hash_code";
    _pruregs[2].name    = "led_blinky";             _pruregs[3].name    = "reset_bank_bitslice";
    _pruregs[4].name    = "enable_alpide_clock";    _pruregs[5].name    = "ru_id";
    _pruregs[6].name    = "max_size";               _pruregs[7].name    = "max_wait_time";
    _pruregs[8].name    = "filter_data_word";       _pruregs[9].name    = "empty_frame_compression";
    _pruregs[10].name   = "check_id";               _pruregs[11].name   = "DPA_delta";
    _pruregs[12].name   = "tabs";

    _pruregs[0].addr    = 0x00;  _pruregs[1].addr    = 0x04;
    _pruregs[2].addr    = 0x08;  _pruregs[3].addr    = 0x0C;
    _pruregs[4].addr    = 0x10;  _pruregs[5].addr    = 0x14;
    _pruregs[6].addr    = 0x18;  _pruregs[7].addr    = 0x1C;
    _pruregs[8].addr    = 0x20;  _pruregs[9].addr    = 0x24;
    _pruregs[10].addr   = 0x28;  _pruregs[11].addr   = 0x2C;
    _pruregs[12].addr   = 0x30;

    _pruregs[0].length    = 0x20;  _pruregs[1].length    = 0x20;
    _pruregs[2].length    = 0x01;  _pruregs[3].length    = 0x05;
    _pruregs[4].length    = 0x01;  _pruregs[5].length    = 0x06;
    _pruregs[6].length    = 0x20;  _pruregs[7].length    = 0x20;
    _pruregs[8].length    = 0x02;  _pruregs[9].length    = 0x10;
    _pruregs[10].length   = 0x01;  _pruregs[11].length   = 0x07;
    _pruregs[12].length   = 0x09;

    for (int i = 0; i < _numregs; i++)
    {
        _pruregs[i].baseaddr    =   0x20000000;
        _addr[i]                =   _pruregs[i].addr;
    }
}

RegisterModule::RegisterModule (const std::string &name)
    : _seqnum(0x00), _name(name)
{
    if (!name.compare("global"))
    {
        _numregs            =   13;
        _pruregs            =   new pru_register[_numregs];
        _module_request     =   new uint8_t[_numregs];
        _addr               =   new uint8_t[_numregs];
        _baseaddr           =   0x20000000;
        init_globreg(_pruregs);
    }
    else if (!name.compare("offload"))
    {
        _numregs            =   11;
        _pruregs            =   new pru_register[_numregs];
        _module_request     =   new uint8_t[_numregs];
        _addr               =   new uint8_t[_numregs];
        _baseaddr           =   0x20001000;
        init_offld(_pruregs);
    }
    else if (!name.compare("trigger"))
    {
        _numregs            =   21;
        _pruregs            =   new pru_register[_numregs];
        _module_request     =   new uint8_t[_numregs];
        _addr               =   new uint8_t[_numregs];
        _baseaddr           =   0x24220000;
        init_trgg(_pruregs);
    }
    else
    {
        std::cout << "error: there is no " << name << " module." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < _numregs; i++)
    {
        _pruregs[i].baseaddr    =   _baseaddr;
        _addr[i]                =   _pruregs[i].addr;
    }
}

RegisterModule::RegisterModule (const int n): _seqnum(0x00)
{
    switch (n)
    {
    case 0:
        _name               =   "global register";
        _numregs            =   13;
        _pruregs            =   new pru_register[_numregs];
        _module_request     =   new uint8_t[_numregs];
        _addr               =   new uint8_t[_numregs];
        _baseaddr           =   0x20000000;
        init_globreg(_pruregs);
        break;
    case 1:
        _name               =   "offload";
        _numregs            =   11;
        _pruregs            =   new pru_register[_numregs];
        _module_request     =   new uint8_t[_numregs];
        _addr               =   new uint8_t[_numregs];
        _baseaddr           =   0x20001000;
        init_offld(_pruregs);
        break;
    case 2:
        _name               =   "trigger manager";
        _numregs            =   21;
        _pruregs            =   new pru_register[_numregs];
        _module_request     =   new uint8_t[_numregs];
        _addr               =   new uint8_t[_numregs];
        _baseaddr           =   0x24220000;
        init_trgg(_pruregs);
        break;
    default:
        std::cout << "error: there is no " << n << " index module." << std::endl;
        exit(EXIT_FAILURE);
        break;
    }
    for (int i = 0; i < _numregs; i++)
    {
        _pruregs[i].baseaddr    =   _baseaddr;
        _addr[i]                =   _pruregs[i].addr;
    }
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
