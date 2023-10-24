#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/scoped_thread.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <utils/etc.h>
#include "VisClient.hpp"
#include "Link.h"
#include "Cell.h"
#include "Vehicle.h"
#include "VisMsg.h"

using boost::asio::ip::tcp;

namespace libsalt {

    /**
     * Constructor.
     */
    VisClient::VisClient(SALT::SimulationController* SC_, std::string& remoteHost_, int remotePort_) : SC(SC_), remoteHost(remoteHost_), remotePort(remotePort_), socket(io), strand(io) {
        statusTimer = new boost::asio::deadline_timer(io, boost::posix_time::seconds(statusSleepTime));
        statusTimer->async_wait(strand.wrap(boost::bind(&VisClient::sendStatusMessage, this, boost::asio::placeholders::error)));

        // Disabled for step-based data sending
        // dataTimer = new boost::asio::deadline_timer(io, boost::posix_time::seconds(dataSleepTime));
        // dataTimer->async_wait(strand.wrap(boost::bind(&VisClient::sendDataMessage, this, boost::asio::placeholders::error)));
    }

    /**
     * Handle connection operation.
     */
    void VisClient::handleConnection(const boost::system::error_code &e) {
        if (e) {
            // an error occurred
            std::cerr << __FUNCTION__ << "() error code : " << e.message() << std::endl;
            return;
        }

        // Successfully established connection
        std::cout << __FUNCTION__ << "() Connected to VisServer: " << remoteHost << ":" << std::to_string(remotePort) << std::endl;

        // Send init message
        sendInitMessage();

        // Read 1st message
        read();
    }

    /**
     * Asynchronously read a data structure from the socket
     */
    void VisClient::read() {
        // std::cout << __FUNCTION__ << "() Reading message from VisServer" << std::endl;

        // Read message from VisServer and store them in inBuf
        // Message is handled by handleReadComplete()
        memset(&data, '\0', sizeof(data));
        boost::asio::async_read(socket, boost::asio::buffer(data),
                                boost::asio::transfer_exactly(sizeof(MsgHeader)),
                                boost::bind(&VisClient::handleRead,
                                this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    /**
     * Handle completion of read message
     */
    void VisClient::handleRead(const boost::system::error_code& e, const size_t bytes_transferred) {
        if (e) {
            if (e.message() == "Operation canceled") {
                // ignore
                return;
            } else if (e == boost::asio::error::eof) {
                std::cout << __FUNCTION__ << "() Client Disconnected" << std::endl;
                stop();
                return;
            }

            // an error occurred
            std::cerr << __FUNCTION__ << "() error code : " << e.message() << std::endl;
            return;
        }

        if (bytes_transferred > 0) {
            std::cout << __FUNCTION__ << "() Received MsgHeader, received=" << bytes_transferred << std::endl;

            // Decode message type
            MsgHeader *header = reinterpret_cast<MsgHeader *>(&data);
            long timestamp = header->timestamp;
            unsigned int bodySize = header->bodySize;

            if (MSG_SET == header->type) {
                std::vector<char> buf;
                buf.resize(header->bodySize);
                boost::system::error_code statusErr;
                size_t received_bytes = boost::asio::read(socket, boost::asio::buffer(buf),
                                                          boost::asio::transfer_exactly(header->bodySize), statusErr);
                std::cout << __FUNCTION__ << "() Received SET message, type=" << header->type << ", bodySize="
                          << header->bodySize << ", received_bytes=" << received_bytes << std::endl;

                // Decode message
                typedef float floatArray[];
                const floatArray *extendPtr = reinterpret_cast<floatArray *>(&buf[0]);
                auto extend = *extendPtr;
                const VisRoadType *roadTypePtr = reinterpret_cast<VisRoadType *>(&buf[sizeof(float) * EXTEND_LENGTH]);
                roadType = *roadTypePtr;

                // Decode geometries of zoom area
                minx = extend[0];
                miny = extend[1];
                maxx = extend[2];
                maxy = extend[3];

                std::string roadTypeStr = "cell";
                if (*roadTypePtr == LINK) {
                    roadTypeStr = "link";
                }

                std::string extendStr =
                        to_string(minx) + "," + to_string(miny) + " " + to_string(maxx) + "," + to_string(maxy);
                cout << "[SetMsg] timestamp=" << timestamp << ", bodySize=" << bodySize << ", roadType=" << roadTypeStr << ", extend=" << extendStr << std::endl;

            } else if (MSG_STOP == header->type) {
                // std::cout << __FUNCTION__ << "() Received STOP message, type=" << header->type<< std::endl;
                cout << "[StopMsg] timestamp=" << timestamp << ", bodySize=" << bodySize << std::endl;

                // Stop simulation
                //boost::thread th1 = boost::thread(boost::bind(&VisClient::stopSimulation, &io));
                //th1.join();

                // std::thread t( [this]() {io.run(); } );
                // boost::thread t1(&VisClient::stopSimulation, &io);
                // std::thread t1(&VisClient::stop);
                // t1.join();
//                if (io_thd) {
//                    io.stop();
//                    io_thd->join();
//                }

                stop();
                // status = VisClientState::FINISHED;
            } else {
                // Error
                std::cout << __FUNCTION__ << "() Received UNKNOWN message, type=" << header->type << std::endl;
            }
        }

/*
        if (bytes_transferred > 0) {
            std::cout << __FUNCTION__ << "() Read message from VisServer, bytes_transferred=" << bytes_transferred << std::endl;

            // Decode message type
            unsigned int *ptrType = reinterpret_cast<unsigned int *>(&inBuf);
            unsigned int type = *ptrType;
            // std::cout << "Received message's type: " << type << std::endl;

            if (MSG_SET == type) {
                std::cout << __FUNCTION__ << "() Received SET message, type=" << type << std::endl;

                // Decode message
                const SetMsg *msg = reinterpret_cast<const SetMsg *>(inBuf);

                long timestamp = msg->header.timestamp;
                unsigned int bodySize = msg->header.bodySize;

                // Decode geometries of zoom area
                minx = msg->extend[0];
                miny = msg->extend[1];
                maxx = msg->extend[2];
                maxy = msg->extend[3];

                roadType = msg->roadType;
                std::string roadTypeStr = "cell";
                if (msg->roadType == LINK) {
                    roadTypeStr = "link";
                }

                std::string extend = to_string(minx) + "," + to_string(miny) + " " + to_string(maxx) + "," + to_string(maxy);
                cout << "[SetMsg] timestamp=" << timestamp << ", roadType=" << roadTypeStr << ", extend=" << extend << std::endl;

            } else if (MSG_STOP == type) {
                std::cout << __FUNCTION__ << "() Received STOP message, type=" << type << std::endl;

                // Decode message
                const StopMsg *msg = reinterpret_cast<const StopMsg *>(inBuf);

                cout << "[StopMsg] timestamp=" << msg->header.timestamp << std::endl;

                // Stop simulation
                stop();
            } else {
                // error
                std::cout << __FUNCTION__ << "() Received UNKNOWN message, type=" << type << std::endl;
            }

            std::cout << __FUNCTION__ << " READ messages from VisServer" << std::endl;
        }
*/

        // Read next message
        read();
    }

    /**
     * Send init message to VisServer.
     */
    void VisClient::sendInitMessage() {
        // std::cout << __FUNCTION__ << "() Starting " << std::endl;

        if (status == VisClientState::RUNNING) {
            InitMsg msg = InitMsg();
            msg.header = MsgHeader();
            msg.header.type = MSG_INIT;

            const auto p1 = std::chrono::system_clock::now();
            long timestamp = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
            msg.header.timestamp = timestamp;

            strncpy(msg.simulationId, SC->getSimID().c_str(), MAX_SIMULATION_ID);
            msg.header.bodySize = sizeof(msg.simulationId);

            // std::cout << "[InitMsg] timestamp=" << msg.header.timestamp << ", simulationId=[" << msg.simulationId << "]" << std::endl;
            std::cout << "[InitMsg] timestamp=" << msg.header.timestamp << ", simulationId=[" << SC->getSimID() << "]" << std::endl;

            std::vector<boost::asio::const_buffer> buffers;
            buffers.push_back(boost::asio::buffer(&msg, sizeof(MsgHeader) + msg.header.bodySize));

            boost::asio::async_write(socket, buffers,
                                     boost::bind(&VisClient::handleWrite, this,
                                                 boost::asio::placeholders::error,
                                                 boost::asio::placeholders::bytes_transferred, buffers));
        }
    }

    /**
     * Tokenize shape string to vector of string.
     */
    vector<string>
    VisClient::tokenizeShape(const std::string& data, const std::string& dropped_delim, const std::string& kept_delim) {
        // boost::char_separator<char> sep(dropped_delim.c_str(), kept_delim.c_str(), boost::keep_empty_tokens);
        boost::char_separator<char> sep(dropped_delim.c_str(), kept_delim.c_str(), boost::drop_empty_tokens);
        boost::tokenizer<boost::char_separator<char>> tokens(data, sep);

        vector<std::string> result { begin(tokens), end(tokens) };
        return result;
    }

    /**
     * Send periodic data message to VisServer
     */
    void VisClient::sendDataMessage(const boost::system::error_code& e) {
        if (e) {
            if (e.message() == "Operation canceled") {
                // ignore
                return;
            }

            // an error occurred
            std::cerr << __FUNCTION__ << "() error code : " << e.message() << std::endl;
            return;
        }

        try {
            sendDataMessageInternal();

            dataTimer->expires_at(dataTimer->expires_at() + boost::posix_time::seconds(dataSleepTime));
            dataTimer->async_wait(strand.wrap(boost::bind(&VisClient::sendDataMessage, this, boost::asio::placeholders::error)));
        } catch (bad_alloc &bad) {
            cout << "Not enough memory" << endl;
        } catch (exception &exp) {
            cout << "Exception: " << exp.what() << endl;
        }
    }

    void VisClient::sendDataMessageInternal() {
        if (status == VisClientState::RUNNING) {

            unsigned int msgSize = 0;
            unsigned int roadsSize = 0;

            DataMsg msg = DataMsg();
            msg.header = MsgHeader();
            msg.header.type = MSG_DATA;

            const auto p1 = std::chrono::system_clock::now();
            long timestamp = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
            msg.header.timestamp = timestamp;

            msgSize += sizeof(MsgHeader);

            int numRoads = 0;
            std::vector<Road> roads;
            if (roadType == VisRoadType::LINK) {
                std::vector<SALT::Link *> links = SC->getLinkList();
                for (auto link : links) {
                    std::vector<std::string> vehicles = link->getVehicles();
                    int numVehicles = vehicles.size();
                    if (numVehicles == 0) {
                        continue;
                    }

                    // Get visualization geometry
                    float road_minx, road_miny, road_maxx, road_maxy;
                    {
                        std::vector<std::string> shapes;
                        // link's shape = myFromNode->getShape() + " " + myToNode->getShape();
                        // node's shape = std::to_string(x) + "," + std::to_string(y);
                        std::string shapeStr = link->getShape();
                        std::string delimiters(" ,"); // space and comma
                        boost::split(shapes, shapeStr, boost::is_any_of(delimiters));

                        // std::cout << shapes[0] << "|" << shapes[1] << "|" << shapes[shapes.size()-2] << "|" << shapes[shapes.size()-1] << std::endl;
                        road_minx = boost::lexical_cast<float>(shapes[0]);
                        road_miny = boost::lexical_cast<float>(shapes[1]);
                        road_maxx = boost::lexical_cast<float>(shapes[shapes.size() - 2]);
                        road_maxy = boost::lexical_cast<float>(shapes[shapes.size() - 1]);
                        //std::cout << "shape=" << link->getShape() << "  ==>  (" << road_minx << "," << road_miny << " " << road_maxx << "," << road_maxy << ")" << std::endl;
                    }

                    // Send only the link contained inside the geometry set by VisServer
                    if ((road_minx >= minx) && (road_miny <= miny) && (road_maxx <= maxx) && (road_maxy > maxy)) {
                        Road road = Road();

                        strncpy(road.roadId, link->getID().c_str(), MAX_ROAD_ID);
                        msgSize += sizeof(road.roadId);
                        roadsSize += sizeof(road.roadId);
                        //std::cout << "roadId=" << road.roadId << ", size=" << sizeof(road.roadId) << std::endl;

                        // =========================
                        // Get link's traffic signal
                        // =========================
                        std::vector<SALT::Link*> nextLinks = link->getNextLinks();
                        road.currentSignal = VisSignalType::RED;
                        for (auto nextLink : nextLinks) {
                            // rotationDir: s(straight), l(left), r(right)
                            std::string rotationDir = link->computeConnectionRotation(*nextLink);
                            // cout << "rotationDir=" << rotationDir << endl;

                            // Do not consider right direction
                            if (rotationDir != "r") {
                                SALT::TrafficSignalState state = link->getTrafficSignalState(nextLink);
                                // if one of the signals is GREEN, link's signal is GREEN
                                if (state == SALT::TrafficSignalState::GREEN) {
                                    road.currentSignal = VisSignalType::GREEN;
                                    break;
                                }
                            }
                        }

                        msgSize += sizeof(road.currentSignal);
                        roadsSize += sizeof(road.currentSignal);

                        road.numVehicles = numVehicles;

                        msgSize += sizeof(road.numVehicles);
                        roadsSize += sizeof(road.numVehicles);

                        // ===================================================
                        // Calculate size of vehicles
                        // ---------------------------------------------------
                        // 1. variable size
                        //road.vehicles = static_cast<VisVehicleClass *>(malloc(road.numVehicles * sizeof(VisVehicleClass)));
                        //msgSize += road.numVehicles * sizeof(VisVehicleClass);
                        //roadsSize += road.numVehicles * sizeof(VisVehicleClass);
                        // 2. fixed size
                        msgSize += sizeof(road.vehicles);
                        roadsSize += sizeof(road.vehicles);
                        // ===================================================

                        //cout << "numVehicles=" << numVehicles << ", vehicles.size()=" << vehicles.size() << endl;
                        for (int i = 0; i < numVehicles; i++) {
                            std::string vehicle = vehicles[i];
                            std::string vClass = Vehicle::getClass(vehicle);
                            // cout << "vehicles[" << i+1 << "/" << numVehicles << "]=" << vehicles[i] << ", vclass=" << vClass << endl;
                            if (vClass.compare("bus") == 0) {
                                road.vehicles[i] = VisVehicleClass::LARGE;
                            } else {
                                road.vehicles[i] = VisVehicleClass::SMALL;
                            }
                        }

                        road.speed = (unsigned int)Link::getAverageVehicleSpeed(road.roadId);
                        msgSize += sizeof(road.speed);
                        roadsSize += sizeof(road.speed);
                        // std::cout << "roadId=" << road.roadId << ", currentSignal=" << road.currentSignal << ", numVehicles=" << road.numVehicles << ", speed=" << road.speed << std::endl;

                        roads.push_back(road);
                        numRoads++;

                        // TEST: only 3 roads
//                            if (numRoads == 3) {
//                                break;
//                            }
                    }
                }
            } else {
                // roadType == CELL
                std::vector<SALT::CellInterface *> cells = SC->getCellList();
                for (SALT::CellInterface* cell : cells) {
                    std::vector<std::string> vehicles = cell->getVehicles();
                    int numVehicles = vehicles.size();
                    if (numVehicles == 0) {
                        continue;
                    }

                    // Get visualization geometry
                    float road_minx, road_miny, road_maxx, road_maxy;
                    {
                        std::vector<std::string> shapes;
                        // Use link's shape
                        // link's shape = myFromNode->getShape() + " " + myToNode->getShape();
                        // node's shape = std::to_string(x) + "," + std::to_string(y);
                        std::string shapeStr = cell->getMyLink()->getShape();
                        std::string delimiters(" ,"); // space and comma
                        boost::split(shapes, shapeStr, boost::is_any_of(delimiters));

                        // std::cout << shapes[0] << "|" << shapes[1] << "|" << shapes[shapes.size()-2] << "|" << shapes[shapes.size()-1] << std::endl;
                        // shape: upper left(minx, miny) -> lower right(maxx, maxy)
                        road_minx = boost::lexical_cast<float>(shapes[0]);
                        road_miny = boost::lexical_cast<float>(shapes[1]);
                        road_maxx = boost::lexical_cast<float>(shapes[shapes.size() - 2]);
                        road_maxy = boost::lexical_cast<float>(shapes[shapes.size() - 1]);
                    }

                    // Send only the cells contained inside the geometry set by VisServer
                    if ((road_minx >= minx) && (road_miny <= miny) && (road_maxx <= maxx) && (road_maxy >= maxy)) {
                        Road road = Road();

                        strncpy(road.roadId, cell->getID().c_str(), MAX_ROAD_ID);
                        msgSize += sizeof(road.roadId);
                        roadsSize += sizeof(road.roadId);

                        road.currentSignal = VisSignalType::GREEN;

                        // =========================
                        // Get cell's traffic signal
                        // =========================
                        SALT::Link* link = cell->getMyLink();
                        std::vector<SALT::Link*> nextLinks = link->getNextLinks();
                        // Default is RED
                        road.currentSignal = VisSignalType::RED;
                        for (auto nextLink : nextLinks) {
                            SALT::Connection* conn = link->getConnectionTo(nextLink, cell->getLane());
                            if (conn != nullptr) {
                                SALT::TrafficSignalState state = link->getTrafficSignalState(nextLink);
                                // if one of the signals is GREEN, link's signal is GREEN
                                if (state == SALT::TrafficSignalState::GREEN) {
                                    road.currentSignal = VisSignalType::GREEN;
                                    break;
                                }
                            }
                        }

                        msgSize += sizeof(road.currentSignal);
                        roadsSize += sizeof(road.currentSignal);

                        road.numVehicles = numVehicles;

                        msgSize += sizeof(road.numVehicles);
                        roadsSize += sizeof(road.numVehicles);

                        // ===================================================
                        // Calculate size of vehicles
                        // ---------------------------------------------------
                        // 1. variable size
                        // road.vehicles = static_cast<VisVehicleClass *>(malloc(road.numVehicles * sizeof(VisVehicleClass)));
                        // msgSize += road.numVehicles * sizeof(VisVehicleClass);
                        // 2. fixed size
                        msgSize += sizeof(road.vehicles);
                        // ===================================================

                        for (int i = 0; i < numVehicles; i++) {
                            std::string vehicle = vehicles[i];
                            std::string vClass = Vehicle::getClass(vehicle);
                            if (vClass.compare("bus") == 0) {
                                road.vehicles[i] = VisVehicleClass::LARGE;
                            } else {
                                road.vehicles[i] = VisVehicleClass::SMALL;
                            }
                        }

                        road.speed = (unsigned int) Cell::getAverageVehicleSpeed(std::string(road.roadId));
                        msgSize += sizeof(road.speed);
                        roadsSize += sizeof(road.speed);
                        // std::cout << "roadId=" << road.roadId << ", currentSignal=" << road.currentSignal << ", numVehicles=" << road.numVehicles << ", speed=" << road.speed << std::endl;

                        roads.push_back(road);
                        numRoads++;
                    }
                }
            }

            if (numRoads > 0) {
                msg.numRoads = numRoads;
                msgSize += sizeof(msg.numRoads);

                // msg.roads = static_cast<struct Road *>(malloc(roadsSize));
                // cout << "msgSize=" << msgSize << ", roadsSize=" << roadsSize << endl;
                for (int i = 0; i < numRoads; i++) {
                    msg.roads[i] = roads[i];
/*
                            Road r = msg.roads[i];
                            std::string cls;
                            for (int j = 0; j < r.numVehicles; j++) {
                                VisVehicleClass c = r.vehicles[j];
                                if (c == VisVehicleClass::SMALL) {
                                    cls += "s";
                                } else {
                                    cls += "L";
                                }
                            }

                            cout << "[" << r.roadId << "] numVehicles=" << r.numVehicles << ", speed=" << r.speed << ", class=(" << cls << ")" << endl;
*/
                }

                msg.header.bodySize = msgSize - sizeof(MsgHeader);

                std::cout << "[DataMsg] roadType=" << roadType << ", timestamp=" << msg.header.timestamp
                          << ", numRoads=" << msg.numRoads << ", msgSize=" << msgSize << ", headerSize=" << sizeof(MsgHeader) << ", bodySize=" << msg.header.bodySize << std::endl;

                std::vector<boost::asio::const_buffer> buffers;
                buffers.push_back(boost::asio::buffer(&msg, msgSize));
                boost::asio::async_write(socket, buffers, boost::bind(&VisClient::handleWrite, this,
                                                                      boost::asio::placeholders::error,
                                                                      boost::asio::placeholders::bytes_transferred, buffers));
            }
        }

    }

    /**
     * Send periodic status message to VisServer.
     */
    void VisClient::sendStatusMessage(const boost::system::error_code& e) {
        if (e) {
            if (e.message() == "Operation canceled") {
                // ignore
                return;
            }

            // an error occurred
            std::cerr << __FUNCTION__ << "() error code : " << e.message() << std::endl;
            return;
        }

        // std::cout << __FUNCTION__ << "() Starting " << std::endl;

        try {
            if (status == VisClientState::RUNNING) {
                if (SC->getProgress() >= 0) {
                    StatusMsg msg = StatusMsg();
                    msg.header = MsgHeader();
                    msg.header.type = MSG_STATUS;

                    const auto p1 = std::chrono::system_clock::now();
                    long timestamp = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
                    msg.header.timestamp = timestamp;

                    msg.status = status;
                    msg.progress = (unsigned int) SC->getProgress();

                    msg.header.bodySize = sizeof(VisClientState) + sizeof(unsigned int);

                    // std::cout << "msg.size=" << sizeof(msg) << ", header.size=" << sizeof(msg.header) << ", header.type.size=" << sizeof(msg.header.type) << ", header.timestamp.size=" << sizeof(msg.header.timestamp) << ", msg.status.size=" << sizeof(msg.status) << ", msg.progress.size=" << sizeof(msg.progress) << std::endl;

                    std::cout << "[StatusMsg] timestamp=" << msg.header.timestamp << ", status=" << msg.status
                              << ", progress=" << msg.progress << std::endl;

                    std::vector<boost::asio::const_buffer> buffers;
                    buffers.push_back(boost::asio::buffer(&msg, sizeof(MsgHeader) + msg.header.bodySize));

                    boost::asio::async_write(socket, buffers,
                                             boost::bind(&VisClient::handleWrite, this,
                                                         boost::asio::placeholders::error,
                                                         boost::asio::placeholders::bytes_transferred, buffers));
                }
            }

            statusTimer->expires_at(statusTimer->expires_at() + boost::posix_time::seconds(statusSleepTime));
            statusTimer->async_wait(strand.wrap(boost::bind(&VisClient::sendStatusMessage, this, boost::asio::placeholders::error)));
        } catch (bad_alloc &bad) {
            cout << "Not enough memory" << endl;
        } catch (exception &exp) {
            cout << "Exception: " << exp.what() << endl;
        }
    }

//    // Stop simulation
//    void VisClient::stopSimulation() {
//        if (status == VisClientState::FINISHED) {
//            cout << this_thread::get_id() << ": Stopping Simulation" << endl;
//            stop();
//        }
//
//        try {
//            stopTimer->expires_at(stopTimer->expires_at() + boost::posix_time::seconds(1));
//            stopTimer->async_wait(strand.wrap(boost::bind(&VisClient::stopSimulation, this)));
//        } catch (bad_alloc &bad) {
//            cout << "Not enough memory" << endl;
//        } catch (exception &exp) {
//            cout << "Exception: " << exp.what() << endl;
//        }
//    }

    /**
     * Send last status message to VisServer.
     */
    void VisClient::sendLastStatusMessage() {
        StatusMsg msg = StatusMsg();
        msg.header = MsgHeader();
        msg.header.type = MSG_STATUS;

        const auto p1 = std::chrono::system_clock::now();
        long timestamp = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
        msg.header.timestamp = timestamp;

        msg.status = status;
        msg.progress = (unsigned int) SC->getProgress();

        msg.header.bodySize = sizeof(VisClientState) + sizeof(unsigned int);

        // std::cout << "msg.size=" << sizeof(msg) << ", header.size=" << sizeof(msg.header) << ", header.type.size=" << sizeof(msg.header.type) << ", header.timestamp.size=" << sizeof(msg.header.timestamp) << ", msg.status.size=" << sizeof(msg.status) << ", msg.progress.size=" << sizeof(msg.progress) << std::endl;

        std::cout << "[StatusMsg] timestamp=" << msg.header.timestamp << ", status=" << msg.status
                  << ", progress=" << msg.progress << std::endl;

        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::buffer(&msg, sizeof(MsgHeader) + msg.header.bodySize));

        boost::asio::async_write(socket, buffers,
                                 boost::bind(&VisClient::handleWrite, this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred, buffers));
    }

    /**
     * Handle write message.
     */
    void VisClient::handleWrite(const boost::system::error_code& e, const size_t bytes_transferred, std::vector<boost::asio::const_buffer> buffers) {
        //std::cout << __FUNCTION__ << "() SENT message to VisServer: " << bytes_transferred << " bytes" << std::endl;
        buffers.clear();
    }

} // end of the namespace
