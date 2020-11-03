#ifndef SERIALIZATION_CONNECTION_HPP
#define SERIALIZATION_CONNECTION_HPP


#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>



namespace salt_comm {

class Connection
{
protected:

	//connected server ip
	//std::string ip_;		

	//fixed length header
	enum { header_length = 8};

	// holds an outbound header	
	std::string outbound_header_;
	
	//hods the outbound data
	std::string outbound_data_;

	//holds an inbound header
	char inbound_header_[header_length];

	//holds an inbound data
	std::vector<char> inbound_data_;

public:

	//socket
	boost::asio::ip::tcp::socket socket_;
	
	//constructor
	Connection(boost::asio::io_service& io_service) : socket_(io_service) 
	{
		//std::cout << "### create Connection ... " << std::endl;

	}

	//get the socket
	boost::asio::ip::tcp::socket& socket() {
		return socket_;
	}

	
	virtual ~Connection() 
	{
			
	}	
	
	//asynchronously write a data structure to the socket
	template <typename T, typename Handler>
	void async_write(const T& t, Handler handler) {
		//serialize the data first so we know how large it is
		std::ostringstream archive_stream;

		boost::archive::text_oarchive archive(archive_stream);
		archive << t;

		outbound_data_ = archive_stream.str();
		//cout << "size: " << outboudn_data_.size() << ", data" << outbound_data_;

		//format the header
		std::ostringstream header_stream;
		header_stream << std::setw(header_length) << std::hex << outbound_data_.size();

	
		if(!header_stream || header_stream.str().size() != header_length) {
			//something went wrong, inform the caller
			boost::system::error_code error(boost::asio::error::invalid_argument);
			socket_.get_io_service().post(boost::bind(handler, error));
			return;
		}	
		outbound_header_ = header_stream.str();

		//write the serializd data to the socket
		//we use "gather-write" to send  both the header and the data in a single write operation
		std::vector<boost::asio::const_buffer> buffers;

		buffers.push_back(boost::asio::buffer(outbound_header_));
		buffers.push_back(boost::asio::buffer(outbound_data_));
		boost::asio::async_write(socket_, buffers, handler);	

	}


	//asynchronusly read a data structure form the socket
	template <typename T, typename Handler>
	void async_read(T& t, Handler handler) 
	{	
		//issue a read operation to read exactly the number of bytes in a header

		void (Connection::*f) (const boost::system::error_code&, T&, boost::tuple<Handler>) = &Connection::handle_read_header<T, Handler>;

		boost::asio::async_read(socket_, boost::asio::buffer(inbound_header_), 
			boost::bind(f, this, boost::asio::placeholders::error, boost::ref(t), boost::make_tuple(handler)));

	}	

	//handle a completd read of a message heder
	template <typename T, typename Handler>
	void handle_read_header(const boost::system::error_code& e, T& t, boost::tuple<Handler> handler)
	{
		if(e) {
			boost::get<0>(handler)(e);
		}
		else {
			//determine the length of the serialized data
			std::istringstream is(std::string(inbound_header_, header_length));
			std::size_t inbound_data_size = 0;

			if(!(is >> std::hex >> inbound_data_size)) {
				//header doesn't seem to be valid. inform the caller
				boost::system::error_code error(boost::asio::error::invalid_argument);
				boost::get<0>(handler) (error);
				return;	
			}


			//start an asynchronous call to receive the data
			inbound_data_.resize(inbound_data_size);
			void (Connection::*f) (const boost::system::error_code&, T&, boost::tuple<Handler>) = &Connection::handle_read_data<T, Handler>;

			boost::asio::async_read(socket_, boost::asio::buffer(inbound_data_),
				boost::bind(f, this, boost::asio::placeholders::error, boost::ref(t), handler));
			
		} 
	}


	//handle a completed read of mesage data
	template <typename T, typename Handler>
	void handle_read_data(const boost::system::error_code& e, T& t, boost::tuple<Handler> handler) 
	{
		if(e) {
			boost::get<0>(handler)(e);
		} else {

			//extract the data structure form the data just received
			try {
				std::string archive_data(&inbound_data_[0], inbound_data_.size());
				std::istringstream archive_stream(archive_data);

				boost::archive::text_iarchive archive(archive_stream);
				archive >> t;

			} catch (std::exception& e) {
				//unable to decode data
				boost::system::error_code error(boost::asio::error::invalid_argument);
				boost::get<0>(handler) (error);
				return;
			}
			//inform caller that data has been received ok.
			boost::get<0>(handler)(e);
		}


	}	
	
}; //end of class Connection


//typedef boost::shared_ptr<Connection> connection_ptr;

}//end of salt_comm 

#endif
