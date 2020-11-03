#ifndef SYNCCLIENT_H
#define SYNCCLIENT_H


#include <boost/asio.hpp>
#include "Connection.hpp"
#include <boost/serialization/vector.hpp>
#include "SerializeTuple.hpp" 
#include "SyncMessage.hpp"
#include "ILocalSimController.hpp"


namespace salt_comm{

class SyncClient {

private:
	//the connection to the server
	Connection connection_;

	//server ip
	std::string remoteIp_;
	
	//server port
	int remotePort_;

	//local ip
	std::string localIp_;

	//server port
	int localPort_;


	SyncMessage syncMsg;
	MasterSyncRes masterSyncData_;
	
	ILocalSimController* simController_;

public:
	//constructore starts the asynchronous connection operation
	SyncClient (boost::asio::io_service& io_service, std::string remoteIp, int remotePort, std::string localIp, int localPort);

	void setLocalSimController(ILocalSimController* simController);

	//hanlde completionof a connection operation
	void handle_connect(const boost::system::error_code& e);
	void handle_complete(const boost::system::error_code& e);
	
	void sendWorkerSyncData(WorkerSyncReq workerSyncData);
	void handle_writeSyncMessage(const boost::system::error_code& e, WorkerSyncReq workerSyncData);
	void handle_writeSyncData(const boost::system::error_code& e);	
	void handle_readSyncData(const boost::system::error_code& e);
};

} //end of the namespace
#endif
