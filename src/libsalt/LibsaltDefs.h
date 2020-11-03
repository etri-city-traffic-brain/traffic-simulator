/****************************************************************************/
// C++ libsalt API implementation
/****************************************************************************/
#pragma once

#include <vector>
#include <limits>
#include <map>
#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <memory>

#include <utils/config.h>
#include <Object/Network/Node.h>
#include <Object/Network/Link.h>
#include <Object/Network/CellInterface.h>
#include <Object/Network/Connection.h>
#include <Object/Agent/VehicleInterface.h>
#include <Object/TrafficSignal/TrafficSignal.h>
#include <Object/TrafficSignal/TrafficSignalSchedule.h>

// ===========================================================================
// class and type definitions
// ===========================================================================
namespace libsalt {

    const double INVALID_DOUBLE_VALUE = -1073741824.0;
    const int INVALID_INT_VALUE = -1073741824;

/**
 * @class LibsaltException
 */
class LibsaltException : public std::runtime_error {
public:
    /** constructor */
    LibsaltException(std::string what)
            : std::runtime_error(what) {}
};

/// @name Structures definitions
/// @{

struct LibsaltResult {
    virtual ~LibsaltResult() {}
    virtual std::string toString() {
        return "";
    }
};

/**
 * @struct LibsaltPosition
 * @brief A 3D-position
 */
struct LibsaltPosition : LibsaltResult {
    std::string toString() {
        std::ostringstream os;
        os << "LibsaltPosition(" << x << "," << y << "," << z << ")";
        return os.str();
    }
    float x, y, z;
};

/** @struct LibsaltPositionVector
  * @brief A list of positions
  */
typedef std::vector<LibsaltPosition> LibsaltPositionVector;

struct LibsaltInt : LibsaltResult {
    LibsaltInt() : value(0) {}
    LibsaltInt(int v) : value(v) {}
    std::string toString() {
        std::ostringstream os;
        os << value;
        return os.str();
    }
    int value;
};


struct LibsaltDouble : LibsaltResult {
    LibsaltDouble() : value(0.) {}
    LibsaltDouble(double v) : value(v) {}
    std::string toString() {
        std::ostringstream os;
        os << value;
        return os.str();
    }
    double value;
};


struct LibsaltString : LibsaltResult {
    LibsaltString() : value("") {}
    LibsaltString(std::string v) : value(v) {}
    std::string toString() {
        return value;
    }
    std::string value;
};


struct LibsaltStringList : LibsaltResult {
    std::string toString() {
        std::ostringstream os;
        os << "[";
        for (std::string v : value) {
            os << v << ",";
        }
        os << "]";
        return os.str();
    }
    std::vector<std::string> value;
};

/// @brief {variable->value}
typedef std::map<int, std::shared_ptr<LibsaltResult> > LibsaltResults;

}

#ifdef SWIG
%template(TLSPhaseVector) std::vector<std::pair<int,std::string>>;
%template(TLSPhasePair) std::pair<int,std::string>;
#endif

namespace libsalt{
    class TLSSchedule {
    public:
        TLSSchedule(std::string _scheduleID, int _offset, std::vector<std::pair<int,std::string>> _phasevector) {
            myID = _scheduleID;
            myOffset = _offset;
            for (auto pv : _phasevector) {
                myPhaseVector.push_back(pv);
            }
        }

        virtual ~TLSSchedule() {}

        std::string myID;
        int myOffset; // Offset time
        // TLSPhasePair := pair<duration,state>
        std::vector<std::pair<int,std::string>> myPhaseVector;

        std::vector<std::pair<int,std::string>> getPhaseVector() {  return myPhaseVector; }
        std::string getSchedulID() { return myID; }
        int getOffset() { return myOffset; }
    };
}


#ifdef SWIG
%{
    namespace swig {
        template <>  struct traits<libsalt::TLSSchedule> {
        typedef pointer_category category;
        static const char* type_name() { return "TLSSchedule"; }
     };
   }

%}
%template(TLSSchedulesMap) std::map<std::string, libsalt::TLSSchedule *>;
%template(TLSTODMap) std::map<int, std::string>;
%template(LibsaltNodeVector) std::vector<libsalt::LibsaltNode>;
%template(LibsaltLinkVector) std::vector<libsalt::LibsaltLink>;
%template(LibsaltLaneVector) std::vector<libsalt::LibsaltLane>;
%template(LibsaltCellVector) std::vector<libsalt::LibsaltCell>;
%template(LibsaltConnectionVector) std::vector<libsalt::LibsaltConnection>;
%template(LibsaltVehicleVector) std::vector<libsalt::LibsaltVehicle>;
#endif


namespace libsalt {

    class TLSLogic {
    public:
        TLSLogic(std::string _nodeID, std::map<std::string,TLSSchedule*> _schedules, std::map<int,std::string> _todplan)
        : myTLSID(_nodeID), myScheduleMap(_schedules), myTODPlan(_todplan){}
        virtual ~TLSLogic() {}

        std::string myTLSID;

        // Time of Day Plan
        std::map<std::string, TLSSchedule*> myScheduleMap; // (schedule name, schedule itself)
        std::map<int, std::string> myTODPlan; //ToD Plan (schedule starting time, schedule name)

        std::string getTLSID() {  return myTLSID;  }
        std::map<std::string, TLSSchedule*> getScheduleMap() {
            return myScheduleMap;
        }

        std::map<int, std::string> getTODPlan() {
            return myTODPlan;
        }

    };

    // Intersection, Junction
    class LibsaltNode {
    public:
        bool operator==(const std::string& e) const {
            return (id == e);
        }

        LibsaltNode() {}
        LibsaltNode(const std::string& _id, const float _x, const float _y, const std::string& _type)
                : id(_id), x(_x), y(_y), type(_type) {}
        LibsaltNode(SALT::Node* node) {
            id = node->getID();
            x = node->getMyLocation().x;
            y = node->getMyLocation().y;
            type = node->getMyIntersectionType();
        }
        ~LibsaltNode() {}

        std::string id; // intersection's name
        float x;
        float y;
        std::string type; // intersection's type: priority, ...

        std::string toString() {
            std::ostringstream os;
            os << "LibsaltNode(name=" << id << ", pos=(" << x << "," << y << "), type=" << type << ")";
            return os.str();
        }

    };

    class LibsaltLink {
    public:
        bool operator==(const std::string& e) const {
            return (id == e);
        }

        LibsaltLink() {}
        LibsaltLink(SALT::Link* link) {
            id = link->getID();
            fromNode = link->getFromNode()->getID();
            toNode = link->getToNode()->getID();
            numLanes = link->getNumLane();
            shape = link->getShape();
            speedLimit = link->getMySpeedLimit();
            spreadType = link->getSpreadType();
            info = link->myInfo;
            len = link->getLength();
            leftPocket = link->getMyLeftPocket();
            rightPocket = link->getMyRightPocket();
        }
        ~LibsaltLink() {}

        std::string id;
        std::string fromNode;
        std::string toNode;
        int numLanes;
        std::string shape;
        float speedLimit;
        SALT::SpreadType spreadType;
        std::string info;
        float len;
        int leftPocket;
        int rightPocket;

        std::string toString(SALT::SpreadType type) {
            if (type == SALT::SpreadType::CENTER) {
                return "center";
            } else {
                return "right";
            }
        }

        std::string toString() {
            std::ostringstream os;
            os << "LibsaltLink(id=" << id << ", fromNode=" << fromNode << ", toNode=" << toNode << ", numLanes=" << numLanes << ", shape=" << shape << ", speedLimit=" << speedLimit << ", spreadType="
            << toString(spreadType) << ", info=" << info << ", len=" << len << ", leftPocket=" << leftPocket << ", rightPocket=" << rightPocket << ")";
            return os.str();
        }
    };

    class LibsaltConnection {
    public:
        LibsaltConnection() {}
        LibsaltConnection(SALT::Connection* c) {
            fromLink = c->getFromLink();
            toLink = c->getToLink();
            fromLane = c->getFromLane();
            toLane = c->getToLane();
            linkIndex = c->getLinkIndex();
            direction.x = c->getDirection().x;
            direction.y = c->getDirection().y;
            rotationDir = c->getRotationDir();
            info = c->myInfo;
        }
        ~LibsaltConnection() {}

        LibsaltLink fromLink;
        LibsaltLink toLink;
        int fromLane;
        int toLane;
        int linkIndex;
        LibsaltPosition direction;
        std::string rotationDir;
        std::string info;

        std::string toString() {
            std::ostringstream os;
            os << "LibsaltConnection(from=(" << fromLink.id << ":" << fromLane << "), to=(" << toLink.id << ":" << toLane <<
                "), direction=(" << direction.x << ", " << direction.y << "), rotationDir=" << rotationDir << ", linkIndex=" << linkIndex << ", info=" << info << ")";
            return os.str();
        }
    };

    class LibsaltLane {
    public:
        bool operator<(const LibsaltLane& e) const {
            return (id < e.id);
        }
        LibsaltLane() {}
        LibsaltLane(const std::string& laneID) {
            istringstream f(laneID);
            std::string s;
            getline(f, s, '_');
            link = s;
            getline(f, s, '_');
            lane = std::stoi(s);
        }
        LibsaltLane(const std::string& linkID, int lane) {
            id = linkID + "_" + std::to_string(lane);
            link = linkID;
            lane = lane;
        }
        ~LibsaltLane() {}

        std::string id;
        std::string link;
        int lane;

        std::string toString() {
            return id;
        }
    };

    class LibsaltCell {
    public:
        bool operator<(const LibsaltCell& e) const {
            return (id < e.id);
        }
        LibsaltCell() {}
        LibsaltCell(const std::string& cellID) {
            id = cellID;
            istringstream f(cellID);
            std::string s;
            getline(f, s, '_');
            link = s;
            getline(f, s, '_');
            section = std::stoi(s);
            getline(f, s, '_');
            lane = std::stoi(s);
        }
        LibsaltCell(const std::string& linkID, int section, int lane) {
            id = linkID + "_" + std::to_string(section) + "_" + std::to_string(lane);
            link = linkID;
            section = section;
            lane = lane;
        }
        LibsaltCell(SALT::CellInterface *r) {
            id = r->getID();
            link = r->getMyLink()->getID();
            section = r->getSection();
            lane = r->getLane();
        }
        ~LibsaltCell() {}

        std::string id;
        std::string link;
        int section;
        int lane;

        std::string toString() {
            return id;
        }
    };

    class LibsaltRoute {
    public:
        LibsaltRoute() {}
        LibsaltRoute(std::string routeString) {
            istringstream f(routeString);
            std::string s;
            while (getline(f, s, ' ')) {
                route.push_back(s);
            }
        }
        ~LibsaltRoute() {}

        std::vector<std::string> route;

        std::string toString() {
            std::ostringstream os;

            std::ostringstream str;
            for (int i = 0; i < route.size(); i++) {
                if (i == 0) {
                    str << route[i];
                } else {
                    str << " " << route[i];
                }
            }

            os << "LibsaltRoute(#links=" << route.size() << ", links=" << str.str() << ")";
            return os.str();
        }
    };

    class LibsaltVehicle {
    public:
        LibsaltVehicle() {}
        LibsaltVehicle(SALT::VehicleInterface* v) {
            id = v->getMyName();
            shape = v->getMyShape();
            speed = v->getMySpeed();
            SALT::VehicleClass clazz = v->getMyClass();
            if (clazz == SALT::BUS) {
                vClass = "bus";
            } else {
                vClass = "passenger";
            }
            departTime = v->getMyDepartTime();
            cellIndex = v->getMyCellIndex();
            length = v->getMyLength();
            route = v->getRouteString();
        }
        ~LibsaltVehicle() {}

        std::string id;
        std::string shape;
        float speed;
        std::string vClass;
        int departTime;
        int cellIndex;
        float length;
        std::string route;

        int routeIndex;

        std::string toString() {
            std::ostringstream os;
            os << "LibsaltVehicle(id=" << id << ", vClass=" << vClass << ", cellIndex=" << cellIndex << ", length=" << length << ", departTime=" << departTime << ", speed=" << speed << ", shape=" << shape << ", route=" << route << ")";
            return os.str();
        }
    };

} // end of namespace libsalt
