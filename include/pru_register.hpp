#ifndef PRU_REGISTER_HPP
#define PRU_REGISTER_HPP 1

#include <string>
#include <vector>

struct pru_register
{
    std::string name;
    uint8_t addr;
    int length;
    bool    rw              =   0;
    uint32_t   baseaddr     =   0;
    uint8_t val[4]          =   {0};
    uint8_t seqnum          =   0x00;
};

void fill_values(std::vector<pru_register*> *regsvec, const uint8_t *buff);


#endif