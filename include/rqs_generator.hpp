#ifndef RQS_GENERATOR_HPP
#define RQS_GENERATOR_HPP 1

#include <vector>

#include "register_module.hpp"
#include "pru_register.hpp"
static const int cmaxlength = 1450;

class ReqGenerator
{
public:
    ReqGenerator();
    ReqGenerator(uint8_t *reqs, int reqslength);
    ~ReqGenerator();
    
    void            gen_read_mulregs(std::vector<pru_register*> *rgvec, 
                                        uint8_t seqnum =   0x00);
    uint8_t         *get_request();
    int             get_reqlength();
    void            gen_read_module(RegisterModule *module);
    void            gen_read_spmodule(RegisterModule *module);
    void            gen_read_modules(RegisterModule *mds, int numofmds,
                                        const uint8_t seqnum = 0);

    void            gen_readmd_register(const pru_register *regs);
    void            gen_write_module();
    void            gen_read_modules(std::vector<RegisterModule*> *mdvec, 
                                        const uint8_t seqnum = 0x00);
    void            gen_write_register(const pru_register *regs);
    bool            is_single();

private:
    uint8_t   _request[cmaxlength]; 
    int             _rqslength;
    bool            _single;
};

#endif