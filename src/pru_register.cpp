#include <string>
#include <fstream>
#include <iostream>
#include "pru_register.hpp"

void fill_values(std::vector<pru_register*> *regsvec, const uint8_t *buff)
{
    int i, j;
    buff += 3;
    for (i = 0; i < regsvec->size(); i++)
    {
        buff++;
        for (j = 0; j < 4; j++)
            regsvec->at(i)->val[j] =   *buff++;
    }
}


