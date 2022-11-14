/*
 * CellInterface.h
 *
 *  Created on: 2018. 3. 14.
 *      Author: SDJ
 */

#ifndef CELLINTERFACE_H_
#define CELLINTERFACE_H_

#include <utils/config.h>
#include <list>
#include <map>
#include <string>
#include <tuple>
#include <Object/Output/OutputWriter.h>
#include <Object/TrafficSignal/TrafficSignalSchedule.h>


namespace SALT{

class Link;
class VehicleInterface;
class SectionInterface;


class CellInterface {
public:
	CellInterface(Link* _link,  int _len, int _row, int _col, bool _myExceptionFlag=false, Length _vehLen=5.0f);
	virtual ~CellInterface();

	typedef tuple<VehicleInterface*,SALTTime, SALTTime> RunTupleType;
	typedef tuple<VehicleInterface*,SALTTime> WaitTupleType;
	typedef tuple<VehicleInterface*,SALTTime> PendTupleType;
	typedef tuple<VehicleInterface*,SALTTime, CellInterface*> ReceiveTupleType;


	virtual void updateStatus()=0;
	virtual string getStatusString()=0;

	virtual void preprocess(SALTTime t)=0;
	virtual void process(SALTTime t)=0;
	virtual void postprocess(SALTTime t)=0;


	// @brief: insert a vehicle into simulation i.e. cell's pendingQ
	virtual void executeVehInsert(VehicleInterface* veh, SALTTime tDepart)=0;

	// @brief: let a vehicle from pendingQ run on cell
	virtual void executeVehDepart(SALTTime tDepart)=0;

	// @brief: pop a vehicle that ends its route
	virtual void executeVehPop(SALTTime t, RunTupleType _running_tuple)=0;

	// @brief: move vehicle from runningQ to waitingQ
	virtual void executeVehMove(SALTTime tStartWaiting, RunTupleType _running_tuple)=0;

	// @brief: send a vehicle to the next cell (high pendingQ)
	virtual bool executeVehSend(SALTTime t, WaitTupleType _wait_tuple)=0;
	// @brief: send back a vehicle to the prev cell (waitingQ)
	virtual void rollbackVehSend(SALTTime t)=0;
	// @brief: receive a vehicle that the previous cell sent (i.e. from this cell's high pendingQ)
	virtual void executeVehReceive(SALTTime tEnterTime)=0;


//	virtual void executeVehImport(SALTTime t);
	// @brief: insert a vehicle into runningQ
	// v : the vehicle inserted in the simulation
	// t : the time when the vehicle departs
	virtual Result insertRunningQ(VehicleInterface* v, SALTTime t)=0;

	// @brief: insert a vehicle into pendingQ
	// v : the vehicle that try to run
	// t : the time when the vehicle plans to depart its route
	virtual Result insertPendingQ(VehicleInterface* v, SALTTime t)=0;

	// @brief: insert a vehicle into receivingQ(high priority pendingQ)
	// v : the vehicle that should depart but cannot depart until conditions meet
	// tStartWaiting : the time when the vehicle .... TODO
	// fromCell : TODO
	virtual Result insertReceivingQ(VehicleInterface* v, SALTTime tStartWaiting, CellInterface* fromCell)=0;

	// @brief: move a vehicle from runningQ to waitingQ
	// v : the vehicle whose expected time to leave is overdue
	// t : the time when the vehicle starts to wait
	virtual Result insertWaitingQ(VehicleInterface* v, SALTTime t, bool insertToBack=true)=0;




	// @brief: traffic signal checking (called by wait2highpend)
	Result checkTrafficSignal(CellInterface* nextCell);
	Result checkTrafficSignal(Link* toLink);
	Result checkTrafficSignal(VehicleInterface* veh);
	Result checkTrafficSignalWithCache(VehicleInterface* veh);
	void makeLinkListToNextValidLink(VehicleInterface* veh);

	Result decreaseVolume(int vol=1);
	Result increaseVolume(int vol=1);

    // compute current sum of vehicle lengths (operation = 0:when increasing volume or 1:when decreasing volume)
    Result computeCurrentVolumeCapacity(Length vehicleLength, int operation);
    Result computeCurrentPassRateCapacity(Length vehicleLength);

	int getMyCurrentVolume() const {return myCurrentVolume;}
	int getCurrentRoom() const;
	Length getCurrentRoomLength() const;
	bool checkInputRoom(int vol);
	bool checkInputRoomLength(Length vehlen);

	int getMyActivationCount() const {	return myActivationCount;}
	bool isActive(){return myActivationCount > 0;}

	void increaseActivationCount(int val=1);
	void decreaseActivationCount(int val=1);

	// @brief: get the next cell "veh" of this cell should go
	// candidates - 1) same link's consecutive cell 2) next link's cell
	CellInterface* getNextCellOnInputRoute(VehicleInterface* veh, Link* nextLinkOnVehRoute);
	CellInterface* getNextValidCell(VehicleInterface* veh);
	CellInterface* getNextValidCellWithCache(VehicleInterface* veh);

	// @brief: get traffic signal to go the next cell
	// intra-link -> always return "GREEN"
	// inter-link -> get trafficSignal(this link, next link)
	TrafficSignalState getTrafficSignalState(CellInterface* nextCell);

	Link* getMyLink(){return myLink;}
	// [Adding] used by class Analyzer (as identity info. of cell)
	int getSection() {
		return mySection;
	};
	int getLane() {
		return myLane;
	};

	float getMyLength() {
		return myLength;
	};
	string getID(){ return myID; }

	Speed getSpeedLimit();


	long getMyCurrentWaitingVolume() {
		return myWaitingQ.size();
	}
	long getMyCurrentRunningVolume() {
		return myRunningQ.size();
	}

	long getMyCurrentPendingVolume() {
		return myPendingQ.size();
	}
	long getMyCurrentReceivingVolume() {
		return myReceivingQ.size();
	}

	void determineCellSeries(VehicleInterface* veh);
	// passing information
	void recordVehicleEntering(VehicleInterface* veh,SALTTime tEnter);
	void recordVehicleWaiting(VehicleInterface* veh,SALTTime tWait);
	void recordVehicleLeaving(VehicleInterface* veh,SALTTime tLeave);
	void eraseVehicelPassingInfo(VehicleInterface* veh);
	void clearVehiclePassingInfo();
	void clearVehiclePassingInfo(SALTTime currentStep, int periodLength);

	void updateVehicleSpeed(VehicleInterface* veh);
	void updateVehicleSpeedV3(VehicleInterface *veh);

	// output related functions
	void computePeriodicValue(SALTTime currentStep, int periodLength);
    void computePeriodicValueV3(SALTTime currentStep, int periodLength);
	void computeSpeed(SALTTime currentStep, int periodLength);
	// the speed metric used by SEOUL city
	void computeSpeedV2(SALTTime currentStep, int periodLength);
    Speed computeVehicleSpeed(VehicleInterface* veh);
    Speed computeVehicleSpeedV3(VehicleInterface* veh);
    void computeVehicleNumber(SALTTime currentStep, int periodLength);
    void computeVehicleNumberV3(SALTTime currentStep, int periodLength);
	void computeWaiting(SALTTime currentStep, int periodLength);


    //
	float getAverageSpeed() const {	return myAverageSpeed;}
    float getAverageVehicleSpeed();
	//
	float getAverageDensity() const {return myAverageDensity;}
	float getAverageNumVehs() const {return myAverageNumVehs;}
	int getNumVehPassed() const{return myNumVehicleHavePassed;}
	//
	float getAverageWaitingQLength() const{	return myAverageWaitingQLength;}
	float getAverageWaitingTime() const{	return myAverageWaitingTime;}

	void resetLastUpdateInfo();


	void printDebugInfo(SALTTime _step);

	float getSumTravelLength() const {	return mySumTravelLength;	}
	float getSumTravelTime() const {	return mySumTravelTime;	}


	//@libsalt get information waiting vehicles (average waiting time / the number from last TLS Phase switching time to current step)
	int getNumVehWaited() {  return myNumWaitingVehicle;  };
	float getAverageVehicleWaitingTime() {  return myAverageVehWaitingTime;  };
    float getAverageVehicleWaitingQLength() {  return myAverageVehWaitingQLength;  };
    tuple<float,float> getVehicleWaitingTimeBaseVeh(SALTTime currentStep);
//    float getVehicleWaitingTimeBaseVehForLink() {  return myVehWaitingTimeBaseVehForLink;  };
    int getWaitVehStateOverTLSDuration(SALTTime currentStep);
	tuple<float,float,float> computeWaitingVehicle(SALTTime currentStep, SALTTime lastSwitchingTime);
    void computeWaitingTimeBaseVehicle(SALTTime currentStep);



	int getNumVehicles();
	std::vector<std::string> getVehicles();

protected:

//	#define DEBUG
#ifdef DEBUG
public:
#endif
	static RandomGenerator* myRandomGenerator;

//    VehicleManager myVehManager = VehicleManager::getInstance();

	static int debug_cnt;
	
	// (constant) parameter given
	Length paramVehicleLength=5.0f;
	Length sumVehicleLength=0.0f;
	Length accumulatedDistBtwVehs=0.0f;
	Length expectedVehicleDist=0.0f;
	Length myVirtualLength=0.0f;


	//	(constant) cell fixed attriutes
	Link* myLink=0;
	float myLength=-1;
	int mySection=-1;
	int myLane=-1;

	bool myLastSectionFlag = false;
	bool myFirstSectionFlag = false;
	bool myExceptionFlag = false;
	
	string myID = ""; // < format: LinkID_SectionID_LaneID // e.g. link123_3_2

	
	bool myOutFlowCellFlag = false; // myLastSectionFlag and myLink->isOutflowLink()

	bool myPocketFlag = false; 

	string myInfo = "";


	int myPassRateCapacity = -1; // floor( speed_limit / veh_length)
	int myVolumeCapacity = -1; // floor( cell_length / veh_length)

	// (variable) status summary
	int myCurrentVolume= 0;
	int myActivationCount = 0;


	// @update myCurrentVolume+(sent-received)tryVehSend function compute these values & tryVehReceive function reference and decrease
	int myNumVehSent=0;
	int myNumVehReceived=0;
	//
	//
	int myVirtualNumVehReceiveCapacity=0; // round(current vacancy + random_function(numVehSent))

	// @these measures are computed via myVehiclePassingInfo (tEnter, tWait, tLeave)
	// @these measures are reset per observation period
	// @@1 speed
	float myAverageSpeed=-1;
	float mySumTravelLength = 0;
	float mySumTravelTime = 0;

	// @@2 number of vehicle have passed during observation period
	int myNumVehicleHavePassed=0;
	float myAverageNumVehs = -1;
	float myAverageDensity = -1;
	// @@3 average waiting length
	float myAverageWaitingQLength = 0.0f;
	float myAverageWaitingTime 	= 0.0f;

	//@libsalt wating vehicle information
	int myNumWaitingVehicle = 0;
	float myAverageVehWaitingTime = 0.0f;
	float myAverageVehWaitingQLength = 0.0f;
	float myAverageVehWaitingTimeBaseVeh = 0.0f;
	float myVehWaitingTimeBaseVehForLink = 0.0f;
	float myVehWaitingTimeSumBaseVeh = 0.0f;
	int myWaitVehOverTLSDuration = 0;


        // (variable) queue
	//	list<VehicleInterface*> myVehicleQ;
	//	 tuple<veh, enterTime(:=the time when the vehicle has enters)
	//					,expectedLeavingTime(:=time when veh can leave)>
	// queue of vehicle that has entered and is passing this cell
	list<RunTupleType> myRunningQ;

	// queue of vehicle that has passed this cell but waits to leave until condition meets
	// tuple <veh, the time when the vehicle has started waiting>
	list<WaitTupleType> myWaitingQ;

	// queue of vehicle that wants to enter this cell
	// tuple <veh, the time when the vehicle has started pening>
	list<PendTupleType> myPendingQ;


	// queue of vehicle that wants to enter this cell
	// tuple <veh, the time when the vehicle has started pending, prevCell>
	list<ReceiveTupleType> myReceivingQ;

	// map[veh id] =  <entered time, time to start waiting, leaved time>
	map<string, tuple<SALTTime,SALTTime, SALTTime>> myVehiclePassingInfo;
	// map[veh] =  <entered time, time to start waiting, leaved time>
//	map<VehicleInterface*, tuple<SALTTime,SALTTime, SALTTime>> myVehiclePassingInfo;
    map<string, Length> myVehicleLenInfo;


};
}
#endif /* CELLINTERFACE_H_ */
