#ifndef CONNECTIONSERVER_HPP
#define CONNECTIONSERVER_HPP


#include <boost/asio.hpp>

namespace salt_comm{

enum class ServerType{
	MASTER_SIDE_SERVER,
	WORKER_SIDE_SERVER
};

class ConnectionServer{

protected:

	//io service
	boost::asio::io_service io_service_;

	//strand
	boost::asio::io_service::strand strand_;

	//ip
	std::string localIp_;

	//port
	unsigned short port_;

	//the acceptor object for socket connection
	boost::asio::ip::tcp::acceptor acceptor_;

	


	//Server type
	const ServerType type_;

public:
	//construtor
	ConnectionServer(std::string ip, unsigned short port, ServerType type);

	virtual void startIOService();
	
	virtual void stopIOService();

	ServerType getType() const;

	boost::asio::io_service* getIOService();

	static std::string getLocalIP();

}; //end of class ConnectionServer 


}  //end of namespace salt_comm 

#endif
