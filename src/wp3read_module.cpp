#include "wp3read_module.hpp"

Wp3ReadModule::Wp3ReadModule(RegsTCPClient *tcpclient, const uint8_t seqnum)
    :_tcpclient(tcpclient), _seqnum(seqnum)
{
    _mdvec  =   new     std::vector<RegisterModule*>;
    for (int i = 0; i < 3; i++)
        _mdvec->push_back(new RegisterModule(i));
    read_allmodules();
}

Wp3ReadModule::	~Wp3ReadModule()
{
    while (!_mdvec->empty())
    {
        delete  _mdvec->back();
        _mdvec->pop_back();
    }
    delete  _mdvec;
}

// Wp3ReadModule::Wp3ReadModule(RegsTCPClient *tcpclient, std::vector<RegisterModule*> *mdvec, const uint8_t seqnum)
//     :_tcpclient(tcpclient), _mdvec(mdvec), _seqnum(seqnum)
// {
//     read_allmodules();    
// }

void    Wp3ReadModule::read_allmodules()
{
    _genrqs.gen_read_modules(_mdvec, _seqnum);
    _tcpclient->start_all(_mdvec, 'c', _seqnum);
}