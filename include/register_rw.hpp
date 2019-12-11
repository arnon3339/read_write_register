#ifndef REGISTER_RW_HPP
#define REGISTER_RW_HPP 1

#include <iomanip>
#include <vector>
#include <string>

#include "regs_tcp_client.hpp"
#include "register_module.hpp"
#include "rqs_generator.hpp"
#include "json.hpp"

class RegisterRW
{
public:
	RegisterRW(RegsTCPClient *tcpclient, const uint8_t seqnum = 0x00);
	RegisterRW(RegsTCPClient *tcpclient, std::vector<RegisterModule*> *mdvec, 
				const uint8_t seqnum = 0x00);
	RegisterRW(RegsTCPClient *tcpclient, std::vector<pru_register*> *rgvec);
	RegisterRW(RegsTCPClient *tcpclient, RegisterModule *reg);
	RegisterRW(RegsTCPClient *tcpclient, pru_register *reg);


	~RegisterRW();

	void				get_request();
	void				display_request();
	void 				display_readvals();
	void				read();
	void 				read_allmodules();
	void				read_registers();
	void				read_registers(std::vector<RegisterModule*> *mds);
	void				read_registers(const int argc, const char **argv);
	void				write_register(const int argc, const char **argv);

	std::vector<RegisterModule*>  *get_registers();

	static const nlohmann::json		_regsdict;

private:
	void		read_allregs();
	void		read_singmodule();
	void		read_singreg();
	void		read_option();
	void		write_option();
	void		write_registers();

	bool		find_register(const std::string &rgname);
	bool		find_wregister(pru_register *rg);
	bool		found_mdreg(int &i);

	void		collect_wrg();
	bool		check_inval(std::string name, std::string value);

	const char			**_argv;
	int				_argc;

	RegsTCPClient					*_tcpclient;
	std::vector<RegisterModule*>	*_mdvec;
	std::vector<pru_register*>		*_rgvec;
	pru_register					_rg;
	const int						_regnum;
	const int						_mdsnum;
	int								_rgindex;
	int								_mdindex;
	uint8_t							_seqnum;
	bool							_mds_state[5]{false};
	bool							_regs_state[30]{false};
	int								_pruid;
	std::string						_wregs_name[30]{""};
	std::string						_md_list[5]{"global_regs", "offload",
												"trigger_manager", "alpide_control",
												"alpide_data"};
};

#endif