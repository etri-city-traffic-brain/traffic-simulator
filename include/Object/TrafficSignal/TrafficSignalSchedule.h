/*
 * TrafficSignalSchedule.h
 *
 *  Created on: 2018. 1. 29.
 *      Author: SDJ
 */

#ifndef OBJECTT_TRAFFICSIGNALSCHEDULE_H_
#define OBJECTT_TRAFFICSIGNALSCHEDULE_H_

#include <utils/config.h>
#include <string>
#include <utility>
#include <vector>
namespace SALT{
    class TrafficSignalSchedule {
    public:
        virtual ~TrafficSignalSchedule();
        TrafficSignalSchedule(std::string id, std::string offset);
        TrafficSignalSchedule(std::string id, std::string offset, int phaseNum);
        TrafficSignalSchedule(std::string id, SALTTime offset, std::vector<std::pair<SALTTime,std::string>> phasevector);
        void appendPhase(std::string duration, std::string state);

        const std::string& getID() const {
            return myID;
        }

        SALTTime computeInitialPhaseIndex();
        SALTTime computeInitialNextPhaseTime(SALTTime currentTime);
        std::string convertIndex2State(int phaseIndex);
        SALTTime convertIndex2Duration(int phaseIndex);

        int getMyPhaseNum() const {
            return myPhaseNum;
        }

        SALTTime getMyOffset() {  return myOffset;  }
        std::vector<std::pair<SALTTime,std::string>> getMyPhaseVector() {  return myPhaseVector;  }

        SALTTime getMyPeriod() {  return myPeriod;  }

        void swapOPTMyPhaseVector(std::vector<std::pair<SALTTime,std::string>> _newphasevector);
        void updateOPTPhaseAfterSwap();

        bool validate() const;

        string print();

    private:
        std::string myID="";
        SALTTime myOffset=0; // ����

        // phase := pair<duration,state>
        std::vector<std::pair<SALTTime,std::string>> myPhaseVector;

        SALTTime myPeriod=0; // �ֱ� = sum(phase.duration)
        int myPhaseNum=0;



    };
}
#endif /* OBJECTT_TRAFFICSIGNALSCHEDULE_H_ */
