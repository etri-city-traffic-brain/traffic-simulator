/****************************************************************************/
// C++ libsalt.link API implementation
/****************************************************************************/

#include <algorithm>
#include <json/json.h>
#include <Object/NetworkManager.h>
#include <utils/ParserSALT.h>

#include <libsalt/LibsaltDefs.h>
#include "Link.h"
#include "utils/config.h"
#include "Simulation.h"

namespace libsalt {

    // ===========================================================================
    // Getter Implementation
    // ===========================================================================

    std::vector<LibsaltLink>
    Link::getLinkList() {
        std::vector<LibsaltLink> links;
        std::vector<SALT::Link*> rs = Simulation::getNetworkManager()->getLinkList();
        for (SALT::Link* r : rs) {
            LibsaltLink link = LibsaltLink(r);
            links.push_back(link);
        }

        return links;
    }

    std::vector<LibsaltLink>
    Link::getValidLinkList() {
        std::vector<LibsaltLink> links;

        const auto rs = Simulation::getNetworkManager()->getValidLinkList();
        for (auto r: rs){
            LibsaltLink link = LibsaltLink(r);
            links.push_back(link);
        }

        return links;
    }

    LibsaltLink
    Link::getLink(const std::string& linkID) {
        std::vector<LibsaltLink> links = getLinkList();
        LibsaltLink link = std::find(links.begin(), links.end(), linkID).operator*();
        return link;
    }

    std::vector<LibsaltLane>
    Link::getLaneList(const std::string& linkID) {
        std::vector<LibsaltLane> lanes;
        int numLane = Simulation::getNetworkManager()->findLinkByID(linkID)->getNumLane();
        for (int i = 0; i < numLane; i++) {
            LibsaltLane lane = LibsaltLane(linkID, i);
            lanes.push_back(lane);
        }

        return lanes;
    }

    int
    Link::getLength(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getLength();
    }

    int
    Link::getNumLane(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getNumLane();
    }

    int
    Link::getNumSection(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getNumSection();
    }

    float
    Link::getSpeedLimit(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getMySpeedLimit();
    }

    float
    Link::getSumPassed(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getSumPassingCount();
    }

    float
    Link::getAverageWaitingQLength(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getAverageWaitingQLength();
    }

    float
    Link::getAverageWaitingTime(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getAverageWaitingTime();
    }

    float
    Link::getAverageDensity(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getAverageDensity();
    }

    float
    Link::getAverageSpeed(const std::string& linkID) {
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(linkID);
        int section = getNumSection(linkID);
        int lane = getNumLane(linkID);
        float sumSpeed = 0.0;
        float avgSpeed = 0.0;
        int numCounts = 0;
        for (int i = 0; i < section; i++) {
            for (int j = 0; j < lane; j++) {
                SALT::CellInterface* cell = link->getCellByIndex(i, j);
                float cellSpeed = cell->getAverageSpeed();
                if (cellSpeed != 0) {
                    sumSpeed += cellSpeed;
                    numCounts++;
                }
            }
        }

        // avgSpeed = sumSpeed / numCells;
        if (numCounts != 0) {
            avgSpeed = sumSpeed / numCounts;
        }
        return avgSpeed;
    }

    float
    Link::getAverageVehicleSpeed(const std::string& linkID) {
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(linkID);
        int section = getNumSection(linkID);
        int lane = getNumLane(linkID);
        float sumSpeed = 0.0;
        float avgSpeed = 0.0;
        int numCounts = 0;
        for (int i = 0; i < section; i++) {
            for (int j = 0; j < lane; j++) {
                SALT::CellInterface* cell = link->getCellByIndex(i, j);
                float cellSpeed = cell->getAverageVehicleSpeed();
                if (cellSpeed != 0) {
                    sumSpeed += cellSpeed;
                    numCounts++;
                }
            }
        }

        // avgSpeed = sumSpeed / numCells;
        if (numCounts != 0) {
            avgSpeed = sumSpeed / numCounts;
        }

    //    if (avgSpeed > 100.0) {
    //        std::cout << "linkID=" << linkID << ", avgSpeed=" << avgSpeed << ", counts=" << numCounts << std::endl;
    //    }

        return avgSpeed;
    }

    float
    Link::getSumTravelLength(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getSumTravelLength();
    }

    float
    Link::getSumTravelTime(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getSumTravelTime();
    }

    std::tuple<float, float, float>
    Link::computeWaitingVehicleInfo(const string &linkID, int currentStep, int lastSwitchingTime) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->computeVehWait(currentStep, lastSwitchingTime);
    }

    float
    Link::getAverageVehicleWaitingTime(const string &linkID, int currentStep, int lastSwitchingTime) {
        return get<1>(computeWaitingVehicleInfo(linkID, currentStep, lastSwitchingTime));
    }

    float
    Link::getAverageVehicleWaitingQLength(const string &linkID, int currentStep, int lastSwitchingTime) {
        return get<2>(computeWaitingVehicleInfo(linkID, currentStep, lastSwitchingTime));
    }

    float
    Link::getNumWaitingVehicle(const string &linkID, int currentStep, int lastSwitchingTime) {
        return get<0>(computeWaitingVehicleInfo(linkID, currentStep, lastSwitchingTime));
    }


    int Link::getStateOfWaitingVehicleOverTLSDuration(const string &linkID, int currentStep) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getStateOfWaitingVehOverTLSDuration(currentStep);
    }


    std::tuple<float, float> Link::getCurrentWaitingTimeBaseVeh(const string &linkID, int currentStep) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getWaitingTimeBaseVeh(currentStep);
    }

    float Link::getCurrentAverageWaitingTimeBaseVehicle(const string &linkID, int currentStep) {
        return get<0>(getCurrentWaitingTimeBaseVeh(linkID, currentStep));
    }

    float Link::getCurrentWaitingTimeSumBaseVehicle(const string &linkID, int currentStep) {
        return get<1>(getCurrentWaitingTimeBaseVeh(linkID, currentStep));
    }

    LibsaltCell
    Link::getCellByIndex(const std::string& linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane);
    }

    std::string
    Link::getTrafficSignalState(const std::string& linkID, const std::string& nextLinkID) {
        SALT::Link* nextLink = Simulation::getNetworkManager()->findLinkByID(nextLinkID);
        SALT::TrafficSignalState state = Simulation::getNetworkManager()->findLinkByID(linkID)->getTrafficSignalState(nextLink);
        std::string s;
        if (state == SALT::TrafficSignalState::GREEN) {
            s = "G";
        } else if (state == SALT::TrafficSignalState::WEAK_GREEN) {
            s = "g";
        } else if (state == SALT::TrafficSignalState::RED) {
            s = "r";
        } else {
            // YELLOW
            s = "y";
        }

        return s;
    }

    LibsaltConnection
    Link::getAnyConnectionTo(const std::string& linkID, const std::string& nextLinkID) {
        SALT::Link* nextLink = Simulation::getNetworkManager()->findLinkByID(nextLinkID);
        SALT::Connection* c = Simulation::getNetworkManager()->findLinkByID(linkID)->getAnyConnectionTo(nextLink);
        LibsaltConnection conn = LibsaltConnection(c);
        return conn;
    }

    LibsaltConnection
    Link::getConnectionTo(const std::string& linkID, const std::string& nextLinkID, int fromLane) {
        SALT::Link* nextLink = Simulation::getNetworkManager()->findLinkByID(nextLinkID);
        SALT::Connection* c = Simulation::getNetworkManager()->findLinkByID(linkID)->getConnectionTo(nextLink, fromLane);
        LibsaltConnection conn = LibsaltConnection(c);
        return conn;
    }

    bool
    Link::canReach(const std::string& linkID, const std::string& nextLinkID) {
        SALT::Link* nextLink = Simulation::getNetworkManager()->findLinkByID(nextLinkID);
        return Simulation::getNetworkManager()->findLinkByID(linkID)->canReach(nextLink);
    }

    LibsaltNode
    Link::getFromNode(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getFromNode();
    }

    LibsaltNode
    Link::getToNode(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getToNode();
    }

    std::string
    Link::getSpreadType(const std::string& linkID) {
        SALT::SpreadType type = Simulation::getNetworkManager()->findLinkByID(linkID)->getSpreadType();
        std::string s;
        if (type == SALT::SpreadType::CENTER) {
            s = "CENTER";
        } else {
            s = "RIGHT";
        }
        return s;
    }

    bool
    Link::isExceptionalLink(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->isExceptionalLink();
    }

    bool
    Link::isOutFlowLink(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->isOutFlowLink();
    }

    const::string
    Link::getShape(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getShape();
    }

    LibsaltPosition
    Link::getDirection(const std::string& linkID) {
        LibsaltPosition p;
        SALT::Vector2D direction = Simulation::getNetworkManager()->findLinkByID(linkID)->getDirection();
        p.x = direction.x;
        p.y = direction.y;

        return p;
    }

    int
    Link::getLeftPocket(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getMyLeftPocket();
    }

    int
    Link::getRightPocket(const std::string& linkID) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getMyRightPocket();
    }




    // ===========================================================================
    // Setter Implementation
    // ===========================================================================

} // end of namespace libsalt

/****************************************************************************/