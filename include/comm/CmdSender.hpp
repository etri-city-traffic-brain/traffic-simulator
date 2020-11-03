#ifndef CMDSENDER_HPP
#define CMDSENDER_HPP 

#include <boost/asio.hpp>
#include <memory>

#include "CmdMessage.hpp"
#include "Connection.hpp"

namespace salt_comm{


class WorkerHandler;
//class MasterCommunicator;

class CmdSender {

private:

	//related MasterCommunicator
	//MasterCommunicator* mCommunicator_;

	//related workerhandler
	WorkerHandler* worker_;

	//the connection to the server
	Connection connection_;

	//worker ip
	std::string workerIp_; 

	//worker port
	int workerPort_;

	//local ip
	//std::string localIp_;

	CmdMessage c_;
	SubScenarioMessage sInfo_;

	//handle completionof a connection operation
	void handle_connect(const boost::system::error_code& e);

	//handle completion of a start_sim message 
	void handle_startSim(const boost::system::error_code& e, SubScenarioMessage s);

	//handle completion of a operation 
	void handle_error(const boost::system::error_code& e);


public:
	//constructore starts the asynchronous connection operation
	CmdSender (boost::asio::io_service& io_service, std::string workerIp, int workerPort, WorkerHandler* worker);
	//CmdSender(boost::asio::io_service& io_service, std::string workerIp, std::string workerPort);
	~CmdSender();

	//handle completin of a read operation
	void sendStartSimMsg(CmdMessage c, SubScenarioMessage s);
	void sendCmdMsg(CmdMessage c);


};

} //end of the namespace
#endif
