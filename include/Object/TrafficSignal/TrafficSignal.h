/*
 * TrafficSignal.h
 *
 *  Created on: 2018. 1. 29.
 *      Author: SDJ
 */

#ifndef OBJECTT_TRAFFICSIGNAL_H_
#define OBJECTT_TRAFFICSIGNAL_H_

#include <utils/config.h>
#include <map>
#include <set>
#include <string>


namespace SALT{
    class Link;
    class Node;
    class Connection;
    class TrafficSignalSchedule;

    class TrafficSignal {
    public:
        TrafficSignal();
        virtual ~TrafficSignal();

        //// functions to build member variables
        void setMyNode(Node* myNode = NULL) {
            this->myNode = myNode;
        }

        void setMyInitTime(SALTTime initTime) {
            this->mySimBeginTime = initTime;
        }

        void insertSchedule(std::string id, TrafficSignalSchedule* schedule);
        // plan := (startingTime, schedule)
        void insertPlan(std::string _startTime, std::string scheduleID);
        void insertPlan(SALTTime startTime, TrafficSignalSchedule* schedule);

        void insertLinkLinkIndex(Link* from, Link* to, int index);

        void initTrafficSignalStatus(std::string defaultScheduleID);

        void setScheduleState(SALTTime setTime);


        // TODO - @brief  finishConstruct
        // set myNode to the node found from id
        // set myGroup, currentPhase, myNext~~~
        void finishConstruct();

        //// functions to get info
        Node* getMyNode()  {	return myNode;	}
        int getLinkLinkIndex(Link* from, Link* to);
        std::string getStatusString();

        SALTTime getNextScheduleTime() {    return myNextScheduleTime; }

        // @brief return min(switchTime | switchTime>givenTime)
        SALTTime extractNextScheduleSwitchTime(SALTTime givenTime);

        //// functions to get current status
        TrafficSignalState getTrafficSignalState(Link* fromLink, Link* toLink);

        //// functions to update traffic signal
        // @brief determine update based on nextSwitchTime and currentTime
        void tryUpdate(SALTTime currentTime);
        void executeUpdateSchedule(SALTTime updateTime);
        void executeUpdatePhase(SALTTime updateTime);

        std::string getCurTrafficSignalState(){  return myCurrentSignalState;  }

        //@libsalt getter function for the learning mode
        std::string getCurrentOPTTrafficSignalScheduleID();
        TrafficSignalSchedule* getCurrentOPTTrafficSignalSchedule();
        int getCurrentOPTTrafficSignalPhaseIndex() {  return myCurrentPhaseIndex; }
        std::map<std::string, TrafficSignalSchedule*> getOPTTrafficSignalScheduleMap() {  return myScheduleMap;  };
        std::map<SALTTime, TrafficSignalSchedule*> getOPTTrafficSignalTODPlan() {  return myTODPlan;  }
        SALTTime getOPTTrafficSignalLastPhaseSwitchingTime() const {  return myLastPhaseSwitchTime;  }
        std::vector<std::string> getOPTTrafficSignalConnectedLinkIDs();
        vector<string> getOPTTrafficSignalConnectedLinkIDsCheckExceptable();
        vector<string> getOPTTrafficSignalConnectedLinkIDsCheckExceptableWTOMultiConnection();
        set<Link*> _getPreviousConnectedLinkIDsOfExceptableLink(Link* _exceptablelink);

        //@libsalt changing current TL Schedule to Next TL Schedule at given time step
        // TODO - @brief need to update the process to change the target schedule with considering TOD & changing offset
        void changeTLSchedule(SALTTime triggerTime);

        //@libsalt changing current TL Phase to given TL Phase at given time step
        void tryOPTUpdate(SALTTime currentTime);
        void executeUpdateOPTPhase(SALTTime updateTime);
        void executeUpdateOPTPhase(SALTTime triggerTime, int _phaseIndex);   //to need modify the detailed process
        void changeTLPhase(SALTTime triggerTime, const string& _scheduleID, int _phaseIndex);

        //@libsal changing current TL Phases to given TL PhaseVector at given time step
        void changeTLPhaseVector(SALTTime triggerTime, const string& _scheduleID, std::vector<std::pair<SALTTime,std::string>> _newphasevector);
        void changeTLPhaseVector(SALTTime triggerTime, const string& _scheduleID, int _phaseIndex, std::vector<std::pair<SALTTime,std::string>> _newphasevector);
        void changeTLPhaseVectorInStepwise(SALTTime triggerTime, const string& _scheduleID, std::vector<std::pair<SALTTime,std::string>> _newphasevector);



    private:
        static RandomGenerator* myRandomGenerator;
        Node* myNode=NULL;
        // TODO - group concept is null implemented currently
//	std::set<TrafficSignal*>* myGroup=NULL;
        SALTTime mySimBeginTime;

        // Time of Day Plan
        std::map<std::string, TrafficSignalSchedule*> myScheduleMap; // (schedule name, schedule itself)
        std::map<SALTTime, TrafficSignalSchedule*> myTODPlan; //ToD Plan (schedule starting time, schedule itself)

        // Connection Control
        std::map<Connection*, int> myMapConnectionIndex;
        std::map<Link*,std::map<Link*,int>> myLinkLinkIndex;
        std::map<int,Connection*> myMapIndexConnection; // the order of Connection on the signal state (like "Grrgggrr");


        // schedule - cur/nextSwitchTime
        TrafficSignalSchedule* myCurrentSchedule=NULL; //
        SALTTime myNextScheduleTime=-1;

        // phase - last/cur/nextSwitchTime
        SALTTime myLastPhaseSwitchTime=-1;
        int myCurrentPhaseIndex = -1;
        SALTTime myNextPhaseTime=-1;

        // determined signal state
        int myCurrentPhaseNum = 0;
        std::string myCurrentSignalState="";
    };
}
#endif /* OBJECTT_TRAFFICSIGNAL_H_ */
