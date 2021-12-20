/****************************************************************************/
// C++ libsalt.link API implementation
/****************************************************************************/
#pragma once

#include <vector>
#include <libsalt/LibsaltDefs.h>
#include <Object/Network/CellInterface.h>
#include <Object/NetworkManager.h>

/**
 * @class Link
 * @brief C++ libsalt.link API implementation
 */
namespace libsalt {
    class Link {
    private:
        // Invalidate standard constructor
        Link() = delete;
        static std::tuple<float,float,float> computeWaitingVehicleInfo(const string &linkID, int currentStep, int lastSwitchingTime);
        static std::tuple<float,float> getCurrentWaitingTimeBaseVeh(const string &linkID, int currentStep);

    public:
        // --------------------------------------------------------------
        // Getter
        // --------------------------------------------------------------

        static std::vector<LibsaltLink> getLinkList();
        static std::vector<LibsaltLink> getValidLinkList();
        static LibsaltLink getLink(const std::string& linkID);
        static std::vector<LibsaltLane> getLaneList(const std::string& linkID);

        static int getLength(const std::string& linkID);
        static int getNumLane(const std::string& linkID);
        static int getNumSection(const std::string& linkID);
        static float getSpeedLimit(const std::string& linkID);
        static float getSumPassed(const std::string& linkID);
        static float getAverageWaitingQLength(const std::string& linkID);
        static float getAverageWaitingTime(const std::string& linkID);
        static float getAverageDensity(const std::string& linkID);
        static float getAverageSpeed(const std::string& linkID);
        static float getAverageVehicleSpeed(const std::string& linkID);
        static float getAverageVehicleWaitingTime(const std::string& linkID, int currentStep, int lastSwitchingTime);
        static float getAverageVehicleWaitingQLength(const std::string& linkID, int currentStep, int lastSwitchingTime);
        static float getNumWaitingVehicle(const std::string& linkID, int currentStep, int lastSwitchingTime);
        static int getStateOfWaitingVehicleOverTLSDuration(const std::string& linkID, int currentStep);
        static float getCurrentAverageWaitingTimeBaseVehicle(const std::string& linkID, int currentStep);
        static float getCurrentWaitingTimeSumBaseVehicle(const std::string& linkID, int currentStep);
        static float getSumTravelLength(const std::string& linkID);
        static float getSumTravelTime(const std::string& linkID);

        static LibsaltCell getCellByIndex(const std::string& linkID, int section, int lane);
        static std::string getTrafficSignalState(const std::string& linkID, const std::string& nextLinkID);
        static LibsaltConnection getAnyConnectionTo(const std::string& linkID, const std::string& nextLinkID);
        static LibsaltConnection getConnectionTo(const std::string& linkID, const std::string& nextLinkID, int fromLane);
        static bool canReach(const std::string& linkID, const std::string& nextLinkID);

        static LibsaltNode getFromNode(const std::string& linkID);
        static LibsaltNode getToNode(const std::string& linkID);
        static std::string getSpreadType(const std::string& linkID);
        static bool isExceptionalLink(const std::string& linkID);
        static bool isOutFlowLink(const std::string& linkID);
        static const::string getShape(const std::string& linkID);
        static LibsaltPosition getDirection(const std::string& linkID);
        static int getLeftPocket(const std::string& linkID);
        static int getRightPocket(const std::string& linkID);
    }; // End of class Link

} // End of namespace libsalt

/****************************************************************************/