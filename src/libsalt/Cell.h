/****************************************************************************/
// C++ libsalt.cell API implementation
/****************************************************************************/
#pragma once

#include <vector>
#include <libsalt/LibsaltDefs.h>
#include <Object/Network/CellInterface.h>
#include <Object/NetworkManager.h>

/**
 * @class Cell
 * @brief C++ libsalt.cell API implementation
 */
namespace libsalt {
    class Cell {
    private:
        // Invalidate standard constructor
        Cell() = delete;
        static std::tuple<float,float,float> computeWaitingVehicleInfo(const string &linkID, int section, int lane, int currentStep, int lastSwitchingTime);
        static std::tuple<float,float> getCurrentWaitingTimeBaseVeh(const string &linkID, int section, int lane, int currentStep);

    public:
        // --------------------------------------------------------------
        // Getter
        // --------------------------------------------------------------
        // cellID = linkID + "_" + section + "_" + Lane;
        static std::vector<LibsaltCell> getValidCellList();
        static std::vector<LibsaltCell> getCellList();
        static std::vector<LibsaltCell> getActiveCellList();
        static LibsaltCell getCell(const std::string& cellID);
        static LibsaltCell getCell(const std::string& linkID, int section, int lane);

        // by linkID, section, lane
        static int getCurrentVolume(const std::string& linkID, int section, int lane);
        static int getCurrentRoom(const std::string& linkID, int section, int lane);
        static int getLength(const std::string &linkID, int section, int lane);
        static float getSpeedLimit(const std::string &linkID, int section, int lane);
        static int getNumVehPassed(const std::string &linkID, int section, int lane);
        static float getSumTravelTime(const std::string &linkID, int section, int lane);
        static float getSumTravelLength(const std::string &linkID, int section, int lane);

        static float getAverageWaitingQLength(const std::string &linkID, int section, int lane);
        static float getAverageDensity(const std::string &linkID, int section, int lane);
        static float getAverageNumVehicles(const std::string &linkID, int section, int lane);
        static float getAverageSpeed(const std::string &linkID, int section, int lane);
        static float getAverageVehicleSpeed(const std::string &linkID, int section, int lane);
        static float getAverageWaitingTime(const std::string &linkID, int section, int lane);

        static float getAverageVehicleWaitingTime(const std::string& linkID, int section, int lane, int currentStep, int lastSwitchingTime);
        static float getAverageVehicleWaitingQLength(const std::string& linkID, int section, int lane, int currentStep, int lastSwitchingTime);
        static float genNumWaitingVehicle(const std::string& linkID, int section, int lane, int currentStep, int lastSwitchingTime);
        static int getStateOfWaitingVehicleOverTLSDuration(const std::string& linkID, int section, int lane, int currentStep);
        static float getCurrentAverageWaitingTimeBaseVehicle(const std::string& linkID, int section, int lane, int currentStep);
        static float getCurrentWaitingTimeSumBaseVehicle(const std::string& linkID, int section, int lane, int currentStep);

        static long getCurrentWaitingVolume(const std::string &linkID, int section, int lane);
        static long getCurrentRunningVolume(const std::string &linkID, int section, int lane);
        static long getCurrentPendingVolume(const std::string &linkID, int section, int lane);
        static long getCurrentReceivingVolume(const std::string &linkID, int section, int lane);

        // by cellID
        static int getCurrentVolume(const std::string& cellID);
        static int getCurrentRoom(const std::string& cellID);
        static int getLength(const std::string& cellID);
        static float getSpeedLimit(const std::string& cellID);
        static int getNumVehPassed(const std::string& cellID);
        static float getSumTravelTime(const std::string& cellID);
        static float getSumTravelLength(const std::string& cellID);

        static float getAverageWaitingQLength(const std::string& cellID);
        static float getAverageDensity(const std::string& cellID);
        static float getAverageNumVehicles(const std::string& cellID);
        static float getAverageSpeed(const std::string& cellID);
        static float getAverageVehicleSpeed(const std::string& cellID);
        static float getAverageWaitingTime(const std::string& cellID);

        static long getCurrentWaitingVolume(const std::string& cellID);
        static long getCurrentRunningVolume(const std::string& cellID);
        static long getCurrentPendingVolume(const std::string& cellID);
        static long getCurrentReceivingVolume(const std::string& cellID);

        // --------------------------------------------------------------
        // Setter
        // --------------------------------------------------------------

    }; // End of class Cell

} // End of namespace libsalt

/****************************************************************************/