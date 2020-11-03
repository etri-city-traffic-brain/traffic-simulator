/****************************************************************************/
// C++ libsalt.lane API implementation
/****************************************************************************/
#pragma once

#include <vector>
#include <libsalt/LibsaltDefs.h>
#include <Object/Network/CellInterface.h>
#include <Object/NetworkManager.h>

// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class Lane
 * @brief C++ libsalt.lane API implementation
 */
namespace libsalt {
    class Lane {
    private:
        // Invalidate standard constructor
        Lane() = delete;

    public:
        // --------------------------------------------------------------
        // Getter
        // --------------------------------------------------------------
        // laneID = myLink->getID()+"_"+std::to_string(myLane);
        static int getLength(const std::string& laneID);
        static float getSpeedLimit(const std::string& laneID);
        static int getNumVehPassed(const std::string& laneID);
        static float getAverageNumVehs(const std::string& laneID);
        static float getAverageSpeed(const std::string& laneID);
        static float getAverageVehicleSpeed(const std::string& laneID);
        static float getAverageWaitingQLength(const std::string& laneID);
        static float getAverageWaitingTime(const std::string& laneID);
        static float getAverageDensity(const std::string& laneID);
        static float getSumTravelLength(const std::string& laneID);
        static float getSumTravelTime(const std::string& laneID);
        static int getCurrentVolume(const std::string& laneID);
        static int getCurrentRoom(const std::string& laneID);
        static int getActivationCount(const std::string& laneID);

        // --------------------------------------------------------------
        // Setter
        // --------------------------------------------------------------

    }; // End of class Lane

} // End of namespace libsalt

/****************************************************************************/
