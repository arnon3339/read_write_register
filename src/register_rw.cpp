#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctype.h>
#include "json.hpp"
#include "register_rw.hpp"
#include "display_mdrg.hpp"

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* generate data dict *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
nlohmann::json    get_datadict()
{
    
    nlohmann::json  j;
    std::ifstream   i("../json/global_regs.json");
    i >> j["global_regs"];
    i.close();
    i.open("../json/offload.json");
    i >> j["offload"];
    i.close();
    i.open("../json/trigger_manager.json");
    i >> j["trigger_manager"];
    i.close();
    i.open("../json/alpide_control.json");
    i >> j["alpide_control"];
    i.close();
    i.open("../json/alpide_data.json");
    i >> j["alpide_data"];
    i.close();
    return j;
}

const nlohmann::json		RegisterRW::_regsdict   =   get_datadict();
RegisterRW::RegisterRW(RegsTCPClient *tcpclient, const uint8_t seqnum)
    :_tcpclient(tcpclient), _seqnum(seqnum), _mdvec(0), _rgvec(0),
    _regnum(0), _mdsnum(5), _rgindex(0), _mdindex(0), _pruid(0)
{
    _mdvec  =   new std::vector<RegisterModule*>;
    _rgvec  =   new std::vector<pru_register*>;

}

RegisterRW::	~RegisterRW()
{
    while (!_mdvec->empty())
    {
        delete _mdvec->back();
        _mdvec->pop_back();
    }
    delete _mdvec;        
    while (!_rgvec->empty())
    {
        delete _rgvec->back();
        _rgvec->pop_back();
    }
    delete _rgvec; 
}

bool    RegisterRW::find_register(const std::string &rgname)
{
    int i,j;
    int numregs;
    bool    isnot_reg   =   true;
    for (i = 0; i < _mdsnum && isnot_reg; i++)
    {
        numregs =  _regsdict[_md_list[i]]["register"].size();
        for (j = 0; j < numregs && (isnot_reg = 
            _regsdict[_md_list[i]]["register"][j]["name"]
            .get<std::string>().compare(rgname)); j++);
    }
    _rgvec->push_back(new pru_register);
    _rgvec->back()->name        =   rgname;
    _rgvec->back()->rw          =   _regsdict[_md_list[i - 1]]["register"]
                                        [j]["mode"].get<std::string>().compare("ro");
    _rgvec->back()->baseaddr    =   std::stoul(_regsdict[_md_list[i - 1]]
                                        ["baseaddr"].get<std::string>(), nullptr, 16);
    _rgvec->back()->addr        =   std::stoul(_regsdict[_md_list[i - 1]]["register"]
                                        [j]["address"].get<std::string>(), nullptr, 16);
    _rgvec->back()->length      =   0;
    return !isnot_reg;
}

bool    RegisterRW::found_mdreg(int &i)
{
    int j, k, l, namesize = 0;
    bool isnot_mulreg   =       false;
    bool isnot_inmd     =       false;
    int numregs;
    for (i = 1; i < _argc - 2 && !isnot_mulreg; i++)
    {
        isnot_inmd  =   false;
        for (j = 0; j < _mdsnum && !isnot_inmd; j++)
        {
            numregs =  _regsdict[_md_list[j]]["register"].size();
            for (k = 0; k < numregs; k++)
            {
                if (!_regsdict[_md_list[j]]["register"][k]["name"]
                    .get<std::string>().compare(*(_argv + i)))
                {
                    for (l = 0; l < namesize && _wregs_name[l].compare(*(_argv + i)); l++);
                    if (!namesize || l == namesize)
                    {
                        _rgvec->push_back(new pru_register);
                        _rgvec->back()->name        =   *(_argv + i);
                        _rgvec->back()->baseaddr    =    std::stoul(_regsdict[_md_list[j]]
                                                            ["baseaddr"].get<std::string>(), nullptr, 16);
                        _rgvec->back()->addr        =   std::stoul(_regsdict[_md_list[j]]["register"][k]
                                                                ["address"].get<std::string>(), nullptr, 16);
                        _wregs_name[namesize++]    =   *(_argv + i);
                    }
                    isnot_inmd  =   true;
                    break;
                }
                else if (j == _mdsnum - 1 && k == numregs - 1)
                {
                    isnot_mulreg    =   true;
                    isnot_inmd      =   true;
                    break;
                }
            }
        }
    }
    return !isnot_mulreg;                    
}

std::vector<RegisterModule*>  *RegisterRW::get_registers()
{
    return _mdvec;
}

/*
[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[                                                  [[[[[[[]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[  Expanded in register_r.cpp and register_w.cpp   [[[[]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[                                                 [[[[[[[[]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
*/