#ifndef VIS_CLIENT_H
#define VIS_CLIENT_H

#include <float.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/scoped_thread.hpp>
#include <Controller/SimulationController.h>
#include "VisMsg.h"

namespace libsalt {

    class VisClient {
    private:
        SALT::SimulationController* SC = nullptr;

        std::string remoteHost = "127.0.0.1";
        // visualization server's port
        int remotePort;
        VisClientState status = VisClientState::FINISHED;

//        boost::asio::io_service io;
//        boost::asio::io_service::strand strand;
        boost::asio::io_context io;
        boost::asio::io_context::strand strand;
        boost::scoped_ptr<boost::thread> io_thd;
        boost::asio::ip::tcp::socket socket;

        boost::asio::deadline_timer* statusTimer = nullptr;
        boost::asio::deadline_timer* dataTimer = nullptr;

        boost::scoped_ptr<boost::thread> stop_thd;

        unsigned int statusSleepTime = 1;
        unsigned int dataSleepTime = 1;

        // holds an inbound data
        char data[1024];
        std::string simulationId;

        float minx = 0;
        float miny = FLT_MAX;
        float maxx = FLT_MAX;
        float maxy = 0;
        VisRoadType roadType = LINK;

        static vector<string> tokenizeShape(const string& data, const string& dropped_delim = " ", const string& kept_delim = "");

    public:
        int interval = 1;

        // Starts asynchronous connection to VisServer
        VisClient(SALT::SimulationController* SC_, std::string& remoteHost_, int remotePort_);

        ~VisClient() {
            stop();
        }

        // start threads
        void start() {
            // std::cout << "Starting VisClient.start()" << std::endl;
            if (status == VisClientState::RUNNING) {
                return;
            }

            // io_service를 main thread 로부터 분리하기 위해서 별도의 thread 생성
//            io_thd.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, boost::ref(io))));
            io_thd.reset(new boost::thread(boost::bind(&boost::asio::io_context::run, boost::ref(io))));

            //boost::thread t(boost::bind(&boost::asio::io_context::run, &io));
            //th_service = boost::bind(&boost::asio::io_service::run, &service);

            //std::cout << "Started VisClient.start()" << std::endl;

            boost::asio::ip::tcp::resolver resolver(io);
            boost::asio::ip::tcp::resolver::query query(remoteHost, std::to_string(remotePort));
            boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

            //std::cout << "Connecting to VisServer: " << remoteHost << ":" << remotePort << std::endl;

            // Start an asynchronous connect operation
            //boost::asio::async_connect(socket, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), remotePort), boost::bind(&VisClient::handleConnection, this, boost::asio::placeholders::error));
            boost::asio::async_connect(socket, endpoint_iterator, boost::bind(&VisClient::handleConnection, this, boost::asio::placeholders::error));

            // Set status as running
            status = VisClientState::RUNNING; // RUNNING
        }

        void stop() {
            if (!io_thd) {
                return;
            }

            status = VisClientState::FINISHED; // FINISHED

            if (statusTimer != nullptr) {
                statusTimer->cancel();
                statusTimer->expires_from_now(boost::posix_time::seconds(0));
            }

            if (dataTimer != nullptr) {
                dataTimer->cancel();
                dataTimer->expires_from_now(boost::posix_time::seconds(0));
            }

            sendLastStatusMessage();

            // Wait for 3 second, so that last message is successfully sent to VisServer
            boost::this_thread::sleep(boost::posix_time::seconds(3));

            io.stop();
            io_thd->join();

            socket.close();
        }

        // handle completion of connection
        void handleConnection(const boost::system::error_code& e);

        // read messages: set, stop
        void read();

        // handle completion of async_read
        void handleRead(const boost::system::error_code& e, const size_t bytes_transferred);

        // send messages: init, status, data
        void sendInitMessage();
        void sendStatusMessage(const boost::system::error_code& e);
        void sendLastStatusMessage();
        void sendDataMessage(const boost::system::error_code& e);
        void sendDataMessageInternal();
        void stopSimulation();

        // handle completion of async_write
        void handleWrite(const boost::system::error_code& e, const size_t bytes_transferred, std::vector<boost::asio::const_buffer> buffers);

    }; // end of class VisClient

} // end of namespace libsalt
#endif // end of #define VIS_CLIENT_H
