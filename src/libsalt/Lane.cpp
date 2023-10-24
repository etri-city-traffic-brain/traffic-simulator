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
        int lastSectionIdx = numSections-1;
        SALT::CellInterface* cell = link->getCellByIndex(lastSectionIdx, lane.lane);

        return cell->getNumVehPassed();
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

    std::tuple<float, float, float>
    Lane::computeWaitingVehicleInfo(const std::string& laneID, int currentStep, int lastSwitchingTime) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();
        float numWaitingVeh = 0.0f;
        float avgWaitingTime = 0.0f;
        float avgWaitingQLength = 0.0f;
        for(int i = 0; i < numSections; i++){
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            avgWaitingTime += get<1>(cell->computeWaitingVehicle(currentStep, lastSwitchingTime));
            avgWaitingQLength += get<2>(cell->computeWaitingVehicle(currentStep, lastSwitchingTime));
            if(i == numSections-1) {
                numWaitingVeh += get<0>(cell->computeWaitingVehicle(currentStep, lastSwitchingTime));
            }
        }

        return make_tuple(numWaitingVeh, (avgWaitingTime/float(numSections)), (avgWaitingQLength/float(numSections)));
    }


    std::tuple<float, float> Lane::getCurrentWaitingTimeBaseVeh(const std::string& laneID, int currentStep) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();
        float avgWaitingTime = 0.0f;
        float sumWaitingTime = 0.0f;
        for(int i = 0; i < numSections; i++){
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            avgWaitingTime += get<0>(cell->getVehicleWaitingTimeBaseVeh(currentStep));
            sumWaitingTime += get<1>(cell->getVehicleWaitingTimeBaseVeh(currentStep));
        }

        return make_tuple(avgWaitingTime / float(numSections), sumWaitingTime);
    }

    float Lane::getAverageVehicleWaitingTime(const std::string& laneID, int currentStep, int lastSwitchingTime) {
        return get<1>(computeWaitingVehicleInfo(laneID, currentStep,lastSwitchingTime));
    }

    float
    Lane::getAverageVehicleWaitingQLength(const std::string& laneID, int currentStep, int lastSwitchingTime) {
        return get<2>(computeWaitingVehicleInfo(laneID, currentStep,lastSwitchingTime));
    }

    float Lane::genNumWaitingVehicle(const std::string& laneID, int currentStep, int lastSwitchingTime) {
        return get<0>(computeWaitingVehicleInfo(laneID, currentStep,lastSwitchingTime));
    }

    int Lane::getStateOfWaitingVehicleOverTLSDuration(const std::string& laneID, int currentStep) {
        LibsaltLane lane = LibsaltLane(laneID);
        SALT::Link* link = Simulation::getNetworkManager()->findLinkByID(lane.link);
        int numSections = link->getNumSection();
        int stateWaitingOverTLSDuration = 0;
        for(int i = 0; i < numSections; i++){
            SALT::CellInterface* cell = link->getCellByIndex(i, lane.lane);
            auto cellstate = cell->getWaitVehStateOverTLSDuration(currentStep);
            if(cellstate == 1) {
                stateWaitingOverTLSDuration = 1;
                break;
            }
        }
        return stateWaitingOverTLSDuration;
    }

    float Lane::getCurrentAverageWaitingTimeBaseVehicle(const std::string& laneID, int currentStep) {
        return get<0>(getCurrentWaitingTimeBaseVeh(laneID, currentStep));
    }

    float Lane::getCurrentWaitingTimeSumBaseVehicle(const std::string& laneID, int currentStep) {
        return get<1>(getCurrentWaitingTimeBaseVeh(laneID, currentStep));
    }

// ===========================================================================
// Setter Implementation
// ===========================================================================

}

/****************************************************************************/