#ifndef MASTERHANDLER_HPP
#define MASTERHANDLER_HPP

#include "Connection.hpp"
#include "CmdMessage.hpp"
#include "ILocalSimController.hpp"

namespace salt_comm{

class MasterHandler : public Connection {
					
private: 
	std::string masterIp_;
	CmdMessage cmdMsg_;
	SubScenarioMessage simScenarioInfo_;
	ILocalSimController* simController_;

public:
	MasterHandler(std::string masterIp, boost::asio::io_service& io_service);

	~MasterHandler();

	void setLocalSimController(ILocalSimController* simController);
	void readCommand();
	void readSimScenarioInfo();
	void handle_readCommand(const boost::system::error_code& e);
	void handle_readSimScenarioInfo(const boost::system::error_code& e);
	
		

//	std::string getMasterIP();
	

};

}
#endif
