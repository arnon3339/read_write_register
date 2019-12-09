#ifndef REGS_TCP_CLIENT_HPP 
#define REGS_TCP_CLIENT_HPP 1
#include <boost/asio.hpp>
#include <string>
#include <vector>

#include "register_module.hpp"
#include "pru_register.hpp"
#include "rqs_generator.hpp"

using boost::asio::ip::tcp;

#define MAXBUFF 1450

class RegsTCPClient
{
public:
    // Constructors
    RegsTCPClient();
    RegsTCPClient(boost::asio::io_service &service,
                    const std::string &name, const std::string &host, 
                    const int port);

    RegsTCPClient(boost::asio::io_service &service,
                    const std::string &name, const std::string &host, 
                    const int port, RegisterModule *regmd);

    RegsTCPClient(boost::asio::io_service &service,
                    const std::string &name, const std::string &host, 
                    const int port, pru_register *regs);


    RegsTCPClient(boost::asio::io_service &service,
                    const std::string &name, const std::string &host, 
                    const int port, uint8_t *reqs, int rqslength = 8);

    // Deconstructors
    ~RegsTCPClient();

    // Start
    void start();

    // Start all modules
    void start_all(RegisterModule *mds, const int numbofmds);
    void start_all(std::vector<RegisterModule*> *mdvec, const char mode = 's',
                    const uint8_t seqnum = 0x00);

    void start_all(std::vector<pru_register*>   *rgvec);


    // get and set method
    void            set_host(const std::string &host);
    std::string     get_host();
    void            set_name(const std::string &name);
    std::string     get_name();
    void            set_port(const int port);
    int             get_port();
    void            display();
    uint8_t         *get_buffer();
    void            set_reqs(uint8_t *reqs, int num_reqs);
    uint8_t         *get_reqs();
    void            set_module(RegisterModule *md);
    void            set_reg(pru_register *reg);
    int             get_reqslength();

private:
    // Connect
    void start_connect();

    // Check timeout
    void check_deadline();
    // Stop
    void    stop();

    // Send
    void    send();

    // Receive
    void    receive();

    // connection handling
    void    handle_connect(const boost::system::error_code& error,
                            tcp::resolver::iterator endpoint_iter);

    // write handling
    void    handle_send(const boost::system::error_code& error);

    // read handling
    void    handle_receive(const boost::system::error_code& error);

    // set multiple modules request
    void    set_modules(std::vector<RegisterModule*> *mdvec, const uint8_t seqnum);

    // display request
    void    display_request();

    // start individual module
    void start_mds(std::vector<RegisterModule*> *mdvec);

private:
    int                             _reqs_length;
    RegisterModule                  *_regmd;
    pru_register                    *_reg;
    std::string                     _host;
    int                             _port;
    std::string                     _name;
    uint8_t                         _buffer[MAXBUFF];
    uint8_t                         *_request;
    boost::asio::io_service&        _ioservice;
    tcp::endpoint                   _endpoint;
    tcp::socket                     _socket;
    ReqGenerator	                _genreq;
};

#endif