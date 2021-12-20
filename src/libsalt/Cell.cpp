/****************************************************************************/
// C++ libsalt.cell API implementation
/****************************************************************************/

#include <json/json.h>
#include <Object/NetworkManager.h>
#include <utils/ParserSALT.h>
#include <libsalt/LibsaltDefs.h>
#include "Cell.h"
#include "Simulation.h"

namespace libsalt {

// ===========================================================================
// Getter Implementation
// ===========================================================================

    std::vector<LibsaltCell>
    Cell::getCellList() {
        std::vector<LibsaltCell> cells;

        const auto rs = Simulation::getNetworkManager()->getCellList();
        for (auto r: rs) {
            LibsaltCell cell = LibsaltCell(r);
            cells.push_back(cell);
        }

        return cells;
    }

    std::vector<LibsaltCell>
    Cell::getActiveCellList() {
        std::vector<LibsaltCell> cells;

        const auto rs = Simulation::getNetworkManager()->getMyActiveCellSet();
        for (auto r: rs) {
            LibsaltCell cell = LibsaltCell(r);
            cells.push_back(cell);
        }

        return cells;
    }

    std::vector<LibsaltCell>
    Cell::getValidCellList() {
        std::vector<LibsaltCell> cells;

        const auto rs = Simulation::getNetworkManager()->getValidCellList();
        for (auto r: rs){
            LibsaltCell cell = LibsaltCell(r);
            cells.push_back(cell);
        }

        return cells;
    }

    LibsaltCell
    Cell::getCell(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return cell;
    }

    LibsaltCell
    Cell::getCell(const std::string& linkID, int section, int lane) {
        LibsaltCell cell = LibsaltCell(linkID, section, lane);
        return cell;
    }

    // by linkID, section, lane

    int Cell::getCurrentVolume(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getMyCurrentVolume();
    }

    int Cell::getCurrentRoom(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getCurrentRoom();
    }

    int Cell::getNumVehPassed(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getNumVehPassed();
    }

    float Cell::getSumTravelLength(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getSumTravelLength();
    }

    float Cell::getAverageWaitingQLength(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getAverageWaitingQLength();
    }

    float Cell::getSumTravelTime(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getSumTravelTime();
    }

    float Cell::getAverageDensity(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getAverageDensity();
    }

    float Cell::getAverageNumVehicles(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getAverageNumVehs();
    }

    float Cell::getAverageSpeed(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getAverageSpeed();
    }

    float Cell::getAverageVehicleSpeed(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getAverageVehicleSpeed();
    }

    float Cell::getAverageWaitingTime(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getAverageWaitingTime();
    }

    std::tuple<float, float, float>
    Cell::computeWaitingVehicleInfo(const string &linkID, int section, int lane, int currentStep, int lastSwitchingTime) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->computeWaitingVehicle(currentStep,lastSwitchingTime);
    }

    float
    Cell::getAverageVehicleWaitingTime(const string &linkID, int section, int lane, int currentStep, int lastSwitchingTime) {
        return get<1>(computeWaitingVehicleInfo(linkID, section, lane, currentStep, lastSwitchingTime));
    }

    float
    Cell::getAverageVehicleWaitingQLength(const string &linkID, int section, int lane, int currentStep, int lastSwitchingTime) {
        return get<2>(computeWaitingVehicleInfo(linkID, section, lane, currentStep, lastSwitchingTime));
    }

    float
    Cell::genNumWaitingVehicle(const string &linkID, int section, int lane, int currentStep, int lastSwitchingTime) {
        return get<0>(computeWaitingVehicleInfo(linkID, section, lane, currentStep, lastSwitchingTime));
    }

    std::tuple<float, float>
    Cell::getCurrentWaitingTimeBaseVeh(const string &linkID, int section, int lane, int currentStep) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getVehicleWaitingTimeBaseVeh(currentStep);
    }

    int Cell::getStateOfWaitingVehicleOverTLSDuration(const string &linkID, int section, int lane, int currentStep) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getWaitVehStateOverTLSDuration(currentStep);
    }

    float
    Cell::getCurrentAverageWaitingTimeBaseVehicle(const string &linkID, int section, int lane, int currentStep) {
        return get<0>(getCurrentWaitingTimeBaseVeh(linkID, section, lane, currentStep));
    }

    float
    Cell::getCurrentWaitingTimeSumBaseVehicle(const string &linkID, int section, int lane, int currentStep) {
        return get<1>(getCurrentWaitingTimeBaseVeh(linkID, section, lane, currentStep));
    }




    int Cell::getLength(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getMyLength();
    }

    float Cell::getSpeedLimit(const std::string& linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getSpeedLimit();
    }

    long Cell::getCurrentWaitingVolume(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getMyCurrentWaitingVolume();
    }

    long Cell::getCurrentRunningVolume(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getMyCurrentRunningVolume();
    }

    long Cell::getCurrentPendingVolume(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getMyCurrentPendingVolume();
    }

    long Cell::getCurrentReceivingVolume(const std::string &linkID, int section, int lane) {
        return Simulation::getNetworkManager()->findLinkByID(linkID)->getCellByIndex(section, lane)->getMyCurrentReceivingVolume();
    }

    // by cellID

    int Cell::getCurrentVolume(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getMyCurrentVolume();
    }

    int Cell::getCurrentRoom(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getCurrentRoom();
    }

    int Cell::getNumVehPassed(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getNumVehPassed();
    }

    float Cell::getSumTravelLength(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getSumTravelLength();
    }

    float Cell::getAverageWaitingQLength(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getAverageWaitingQLength();
    }

    float Cell::getSumTravelTime(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getSumTravelTime();
    }

    float Cell::getAverageDensity(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getAverageDensity();
    }

    float Cell::getAverageNumVehicles(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getAverageNumVehs();
    }

    float Cell::getAverageSpeed(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getAverageSpeed();
    }

    float Cell::getAverageVehicleSpeed(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getAverageVehicleSpeed();
    }

    float Cell::getAverageWaitingTime(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getAverageWaitingTime();
    }

    int Cell::getLength(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getMyLength();
    }

    float Cell::getSpeedLimit(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getSpeedLimit();
    }

    long Cell::getCurrentWaitingVolume(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getMyCurrentWaitingVolume();
    }

    long Cell::getCurrentRunningVolume(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getMyCurrentRunningVolume();
    }

    long Cell::getCurrentPendingVolume(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getMyCurrentPendingVolume();
    }

    long Cell::getCurrentReceivingVolume(const std::string& cellID) {
        LibsaltCell cell = LibsaltCell(cellID);
        return Simulation::getNetworkManager()->findLinkByID(cell.link)->getCellByIndex(cell.section, cell.lane)->getMyCurrentReceivingVolume();
    }



// ===========================================================================
// Setter Implementation
// ===========================================================================

}

/****************************************************************************/