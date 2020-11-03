#ifndef CMDPROCESSSERVER_HPP
#define CMDPROCESSSERVER_HPP 

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "ConnectionServer.hpp"
#include "CmdMessage.hpp"
#include "MasterHandler.hpp"
#include "SyncClient.hpp"

namespace salt_comm{

class CmdProcessServer : public ConnectionServer{

private:
	std::string masterIp_;
	CmdMessage cmdMessage_;
public:
	//construtor
	CmdProcessServer(std::string ip, unsigned short port, ServerType type, std::string masterIp);
	
	void listen();

	void handle_accept(const boost::system::error_code& e, MasterHandler* newMaster);
	


}; //end of class ConnectionServer 


}  //end of namespace salt_comm 

#endif
