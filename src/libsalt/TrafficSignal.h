/****************************************************************************/
// C++ libsalt.trafficsignal API implementation
/****************************************************************************/
#pragma once

#include <vector>
#include <libsalt/LibsaltDefs.h>
#include <utils/config.h>


//#define DEBUG
#define FAIL 0
#define SUCCEED 1

// ===========================================================================
// class definitions
// ===========================================================================
/**
* @class TrafficSignal
* @brief C++ libsalt API implementation
*/

namespace libsalt {
    class TrafficSignal {
    public:
        static std::vector<std::string> getTLSIDList();
        static int getTLSIDCount();
        static TLSLogic* getTLSByNodeID(std::string _tlsID);
        static std::vector<std::string> getTLSConnectedLinkID(std::string _tlsID);
        static std::vector<std::string> getTLSConnectedLinkIDsCheckExceptable(std::string _tlsID);
        static std::vector<std::string> getTLSConnectedLinkIDsCheckExceptableWTOMultiConnection(std::string _tlsID);
        static std::vector<std::string> getTLSScheduleIDsByNodeID(std::string _tlsID);
        static std::string getCurrentTLSScheduleIDByNodeID(std::string _tlsID);
        static TLSSchedule* getCurrentTLSScheduleByNodeID(std::string _tlsID);
        static std::vector<std::pair<int,std::string>> getTLSPhasesByNodeID(std::string _tlsID, std::string _scheduleID);
        static std::pair<int,std::string> getTLSPhaseByNodeID(std::string _tlsID, std::string _scheduleID, int _phaseIndex);
        static std::string getCurrentTLSPhaseStateByNodeID(std::string _tlsID);
        static int getCurrentTLSPhaseIndexByNodeID(std::string _tlsID);
        static int getLastTLSPhaseSwitchingTimeByNodeID(std::string _tlsID);


        static int setTLSByNodeID(std::string _tlsID, TLSLogic* _logic);
        static void setTLSPhaseVector(int triggertime, std::string _nodeID, std::string _scheduleID, std::vector<std::pair<int,std::string>> _phaseVector);
        static void setTLSPhaseVector(int triggertime, std::string _nodeID, std::string _scheduleID, int _phaseIndex, std::vector<std::pair<int,std::string>> _phaseVector);
        static void changeTLSPhase(int triggertime, std::string _nodeID, std::string _scheduleID, int _phaseIndex);
        static void changeTLSPhaseVectorInStepwise(int triggertime, std::string _nodeID, std::string _scheduleID, std::vector<std::pair<int,std::string>> _phaseVector);

    private:
        static SALT::TrafficSignalManager* getTLSManager();
        static std::map<std::string, int> _transformTODPlan(TLSLogic* _logic);

    };

} // end of namespace libsalt
