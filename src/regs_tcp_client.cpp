#include "regs_tcp_client.hpp"
#include <iostream>
#include <iomanip>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>

#include "register_module.hpp"
#include "pru_register.hpp"
#include "rqs_generator.hpp"


/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Constructors *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
RegsTCPClient::RegsTCPClient(boost::asio::io_service &service,
                    const std::string &name, const std::string &host, 
                    const int port)
: _reqs_length(0), _ioservice(service), _name(name), _host(host), _port(port),
 _socket(service), _reg(0), _regmd(0), 
 _endpoint( boost::asio::ip::address::from_string(host), port )
{}

RegsTCPClient::RegsTCPClient(boost::asio::io_service &service,
                    const std::string &name, const std::string &host, 
                    const int port, uint8_t *reqs, int num_reqs)
	: _ioservice(service), _name(name), _host(host), _port(port),
	 _socket(service), _request(reqs), _reqs_length(num_reqs), _reg(0),
	  _regmd(0), _endpoint( boost::asio::ip::address::from_string(host), port )
{		
}

RegsTCPClient::RegsTCPClient(boost::asio::io_service &service,
                    const std::string &name, const std::string &host, 
                    const int port, RegisterModule *regmd)
	:_ioservice(service), _name(name), _host(host), _port(port),
    _socket(service), _reqs_length(0), _regmd(regmd),
	_endpoint( boost::asio::ip::address::from_string(host), port )
{
	_genreq.gen_read_module(_regmd);
	_request		=	_genreq.get_request();
	_reqs_length	=	_genreq.get_reqlength();
}

RegsTCPClient::RegsTCPClient(boost::asio::io_service &service,
                    const std::string &name, const std::string &host, 
                    const int port, pru_register *regs)
:_ioservice(service), _name(name), _host(host), _port(port),
    _socket(service), _reqs_length(0), _reg(regs), _regmd(0),
	_endpoint( boost::asio::ip::address::from_string(host), port )
{		
	_genreq.gen_readmd_register(_reg);
	_request		=	_genreq.get_request();
	_reqs_length	=	_genreq.get_reqlength();
}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Deconstructors *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
RegsTCPClient::~RegsTCPClient()
{
    _socket.close();
}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Starting *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
void	RegsTCPClient::start()
{
   	_socket.connect(_endpoint);
   	boost::system::error_code error;
  	boost::asio::write(_socket, boost::asio::buffer(_request, _reqs_length), error );
	if( error ) 
    	std::cout << "send failed: " << error.message() << std::endl;
  	else 
    	std::cout << "send correct!" << std::endl;

	boost::asio::read(_socket, boost::asio::buffer(_buffer, _reqs_length), 
		boost::asio::transfer_all(), error );

	if( error && error != boost::asio::error::eof ) 
    	std::cout << "receive failed: " << error.message() << std::endl;
  	else 
    	if (_regmd)
			_regmd->set_regsval(_buffer);
		else if (_reg)
			for (int i = 0; i < _reqs_length - 1; i++)
				_reg->val[i]	=	_buffer[i + 4];  

	for (int i = 0; i < _reqs_length; i++)
		printf("0x%.2X\t", _buffer[i]);
	printf("\n");
  	
}

void	RegsTCPClient::start_all(std::vector<RegisterModule*> *mdvec, const char mode,
                    			const uint8_t seqnum)
{
	int i, nummds;
	const uint8_t *buff	=	_buffer;
	buff	+=	3;
	switch (mode)
	{
	case 's':
		for (i = 0; i < mdvec->size(); i++)
		{
			set_module(mdvec->at(i));
			start();
			_socket.close();
		}	
		break;
	case 'c':
		_genreq.gen_read_modules(mdvec, seqnum);
		set_reqs(_genreq.get_request(), _genreq.get_reqlength());
		// display_request();
		_reg	=	0;
		_regmd	=	0;
		start();
		nummds	=	mdvec->size();
		for (i = 0; i < nummds; i++)
		{
			buff	+=	i? 5 * (mdvec->at(i - 1)->get_numofregs()): 0;	
			mdvec->at(i)->set_mdvals(buff);
		}
		break;
	default:
		std::cout << "error: Invalid request generator mode" << std::endl;
		break;
	}	
}


void	RegsTCPClient::start_all(std::vector<pru_register*>   *rgvec)
{
	_genreq.gen_read_mulregs(rgvec);
	_request		=	_genreq.get_request();
	_reqs_length	=	_genreq.get_reqlength();
	_socket.connect(_endpoint);
	boost::system::error_code error;
	boost::asio::write(_socket, boost::asio::buffer(_request, _reqs_length), error );
	if( error ) 
		std::cout << "send failed: " << error.message() << std::endl;
	else 
		std::cout << "send correct!" << std::endl;

	boost::asio::read(_socket, boost::asio::buffer(_buffer, _reqs_length), 
		boost::asio::transfer_all(), error );

	if( error && error != boost::asio::error::eof ) 
		std::cout << "receive failed: " << error.message() << std::endl;
	else 
		fill_values(rgvec, _buffer);	
}

/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* get and set *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/

std::string	RegsTCPClient::get_host()
{
    return _host;
}

void	RegsTCPClient::set_host(const std::string &host)
{
    _host = host;
}

void	RegsTCPClient::set_name(const std::string &name)
{
    _name = name;
}

std::string	RegsTCPClient::get_name()
{
    return _name;
}

void	RegsTCPClient::set_port(const int port)
{
    _port = port;
}

int	RegsTCPClient::get_port()
{
    return _port;
}

uint8_t *RegsTCPClient::get_buffer()
{
    return _buffer;
}

void	RegsTCPClient::set_reqs(uint8_t *reqs, int num_reqs)
{
	_request		=	reqs;
	_reqs_length	=	num_reqs;
}

uint8_t *RegsTCPClient::get_reqs()
{
	return _request;
}

void	RegsTCPClient::set_module(RegisterModule *md)
{
	_reg	=	0;
	_regmd	=	md;
	_genreq.gen_read_module(_regmd);
	_request		=	_genreq.get_request();
	_reqs_length	=	_genreq.get_reqlength();

}
void	RegsTCPClient::set_reg(pru_register *reg)
{
	_regmd	=	0;
	_reg	=	reg;
	_genreq.gen_readmd_register(_reg);
	_request		=	_genreq.get_request();
	_reqs_length	=	_genreq.get_reqlength();
	
}

int	RegsTCPClient::get_reqslength()
{
	return _reqs_length;
}
/*-<o>-<x>-<O>-<X>-<O>-<x>-<o>-* Debugging *-<o>-<x>-<O>-<X>-<O>-<x>-<o>-*/
void	RegsTCPClient::display()
{
    std::cout << std::left << std::setw(7) << "Name"    << "is  " << _name      << "." << std::endl;    
    std::cout << std::left << std::setw(7) << "Host"    << "is  " << _host      << "." << std::endl;
    std::cout << std::left << std::setw(7) << "Port"    << "is  " << _port      << "." << std::endl;
    std::cout << std::left << std::setw(7) << "Buffer"  << "is  " << _buffer    << "." << std::endl;

}

void	RegsTCPClient::display_request()
{
	int i;
	printf("\n");
	for (i = 0; i < 3; i++)
	{
			printf("0x%.2X\t", _request[i]);
	}
	printf("\n");
	for (; i < _reqs_length - 1; i++)
	{
			printf("0x%.2X\t", _request[i]);
			if (i && !((i - 2)%5))
				printf("\n");			
	}
	printf("0x%.2X\t", _request[i]);
	printf("\n\n");
}