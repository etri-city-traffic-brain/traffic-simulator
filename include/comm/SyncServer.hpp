#ifndef SYNCSERVER_HPP
#define SYNCSERVER_HPP 

#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "SyncMessage.hpp"
#include "ConnectionServer.hpp"
#include "WorkerManager.hpp"
#include "WorkerHandler.hpp"

#define IO_THREAD_NUM 4

namespace salt_comm{

class SyncServer : public ConnectionServer{

private:

	WorkerManager* WM_;

	SyncMessage syncMsg_; 

//	std::string workerPort_; // worker port is not fixed -> syncMsg has worker's port --> delete workerPort attr

	
	boost::thread_group io_threads_;	
	
public:
	//construtor
	SyncServer(std::string ip, unsigned short port, ServerType type);
	

	void listen();

	void handle_accept(const boost::system::error_code& e, WorkerHandler_ptr worker);

	void handle_read(const boost::system::error_code& e, WorkerHandler_ptr worker);


	//overriding
	void startIOService();
	void stopIOService();


}; //end of class SyncServer 


}  //end of namespace salt_comm 

#endif
