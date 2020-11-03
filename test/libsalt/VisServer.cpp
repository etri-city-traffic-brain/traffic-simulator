// import libraries
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "../../src/libsalt/VisMsg.h"

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;

class con_handler : public boost::enable_shared_from_this<con_handler> {
private:
    tcp::socket sock;

    enum { max_length = sizeof(DataMsg)};
    boost::array<char, max_length> data;

    bool inited = false;

    // Make message header
    struct MsgHeader makeHeader(unsigned int type) {
        struct MsgHeader header;
        header.type = type;
        auto unix_timestamp = std::chrono::seconds(std::time(NULL));
        long timestamp = std::chrono::milliseconds(unix_timestamp).count(); // unix timestamp in milliseconds
        header.timestamp = timestamp;

        return header;
    }

public:
    typedef boost::shared_ptr<con_handler> pointer;
    con_handler(boost::asio::io_service& io_service): sock(io_service) {}

    // creating the pointer
    static pointer create(boost::asio::io_service& io_service) {
        return pointer(new con_handler(io_service));
    }

    // socket creation
    tcp::socket& socket() {
        return sock;
    }

    void stop(void) {
        sock.close();
    }

    void read() {
        memset(&data, '\0', sizeof(data));

        boost::asio::async_read(sock, boost::asio::buffer(data),
                                boost::asio::transfer_exactly(sizeof(MsgHeader)),
                                boost::bind(&con_handler::handle_read,
                                            shared_from_this(),
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));

/*
        InitMsg init;
        init.header = makeHeader(VIS_INIT);
        strcpy(init.simulationId, "xx13579");
        int size = sizeof(init);
        boost::asio::async_write(sock,
                                 boost::asio::buffer(&init, size),
                             boost::bind(&con_handler::handle_write,shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred)
        );
*/
        // boost::this_thread::sleep(boost::posix_time::seconds(1));

/*
        StopMsg stop;
        stop.header = makeHeader(VIS_STOP);

        int stopSize = sizeof(stop);
        boost::asio::async_write(sock,
                                 boost::asio::buffer(&stop, stopSize),
                                 boost::bind(&con_handler::handle_write,shared_from_this(),
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred)
        );
*/

    }

    void handle_read(const boost::system::error_code& err, size_t bytes_transferred) {
        if (bytes_transferred > 0) {
            std::cout << __FUNCTION__ << "() Received MsgHeader, received=" << bytes_transferred << std::endl;
        }

        if (!err) {
            // Decode message type
            MsgHeader* header = reinterpret_cast<MsgHeader*>(&data);

            if (MSG_INIT == header->type) {
                // boost::array<char, sizeof(StatusMsg)> buf;
                std::vector<char> buf;
                buf.resize(header->bodySize);
                boost::system::error_code statusErr;
                size_t received_bytes = boost::asio::read(sock, boost::asio::buffer(buf), boost::asio::transfer_exactly(header->bodySize), statusErr);
                std::cout << __FUNCTION__ << "() Received INIT message, type=" << header->type << ", bodySize=" << header->bodySize << ", received_bytes=" << received_bytes << std::endl;

//                if (inited) {
//                    std::cerr << "ERROR: Received INIT message again" << std::endl;
//
//                    // Read next messages
//                    read();
//                }

//                inited = true;

                // Decode message
                typedef char charArray[];
                const charArray* simulationId = reinterpret_cast<charArray*>(&buf[0]);
                std::cout << "[InitMsg] timestamp=" << header->timestamp << ", simulationId=[" << *simulationId << "]" << std::endl;

                // test
                // sendSetMsg();
                // sendStopMsg();

            } else if (MSG_STATUS == header->type) {

                // boost::array<char, sizeof(StatusMsg)> buf;
                std::vector<char> buf;
                buf.resize(header->bodySize);
                boost::system::error_code statusErr;
                size_t received_bytes = boost::asio::read(sock, boost::asio::buffer(buf), boost::asio::transfer_exactly(header->bodySize), statusErr);
                std::cout << __FUNCTION__ << "() Received STATUS message, type=" << header->type << ", bodySize=" << header->bodySize << ", received_bytes=" << received_bytes << std::endl;

                // Decode message
                // const StatusMsg* msg = reinterpret_cast<const StatusMsg*>(buf.data());
                // std::cout << "[StatusMsg] timestamp=" << msg->header.timestamp << ", status=" << msg->status << ", progress=" << msg->progress << std::endl;

                const VisClientState* status = reinterpret_cast<VisClientState*>(&buf[0]);
                const unsigned int* progress = reinterpret_cast<unsigned int*>(&buf[sizeof(VisClientState)]);
                std::cout << "[StatusMsg] timestamp=" << header->timestamp << ", status=" << *status << ", progress=" << *progress << std::endl;

            } else if (MSG_DATA == header->type) {

                // boost::array<char, 100000> buf;
                std::vector<char> buf;
                buf.resize(header->bodySize);
                boost::system::error_code dataErr;
                // size_t received = boost::asio::read(sock, boost::asio::buffer(buf), boost::asio::transfer_exactly(header->size), dataErr);
                size_t received_bytes = boost::asio::read(sock, boost::asio::buffer(buf), boost::asio::transfer_exactly(sizeof(DataMsg) - sizeof(MsgHeader)), dataErr);
                std::cout << __FUNCTION__ << "() Received DATA message, type=" << header->type << ", bodySize=" << header->bodySize << ", received_bytes=" << received_bytes << std::endl;

                // Decode message
                // const DataMsg* msg = reinterpret_cast<const DataMsg*>(buf.data());
                const unsigned int* numRoads = reinterpret_cast<unsigned int*>(&buf[0]);

                typedef struct Road RoadArray[header->bodySize];
                const RoadArray* roadsPtr = reinterpret_cast<RoadArray*>(&buf[sizeof(unsigned int)]);
                auto roads = *roadsPtr;
                std::cout <<"[DataMsg] timestamp=" << header->timestamp << ", numRoads=" << *numRoads << std::endl;
                for (int i = 0; i < *numRoads; i++) {
                    Road road = roads[i];
                    std::string cls;
                    for (int j = 0; j < road.numVehicles; j++) {
                        VisVehicleClass c = road.vehicles[j];
                        if (c == VisVehicleClass::SMALL) {
                            cls += "s";
                        } else {
                            cls += "L";
                        }
                    }

                    std::string signal = "RED";
                    VisSignalType s = road.currentSignal;
                    if (s == VisSignalType::GREEN) {
                        signal = "GREEN";
                    }

                    if (i == 0 || i % 10 == 0 || i == *numRoads - 1) {
                        cout << (i+1) << "/" << *numRoads << ") roadId=" << std::string(road.roadId) << ", signal=" << signal << ", speed=" << road.speed << ", numVehicles=" << road.numVehicles << ", class=" << cls << endl;
                    }
                }
            } else {
                std::cout << __FUNCTION__ << "() Received UNKNOWN message, type=" << header->type << std::endl;
            }

        }
        else {
            if (err == boost::asio::error::eof) {
                std::cout << __FUNCTION__ << "() Client Disconnected" << std::endl;
                stop();
                return;
            } else {
                std::cerr << __FUNCTION__ << "() error: " << err.message() << std::endl;
            }
        }

        std::cout << __FUNCTION__ << "() Completed" << std::endl << std::endl;

        // Read next messages
        read();
    }

    void handle_write(const boost::system::error_code& err, size_t bytes_transferred) {
        if (!err) {
            cout << "Server sent message, size=" << bytes_transferred << endl;
        } else {
            std::cerr << __FUNCTION__ << "() error: " << err.message() << endl;
            sock.close();
        }
    }

    void sendSetMsg() {
        SetMsg msg = SetMsg();

        msg.header = MsgHeader();
        msg.header.type = MSG_SET;

        const auto p1 = std::chrono::system_clock::now();
        long timestamp = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
        msg.header.timestamp = timestamp;

        //msg.roadType = VisRoadType::LINK;
        msg.roadType = VisRoadType::CELL;
        msg.extend[0] = 100.0; // minx
        msg.extend[1] = 200.0; // miny
        msg.extend[2] = 200.0; // maxx
        msg.extend[3] = 100.0; // maxy

        msg.header.bodySize = sizeof(VisRoadType) + (sizeof(float) * EXTEND_LENGTH);
        int msgSize = sizeof(MsgHeader) + msg.header.bodySize;
        std::cout << "[SetMsg] roadType=" << msg.roadType << ", timestamp=" << msg.header.timestamp
                  << ", msgSize=" << msgSize << ", headerSize=" << sizeof(MsgHeader) << ", bodySize=" << msg.header.bodySize << std::endl;

        boost::asio::async_write(sock, boost::asio::buffer(&msg, msgSize), boost::bind(&con_handler::handle_write, shared_from_this(),
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));

    }

    void sendStopMsg() {
        StopMsg msg = StopMsg();

        msg.header = MsgHeader();
        msg.header.type = MSG_STOP;

        const auto p1 = std::chrono::system_clock::now();
        long timestamp = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
        msg.header.timestamp = timestamp;

        msg.header.bodySize = 0;
        int msgSize = sizeof(MsgHeader) + msg.header.bodySize;
        std::cout << "[StopMsg] timestamp=" << msg.header.timestamp << ", msgSize=" << msgSize << ", headerSize=" << sizeof(MsgHeader) << ", bodySize=" << msg.header.bodySize << std::endl;

        boost::asio::async_write(sock, boost::asio::buffer(&msg, msgSize), boost::bind(&con_handler::handle_write, shared_from_this(),
                                                                                       boost::asio::placeholders::error,
                                                                                       boost::asio::placeholders::bytes_transferred));
    }

};

class Server
{
private:
    tcp::acceptor acceptor_;
    boost::asio::io_service::strand strand;

    void accept() {
        // socket
        con_handler::pointer connection = con_handler::create(acceptor_.get_io_service());

        // asynchronous accept operation and wait for a new connection.
        acceptor_.async_accept(connection->socket(), strand.wrap(
            boost::bind(&Server::handle_accept, this, connection,
            boost::asio::placeholders::error)));
    }

public:

    // constructor for accepting connection from client
    Server(boost::asio::io_service& io_service, int port): acceptor_(io_service, tcp::endpoint(tcp::v4(), port)), strand(io_service) {
        accept();
    }

    void handle_accept(con_handler::pointer connection, const boost::system::error_code& err)
    {
        cout << __FUNCTION__ << "() CONNECT from VisClient" << endl << endl;
        if (!err) {
            connection->read();
        } else {
            std::cerr << "error: " << err.message() << endl;
        }
        accept();
    }
};

int
main(int argc, char *argv[]) {
    try {
        int port = 1337;
        boost::asio::io_service io_service;
        Server server(io_service, port);
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << endl;
    }
    return 0;
}
