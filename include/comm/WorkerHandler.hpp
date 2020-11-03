#ifndef WORKERHANDLER_HPP
#define WORKERHANDLER_HPP

#include <memory>
#include "Connection.hpp"
#include <boost/serialization/vector.hpp>
#include "SerializeTuple.hpp"
#include "CmdSender.hpp"
#include "SyncMessage.hpp"

namespace salt_comm{

enum WorkerState {
	UNREADY,
	READY
};

class MasterCommunicator;

class WorkerHandler : public Connection {
					
private: 
	//related class 
	MasterCommunicator* mCommunicator_;
	
	WorkerState state_;

	std::string workerIp_;
	int workerPort_;

	CmdSender* cmdSender_;
	
	SyncMessage  syncMsg_;

	WorkerSyncReq	workerSyncData_;

	void handle_readSyncMessage(const boost::system::error_code& e);	
	void readWorkerSyncData();
	void handle_readWorkerSyncData(const boost::system::error_code& e);
	void handle_sendMasterSyncData(const boost::system::error_code& e);
public:
	WorkerHandler(boost::asio::io_service& io_service);

	~WorkerHandler();

	WorkerState getState();
	void changeStateToReady();
	void changeStateToUnready();

	void setCmdSender(CmdSender* sender); 
	void setMasterCommunicator(MasterCommunicator* mcommunicator);
	void setWorkerIPandPort(std::string ip, int port);

	std::string getWorkerIP() { return workerIp_; }
	int getWorkerPort() { return workerPort_; }
	void sendStartSimCommand(SubScenarioMessage s);
	void sendStopSimCommand(std::string simId, long timeStep);
	void sendPauseSimCommand(std::string simId, long timeStep);
	void sendResumeSimCommand(std::string simId);
	void sendTerminateWorkerCommand();
	
	//function to send and receive sync messsges
	void receiveWorkerSyncData();
	void sendMasterSyncData(MasterSyncRes masterSyncData);

	// function to clear
	void clearWorkerSyncData();
};

typedef std::shared_ptr<WorkerHandler> WorkerHandler_ptr;

} //end of namespace salt_comm
#endif
