/****************************************************************************/
// C++ libsalt.lane API implementation
/****************************************************************************/

#include <json/json.h>
#include <Object/NetworkManager.h>
#include <Object/Network/Link.h>
#include <utils/ParserSALT.h>

#include <libsalt/LibsaltDefs.h>
#include "Lane.h"
#include "Simulation.h"

namespace libsalt {

// ===========================================================================
// Getter Implementation
// ===========================================================================

    int
    Lane::getLength(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();

        int length = 0;
        for (int i = 0; i < numSections; i++) {
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            length += cell->getMyLength();
        }
        return length;
    }

    float
    Lane::getSpeedLimit(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        // SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);

        return Simulation::getNetworkManager()->findLinkByID(lane.link)->getMySpeedLimit();
    }

    float
    Lane::getAverageSpeed(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();

        float avgSpeed = 0.0;
        float sumSpeed = 0;
        int numCounts = 0;
        for (int i = 0; i < numSections; i++) {
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            float cellSpeed = cell->getAverageSpeed();
            if (cellSpeed != 0) {
                sumSpeed += cellSpeed;
                numCounts++;
            }
        }

        // return avgSpeed / numSections;
        if (numCounts != 0) {
            avgSpeed = sumSpeed / numCounts;
        }

        return avgSpeed;
    }

    float
    Lane::getAverageVehicleSpeed(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();

        float avgSpeed = 0.0;
        float sumSpeed = 0;
        float numCounts = 0;
        for (int i = 0; i < numSections; i++) {
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            float cellSpeed = cell->getAverageVehicleSpeed();
            if (cellSpeed != 0) {
                sumSpeed += cellSpeed;
                numCounts++;
            }
        }

        if (numCounts != 0) {
            avgSpeed = sumSpeed / numCounts;
        }

        return avgSpeed;
    }

    int
    Lane::getNumVehPassed(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();

        int numVehPassed = 0;
        for (int i = 0; i < numSections; i++) {
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            numVehPassed += cell->getNumVehPassed();
        }
        return numVehPassed;
    }

    float
    Lane::getAverageNumVehs(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();

        int avgNumVehs = 0;

        for (int i = 0; i < numSections; i++) {
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            avgNumVehs += cell->getAverageNumVehs();
        }
        return avgNumVehs;
    }

    float
    Lane::getAverageWaitingQLength(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();

        float avgWQLen = 0;
        for (int i = 0; i < numSections; i++) {
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            avgWQLen += cell->getAverageWaitingQLength();
        }
        return avgWQLen;
    }

    float
    Lane::getAverageWaitingTime(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();

        float avgWQTime = 0;
        for (int i = 0; i < numSections; i++) {
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            avgWQTime += cell->getAverageWaitingTime();
        }
        return avgWQTime / numSections;
    }

    float Lane::getAverageDensity(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();

        float avgDensity = 0;
        for (int i = 0; i < numSections; i++) {
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            avgDensity += cell->getAverageDensity();
        }
        return avgDensity / numSections;
    }

    float Lane::getSumTravelLength(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();

        float sumTravelLength = 0;
        for (int i = 0; i < numSections; i++) {
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            sumTravelLength += cell->getSumTravelLength();
        }
        return sumTravelLength;
    }

    float Lane::getSumTravelTime(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();

        float sumTravelTime = 0;
        for (int i = 0; i < numSections; i++) {
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            sumTravelTime += cell->getSumTravelLength();
        }
        return sumTravelTime;
    }

    int Lane::getCurrentVolume(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();

        int currentVolume = 0;
        for (int i = 0; i < numSections; i++) {
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            currentVolume += cell->getMyCurrentVolume();
        }
        return currentVolume;
    }

    int Lane::getCurrentRoom(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();

        int currentRoom = 0;
        for (int i = 0; i < numSections; i++) {
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            currentRoom += cell->getCurrentRoom();
        }
        return currentRoom;
    }

    int Lane::getActivationCount(const std::string& laneID) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();

        int activationCount = 0;
        for (int i = 0; i < numSections; i++) {
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            activationCount += cell->getMyActivationCount();
        }
        return activationCount;
    }

// ===========================================================================
// Setter Implementation
// ===========================================================================

}

/****************************************************************************/