/****************************************************************************/
// C++ libsalt.trafficsignal API implementation
/****************************************************************************/
#include <Object/TrafficSignalManager.h>
#include <Object/TrafficSignal/TrafficSignal.h>
#include "TrafficSignal.h"
#include "Simulation.h"

namespace libsalt {
    SALT::TrafficSignalManager *libsalt::TrafficSignal::getTLSManager() {
        return Simulation::getTrafficSignalManager();
    }

    std::vector<std::string> libsalt::TrafficSignal::getTLSIDList() {
        return getTLSManager()->getOPTTrafficSignalIDList();
    }

    int libsalt::TrafficSignal::getTLSIDCount() {
        return getTLSManager()->getOPTTrafficSignalIDCount();
    }

    libsalt::TLSLogic *libsalt::TrafficSignal::getTLSByNodeID(std::string _tlsID) {
        SALT::TrafficSignal * tls =  getTLSManager()->getOPTTrafficSignalByNodeID(_tlsID);
        std::map<int, SALT::TrafficSignalSchedule*> todplanmap = tls->getOPTTrafficSignalTODPlan();
        std::map<std::string, SALT::TrafficSignalSchedule*> tssmap = tls->getOPTTrafficSignalScheduleMap();
        std::map<std::string, TLSSchedule*> tlssmap;
        for (auto tss : tssmap) {
            auto tlss = new libsalt::TLSSchedule(tss.first, tss.second->getMyOffset(), tss.second->getMyPhaseVector());
            tlssmap[tss.first] = tlss;
        }
        std::map<int, std::string> tlstodmap;
        for (auto todp : todplanmap) {
            tlstodmap[todp.first] = todp.second->getID();
        }

        return new TLSLogic(_tlsID, tlssmap, tlstodmap);
    }


    std::vector<std::string> TrafficSignal::getTLSConnectedLinkID(std::string _tlsID) {
        return getTLSManager()->getOPTTrafficSignalConnectedLinkIDs(_tlsID);
    }


    std::vector<std::string> TrafficSignal::getTLSConnectedLinkIDsCheckExceptable(std::string _tlsID) {
        return getTLSManager()->getOPTTrafficSignalConnectedLinkIDsCheckExceptable(_tlsID);
    }


    std::vector<std::string> TrafficSignal::getTLSConnectedLinkIDsCheckExceptableWTOMultiConnection(std::string _tlsID) {
        return getTLSManager()->getOPTTrafficSignalConnectedLinkIDsCheckExceptableWTOMultiConnection(_tlsID);
    }


    std::vector<std::string> libsalt::TrafficSignal::getTLSScheduleIDsByNodeID(std::string _tlsID) {
        return getTLSManager()->getOPTTrafficSignalScheduleIDs(_tlsID);
    }

    std::string libsalt::TrafficSignal::getCurrentTLSScheduleIDByNodeID(std::string _tlsID) {
        return getTLSManager()->getCurrentOPTTrafficSignalScheduleID(_tlsID);
    }

    libsalt::TLSSchedule *
    libsalt::TrafficSignal::getCurrentTLSScheduleByNodeID(std::string _tlsID) {
        SALT::TrafficSignalSchedule * tss = getTLSManager()->getCurrentOPTTrafficSignalScheduleByNodeID(_tlsID);
        return new TLSSchedule(tss->getID(), tss->getMyOffset(), tss->getMyPhaseVector());
    }

    std::vector<std::pair<int, std::string>>
    libsalt::TrafficSignal::getTLSPhasesByNodeID(std::string _tlsID, std::string _scheduleID) {
        std::map<std::string, SALT::TrafficSignalSchedule*> tssmap = getTLSManager()->getOPTTrafficSignalByNodeID(_tlsID)->getOPTTrafficSignalScheduleMap();
        return tssmap[_scheduleID]->getMyPhaseVector();
    }

    std::pair<int, std::string>
    libsalt::TrafficSignal::getTLSPhaseByNodeID(std::string _tlsID, std::string _scheduleID, int _phaseIndex) {
        return getTLSPhasesByNodeID(_tlsID, _scheduleID)[_phaseIndex];
    }

    std::string libsalt::TrafficSignal::getCurrentTLSPhaseStateByNodeID(std::string _tlsID) {
        return getTLSManager()->getCurrentOPTTrafficSignalPhaseStateByNodeID(_tlsID);
    }

    int libsalt::TrafficSignal::getCurrentTLSPhaseIndexByNodeID(std::string _tlsID) {
        return getTLSManager()->getCurrentOPTTrafficSignalPhaseIndexByNodeID(_tlsID);
    }

    int libsalt::TrafficSignal::getLastTLSPhaseSwitchingTimeByNodeID(std::string _tlsID) {
        return getTLSManager()->getOPTTrafficSignalLastPhaseSwitchingTimeByNodeID(_tlsID);
    }

    int libsalt::TrafficSignal::setTLSByNodeID(std::string _tlsID, libsalt::TLSLogic *_logic) {
        std::map<std::string, SALT::TrafficSignalSchedule*> schedulemap; // (schedule name, schedule itself)
        std::map<int, SALT::TrafficSignalSchedule*> todplan; //ToD Plan (schedule starting time, schedule itself)
        auto transTODP = _transformTODPlan(_logic);

#ifdef DEBUG
        auto origintl = getTLSManager()->getOPTTrafficSignalByNodeID(_tlsID);
        cout<<"[Original TrafficSignal Logic] "<<endl;
        for (auto tl : origintl->getOPTTrafficSignalScheduleMap()) {
            cout<<"schedule id = "<<tl.first<<endl;
            for (auto tlsch : tl.second->getMyPhaseVector()) {
                cout<<"phase = ("<<tlsch.second<<"), duration = "<<tlsch.first<<endl;
            }
        }
        cout<<"[New TrafficSignal Logic] "<<endl;
        for(auto ntl : _logic->getScheduleMap()) {
            cout<<"schedul id = "<<ntl.first<<endl;
            for (auto ntlsch : ntl.second->getPhaseVector()) {
                cout<<"phase = ("<<ntlsch.second<<"), duration = "<<ntlsch.first<<endl;
            }
        }
#endif
        auto res = FAIL;

        for (auto tlss : _logic->getScheduleMap()) {
            auto todkey = transTODP.find(tlss.first);
            if (todkey != transTODP.end()) {
                if(getTLSManager()->setOPTTrafficSignalScheduleByNodeID(_tlsID, todkey->second, tlss.second->getSchedulID(), tlss.second->getOffset(), tlss.second->getPhaseVector()) == SALT::SUCCESS)
                    res = SUCCEED;
                else res = FAIL;
            } else {
                if(getTLSManager()->setOPTTrafficSignalScheduleByNodeID(_tlsID, -1, tlss.second->getSchedulID(), tlss.second->getOffset(), tlss.second->getPhaseVector()) == SALT::SUCCESS)
                    res = SUCCEED;
                else res = FAIL;
            }
        }
        return res;
    }

    void TrafficSignal::setTLSPhaseVector(int triggertime, std::string _nodeID, std::string _scheduleID,
                                          std::vector<std::pair<int, std::string>> _phaseVector) {
        getTLSManager()->changeTLSPhaseVector(triggertime, _nodeID, _scheduleID, _phaseVector);
    }


    void
    TrafficSignal::setTLSPhaseVector(int triggertime, std::string _nodeID, std::string _scheduleID, int _phaseIndex,
                                     std::vector<std::pair<int, std::string>> _phaseVector) {
        getTLSManager()->changeTLSPhaseVector(triggertime, _nodeID, _scheduleID, _phaseIndex, _phaseVector);
    }


    void libsalt::TrafficSignal::changeTLSPhase(int _triggertime, std::string _nodeID,
                                                std::string _scheduleID, int _phaseIndex) {
        getTLSManager()->changeTLPhase(_triggertime, _nodeID, _scheduleID, _phaseIndex);
    }


    void TrafficSignal::changeTLSPhaseVectorInStepwise(int triggertime, std::string _nodeID, std::string _scheduleID,
                                                       std::vector<std::pair<int, std::string>> _phaseVector) {
        getTLSManager()->changeTLSPhaseVectorInStepwise(triggertime, _nodeID, _scheduleID, _phaseVector);
    }

    std::map<std::string, int> libsalt::TrafficSignal::_transformTODPlan(libsalt::TLSLogic *_logic) {
        std::map<std::string, int> transTODP;
        for (auto & tod : _logic->myTODPlan){
            transTODP.insert(std::make_pair(tod.second, tod.first));
        }
        return transTODP;
    }




} // end of namespace libsalt


/****************************************************************************/
