#ifndef REGISTER_MODULE_HPP
#define REGISTER_MODULE_HPP 1

#include <string>
#include "pru_register.hpp"

class RegisterModule 
{
public:
    // Constructors
    RegisterModule ();
    RegisterModule (const std::string &name);
    RegisterModule (const int n);

    // Deconstructors
    ~RegisterModule ();

    // get and set
    int                 getNumOfRegs();
    pru_register        *get_regs_by_index(int index);
    void                set_modlue_request(uint8_t *reqs);
    void                set_seqnum(uint8_t seqnum);
    int                 get_moduel_reqs_size();
    int                 get_total_length();
    int                 get_numofregs();
    uint8_t             *get_total_addr();
    u_int32_t           get_baseaddr();
    uint8_t             get_seqnum();
    pru_register        *get_allreg();
    void                set_regsval(const uint8_t *buff);
    std::string         get_name();
    void                set_mdvals(const uint8_t *buff);
    
    // debugging


private:
    int             _numregs;
    uint32_t        _baseaddr;
    uint8_t         _seqnum;
    pru_register    *_pruregs;
    uint8_t         *_module_request;
    uint8_t         *_addr;
    std::string     _name;
};

#endif