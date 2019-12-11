#include "rqs_generator.hpp"
#include "register_module.hpp"
#include "pru_const.hpp"
#include "pru_register.hpp"

union gen_length
{
    uint16_t    u16length;
    uint8_t     u8length[2];
}len;

union gen_addr
{
    uint32_t    u32addr;
    uint8_t     u8addr[4];
}addr;

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Constructors *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
ReqGenerator::ReqGenerator(): _rqslength(0)
{}

ReqGenerator::ReqGenerator(uint8_t *reqs, int rqslength)
: _rqslength(0)
{
    for (int i = 0; i < rqslength; i++)
        _request[rqslength++] = *reqs++;
}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Deconstructors *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
ReqGenerator::~ReqGenerator()
{}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Module request generator *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
void ReqGenerator::gen_read_module(RegisterModule *module)
{
    _single     =   false;
    int legnth = 5;
    int i;
    addr.u32addr            =           module->get_baseaddr();
    _rqslength  =   0;

    len.u16length               =       module->get_total_length();
    _request[_rqslength++]      =       len.u8length[1];
    _request[_rqslength++]      =       len.u8length[0];        
    _request[_rqslength++]      =       cmdtype::fmwrw;

    for (i = 0; i < module->get_numofregs(); i++)
    {
        _request[_rqslength++]  = cmdtype::readop;
        _request[_rqslength++]  =   addr.u8addr[3];
        _request[_rqslength++]  =   addr.u8addr[2];
        _request[_rqslength++]  =   addr.u8addr[1];
        _request[_rqslength++]  =   addr.u8addr[0]; 
        addr.u32addr            +=  4;
    }
    _request[_rqslength++]  =   module->get_seqnum();
   
}

void    ReqGenerator::gen_read_spmodule(RegisterModule *module)
{
    int i;
    int num         =       module->get_numofregs();
    pru_register    *prureg =   module->get_allreg();
    _single     =   false;
    _rqslength  =   0;

    len.u16length               =       module->get_total_length();
    _request[_rqslength++]      =       len.u8length[1];
    _request[_rqslength++]      =       len.u8length[0];        
    _request[_rqslength++]      =       cmdtype::fmwrw;

    for (i = 0; i < num; i++)
    {
        addr.u32addr            =   prureg[i].baseaddr +  prureg[i].addr;
        // printf("0x%X\n", addr.u32addr);
        _request[_rqslength++]  =   cmdtype::readop;
        _request[_rqslength++]  =   addr.u8addr[3];
        _request[_rqslength++]  =   addr.u8addr[2];
        _request[_rqslength++]  =   addr.u8addr[1];
        _request[_rqslength++]  =   addr.u8addr[0]; 
    }
    _request[_rqslength++]  =   module->get_seqnum();

}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Register request generator *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
void ReqGenerator::gen_readmd_register(const pru_register *regs)
{
    _rqslength      =   0;
    len.u16length   =   5;
    _request[_rqslength++]  =   len.u8length[1];
    _request[_rqslength++]  =   len.u8length[0];
    _request[_rqslength++]  =   cmdtype::fmwrw;
    _request[_rqslength++]  =   cmdtype::readop;
     addr.u32addr           =   regs->baseaddr + regs->addr;
    _request[_rqslength++]  =   regs->baseaddr;
    _request[_rqslength++]  =   addr.u8addr[3];
    _request[_rqslength++]  =   addr.u8addr[2];
    _request[_rqslength++]  =   addr.u8addr[1];
    _request[_rqslength++]  =   addr.u8addr[0];
    _request[_rqslength++]      =   regs->seqnum;
}

void    ReqGenerator::gen_read_mulregs(std::vector<pru_register*> *rgvec, 
                                            uint8_t seqnum)
{
    _rqslength      =   0;
    len.u16length   =   rgvec->size()*5;
    _request[_rqslength++]  =   len.u8length[1];
    _request[_rqslength++]  =   len.u8length[0];
    _request[_rqslength++]  =   cmdtype::fmwrw;
    for (int i = 0; i < rgvec->size(); i++)
    {
        _request[_rqslength++]  =   cmdtype::readop;
        addr.u32addr    =   rgvec->at(i)->baseaddr + rgvec->at(i)->addr;
        _request[_rqslength++]  =   addr.u8addr[3];
        _request[_rqslength++]  =   addr.u8addr[2];
        _request[_rqslength++]  =   addr.u8addr[1];
        _request[_rqslength++]  =   addr.u8addr[0];
    }
    _request[_rqslength++]      =   seqnum;
    
}

void ReqGenerator::gen_write_register(const pru_register *regs)
{
    _rqslength      =   0;
    len.u16length   =   5;
    addr.u32addr            =   regs->baseaddr + regs->addr;
    _request[_rqslength++]  =   len.u8length[1];
    _request[_rqslength++]  =   len.u8length[0];
    _request[_rqslength++]  =   cmdtype::fmwrw;
    _request[_rqslength++]  =   cmdtype::writeop;
    _request[_rqslength++]  =   addr.u8addr[3];
    _request[_rqslength++]  =   addr.u8addr[2];
    _request[_rqslength++]  =   addr.u8addr[1];
    _request[_rqslength++]  =   addr.u8addr[0];
    _request[_rqslength++]  =   regs->val[3];
    _request[_rqslength++]  =   regs->val[2];
    _request[_rqslength++]  =   regs->val[1];
    _request[_rqslength++]  =   regs->val[0];
    _request[_rqslength++]      =   regs->seqnum;
}

bool    ReqGenerator::is_single()
{
    return _single;
}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Multiple modules request *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
void    ReqGenerator::gen_read_modules(std::vector<RegisterModule*> *mdvec, 
                                        const uint8_t seqnum)
{
    int i, j, nummds, nummdregs;
    len.u16length   =   0;
    _rqslength      =   0;
    nummds          =   mdvec->size();
    for (i = 0; i < mdvec->size(); i++)
        len.u16length   +=  mdvec->at(i)->get_total_length();
    _request[_rqslength++]      =       len.u8length[1];
    _request[_rqslength++]      =       len.u8length[0];        
    _request[_rqslength++]      =       cmdtype::fmwrw;

    for (i = 0; i < nummds; i++)
    {
        nummdregs       =   mdvec->at(i)->get_numofregs();
        addr.u32addr    =   mdvec->at(i)->get_baseaddr();
        for (j = 0; j < nummdregs; j++)
        {
            _request[_rqslength++]  = cmdtype::readop;
            _request[_rqslength++]  =   addr.u8addr[3];
            _request[_rqslength++]  =   addr.u8addr[2];
            _request[_rqslength++]  =   addr.u8addr[1];
            _request[_rqslength++]  =   addr.u8addr[0]; 
            addr.u32addr            +=  4;           
        }
    }
    _request[_rqslength++]  =   seqnum;
}

void    ReqGenerator::gen_read_modules(RegisterModule *mds, int numofmds, 
                                        const uint8_t seqnum)
{
    int i, j, nummdregs;
    len.u16length   =   0;
    _rqslength      =   0;
    for (i = 0; i < numofmds; i++)
        len.u16length   +=  (mds + i)->get_total_length();
    _request[_rqslength++]      =       len.u8length[1];
    _request[_rqslength++]      =       len.u8length[0];        
    _request[_rqslength++]      =       cmdtype::fmwrw;

    for (i = 0; i < numofmds; i++)
    {
        nummdregs       =   (mds + i)->get_numofregs();
        addr.u32addr    =   (mds + i)->get_baseaddr();
        for (j = 0; j < nummdregs; j++)
        {
            _request[_rqslength++]  = cmdtype::readop;
            _request[_rqslength++]  =   addr.u8addr[3];
            _request[_rqslength++]  =   addr.u8addr[2];
            _request[_rqslength++]  =   addr.u8addr[1];
            _request[_rqslength++]  =   addr.u8addr[0]; 
            addr.u32addr            +=  4;           
        }
    }
    _request[_rqslength++]  =   seqnum;
}


/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* get and set *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
uint8_t *ReqGenerator::get_request()
{
    return _request;
}

int ReqGenerator::get_reqlength()
{
    return _rqslength;
}