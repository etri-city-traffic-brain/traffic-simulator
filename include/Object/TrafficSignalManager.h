/*
 * TrafficSignalManager.h
 *
 *  Created on: 2018. 1. 30.
 *      Author: SDJ
 */

#ifndef TRAFFICSIGNALMANAGER_H_
#define TRAFFICSIGNALMANAGER_H_

#include <Object/TrafficSignal/TrafficSignal.h>
#include <utils/config.h>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
namespace SALT{

    class SimScenarioInfo;

    class TrafficSignalManager {
    private:
        static TrafficSignalManager* instance;

        TrafficSignalManager(){};
    public:
        static TrafficSignalManager* getInstance();

    public:
        virtual ~TrafficSignalManager();
        Result build(SimScenarioInfo* scenario);
        Result build(string const& _signal_file, const string& _input_type);

        void setMyInitTime(SALTTime initTime) {
            this->mySimBeginTime = initTime;
        }

        // @brief: after parsing and constructing each TrafficSignal instance, register its info to other network elements
        Result finishBuildingTrafficSignal();

        void registerTrafficSignalToManager(const string& _nodeID, TrafficSignal* _ts);

        // @libsalt register function for the learning mode
        void registerOPTTrafficSignalToManager(const string& _nodeID);

        // @brief: update current signal state of all traffic signal.
        void update(SALTTime currentTime);

        void changeStatusToQueryStep(SALTTime queryStep);

        void printStatus(SCOPE _scope=MACRO);
        string print(void);

        // @getter functions
        TrafficSignal* getTrafficSignalByNodeID(const string& _nodeID);
        const list<TrafficSignal*>& getTrafficSignalList() const {	return myTrafficSignalList;	}

        // @libsalt getter function for the Traffic Light Signal Optimization mode
        list<TrafficSignal*> getOPTTrafficSignalList();
        vector<string> getOPTTrafficSignalIDList();
        int getOPTTrafficSignalIDCount();
        TrafficSignal* getOPTTrafficSignalByNodeID(const string& _nodeID);
        vector<string> getOPTTrafficSignalConnectedLinkIDs(const string& _nodeID);
        vector<string> getOPTTrafficSignalConnectedLinkIDsCheckExceptable(const string& _nodeID);
        vector<string> getOPTTrafficSignalConnectedLinkIDsCheckExceptableWTOMultiConnection(const string& _nodeID);
        TrafficSignalSchedule* getCurrentOPTTrafficSignalScheduleByNodeID(const string& _nodeID);
        vector<string> getOPTTrafficSignalScheduleIDs(const string& _nodeID);
        string getCurrentOPTTrafficSignalScheduleID(const string& _nodeID);
        string getCurrentOPTTrafficSignalPhaseStateByNodeID(const string& _nodeID);
        int getCurrentOPTTrafficSignalPhaseIndexByNodeID(const string& _nodeID);
        SALTTime getOPTTrafficSignalLastPhaseSwitchingTimeByNodeID(const string& _nodeID);

        // @libsalt changing schedule or phase of target Simulation Traffic Signal
        void changeTLScheduleByNodeID(SALTTime triggertime, const string& _nodeID);
        void changeTLSPhaseVector(SALTTime triggertime, const string &_nodeID, const string &_scheduleID, std::vector<std::pair<SALTTime,std::string>> _phasevector);
        void changeTLSPhaseVector(SALTTime triggertime, const string &_nodeID, const string &_scheduleID, int _phaseIndex, std::vector<std::pair<SALTTime,std::string>> _phasevector);
        void changeTLSPhaseVectorInStepwise(SALTTime triggertime, const string &_nodeID, const string &_scheduleID, std::vector<std::pair<SALTTime,std::string>> _phasevector);
        void changeTLPhase(SALTTime triggertime, const string &_nodeID, const string &_scheduleID, int _phaseIndex);

        // @libsalt set todplan, schedule or phase of target Simulation Traffic Signal (Not Dynamical Changing, just set the information before simulation)
        Result setOPTTrafficSignalScheduleByNodeID(const string &_nodeID, SALTTime todkey, const string &_scheduleID, SALTTime _offset, std::vector<std::pair<SALTTime,std::string>> _phasevector);

        void clear();

    private:
        list<TrafficSignal*> myTrafficSignalList;
        list<TrafficSignal*> myOPTTrafficSignalList;
        map<string, TrafficSignal*> myTrafficSignalMap;
        map<string, TrafficSignal*> myOPTTrafficSignalMap;
        map<string, set<TrafficSignal*>> myTrafficSignalGroupMap;
        // @libsalt
        TrafficSignal* _getOPTTrafficSignalByNodeID(const string& _nodeID);

        SALTTime mySimBeginTime;

    };

    ostream& operator << (ostream& strm, TrafficSignalManager& obj);
}

#endif /* TRAFFICSIGNALMANAGER_H_ */
