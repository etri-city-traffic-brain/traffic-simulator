/*
 * Link.h
 *
 *  Created on: 2018. 1. 29.
 *      Author: SDJ
 */

#ifndef OBJECTT_EDGE_H_
#define OBJECTT_EDGE_H_

#include <utils/config.h>
#include <list>
#include <map>


namespace SALT{
class Node;
class Connection;
class CellInterface;
class VehicleInterface;

class Link {
public:
	Link();
	Link(Node* _fromNode, Node* _toNode);
//	Link(ID _ID, Node* _fromNode, Node* _toNode, int _numLanes, string _info);
//	Link(ID _ID, Node* _fromNode, Node* _toNode, int _numLanes, string shape, string spreadType, string _info, float _len=-1.0f);
//	Link(string _ID, string _fromNode, string _toNode, string _numLanes, string _info);
//	Link(ID _ID, Node* _fromNode, Node* _toNode, int _numLanes,	string shape, float _speedLimit, string spreadType, string _info, float _len=-1.0f);
	Link(ID _ID, Node* _fromNode, Node* _toNode, int _numLanes,	string shape, float _speedLimit, string spreadType, string _info, float _len=-1.0f, int _left_pocket=-1, int _right_pocket=-1);
	virtual ~Link();

	void addNextLink(Link* _link, Connection* _connection);
    std::vector<Link*> getNextLinks();
	void addCell(int sectionIdx,int lane, CellInterface* _cell);

	CellInterface* getCellByIndex(int sectionIdx, int lane);
	void determineCellSeries(VehicleInterface* veh, int departingCellRow, int departingCellCol, Link* nextLink);


	// functions that manages vehicle on link
	CellInterface* insertVehicle(VehicleInterface* veh, SALTTime t,int lane);
	CellInterface* pendVehicle(VehicleInterface* veh, SALTTime t,int lane);
	void appendOnVeh(VehicleInterface* veh);
	void removeOnVeh(VehicleInterface* veh);

	// traffic signal related functions
	TrafficSignalState getTrafficSignalState(Link* nextLink);
	Connection* getAnyConnectionTo(Link* nextLink);
	Connection* getConnectionTo(Link* nextLink, int fromLane);
	bool canReach(Link* nextLink);

	string getStatusString();

	// @Getter functions for rendering
	ID getID(){ return myID; }
	Node* getFromNode(){return myFromNode;	}
	Node* getToNode(){	return myToNode;	}
	// link[section_index][lane_index] is cell
	int getNumLane(){ return myNumLanes; }
	int getNumSection() const {	return myNumSections;	}
	SpreadType getSpreadType() const {	return mySpreadType;	}

	Length getLength() const {	return myLength;	}

	//@brief returns the series of coordinates
	// every gap b\w coordinates is blank space(i.e. " ")
	// @case1 - no optional attribute 'shape' on "link" tag
	//		--> return "x1,y1 x2,y2" (fromNodeShape toNodeShape)
	// @case2 - optional attribute 'shape' exists
	//		--> return "x_from,y_from x_middle1,y_middle1 .... x_to,y_to" (fromNodeShape,myShape,toNodeShape)
	string getShape();

    int getNumVehicles();
    std::vector<std::string> getVehicles();

	void setNumSection(int _numSections = 0) {
		this->myNumSections = _numSections;
	}

	Speed getMySpeedLimit() const {	return mySpeedLimit;}

	bool isExceptionalLink(){return myExceptionFlag;}
	void setAsExceptionalLink(){
		myExceptionFlag = true;
	}

	bool isOutFlowLink() const {
		return myFlagOutFlow;
	}
	bool isSimulatable(){ return flagSimulatable; }

	Vector2D computeConnectionDirection(const Link & l) const;
	std::string computeConnectionRotation(const Link & l) const;

	const Vector2D& getDirection() const {
		return myDirection;
	}

	tuple<string, string> getBorderMapKey() const;

	const map<int, map<int, CellInterface*> >& getCell2DMap() const {
		return myCellMap;
	}


	void computePeriodicValue();
	void computeSpeed();
	void computeSpeedV2();
	void computeVehicleCountAndDensity();
	void computePassed();
	void computeWait();

	//@return tuple(NumOfWaitingVehicles, AverageVehicleWaitingTime, AverageVehicleWaitingQLength)
	tuple<float,float,float> computeVehWait(SALTTime currentStep, SALTTime lastSwitchingTime);

	float getAverageDensity() const {
		return myAverageDensity;
	}

	void setAverageDensity(float myAverageDensity = 0.0f) {
		this->myAverageDensity = myAverageDensity;
	}

	float getAverageSpeed() const {
		return myAverageSpeed;
	}

	void setAverageSpeed(float myAverageSpeed = -1.0f) {
		this->myAverageSpeed = myAverageSpeed;
	}

	float getAverageWaitingQLength() const {
		return myAverageWaitingQLength;
	}

	void setAverageWaitingQLength(float myAverageWaitingQLength = 0.0f) {
		this->myAverageWaitingQLength = myAverageWaitingQLength;
	}

	float getAverageWaitingTime() const {
		return myAverageWaitingTime;
	}

	void setAverageWaitingTime(float myAverageWaitingTime = 0.0f) {
		this->myAverageWaitingTime = myAverageWaitingTime;
	}

	float getSumPassingCount() const {
		return mySumPassed;
	}

	void setSumPassed(float mySumPassingCount = 0.0f) {
		this->mySumPassed = mySumPassingCount;
	}

//	float getNumOfWaitingVehicle() const {
//	    return myNumWaitingVehicle;
//	}
//
//	void setNumOfWaitingVehicle(float myNumWaitingVehicle = 0.0f) {
//	    this->myNumWaitingVehicle = myNumWaitingVehicle;
//	}
//
//	float getAverageVehicleWaitingTime() const {
//	    return myAverageVehWaitingTime;
//	}
//
//	void setAverageVehicleWaitingTime(float myAverageVehWatingTime = 0.0f) {
//	    this->myAverageVehWaitingTime = myAverageVehWatingTime;
//	}
//
//	float getAverageVehicleWaitingQLength() const {
//	    return myAverageVehWaitingLength;
//	}
//
//	void setAverageVehicleWaitingQLength(float myAverageVehWaitingQLength = 0.0f) {
//	    this->myAverageVehWaitingLength = myAverageVehWaitingQLength;
//	}

	//	float getAverageSpeed() const { return myAverageSpeed; }
//
////	float getSumNumVehs() const { return myObservedVehCount; }
////	float getAverageNumVehs() const { return myAverageNumVehs; }
//	float getAverageDensity() const { return myAverageDensity; }
//	float getSumPassingCount() const  { return mySumPassingCount; }
//
//	float getSumWaitingQLength() const { return myObservedWaitingQLength; }
//	float getAverageWaitingQLength() const { return myAverageWaitingQLength; }
//	float getAverageWaitingTime() const { return myAverageWaitingTime; }
	float getSumTravelLength() const {	return mySumTravelLength;	}
	float getSumTravelTime() const {	return mySumTravelTime;	}

	int getMyLeftPocket() const {
		return myLeftPocket;
	}

	int getMyRightPocket() const {
		return myRightPocket;
	}

private:
	static RandomGenerator* myRandomGenerator;
	ID myID;

	Node* myFromNode=nullptr;
	Node* myToNode=nullptr;

	Length myLength=-1;
	int myNumLanes=0;

	Speed mySpeedLimit= -1; // (m/s)

	SpreadType mySpreadType=SpreadType::CENTER;

	int myLeftPocket = -1;
	int myRightPocket = -1;

	map<Link*, list<Connection*>> myMapNextLink2Connection;
	map<Link*, list<int>> myMapNextLink2Lane;
	int myNumSections = 0;
	// too short links are exceptional links
	bool myExceptionFlag = false;

	// is this link out-flow (inside-node --> outside-node)
	bool myFlagOutFlow = false;

	// to check data error
	bool flagSimulatable=true;

	// directional (unit) vector of this link
	Vector2D myDirection;

	// @usage: myCellMap[section][lane]
	// lane : rightmost->leftmost / section : upstream->downstream
	map<int,map<int,CellInterface*>> myCellMap;


//	list<VehicleInterface*> myOnVehicleList;

	// traffic state measure
	float mySumPassed=0.0f;
	float myAverageSpeed= -1.0f;
	float myAverageDensity = 0.0f;
	float myAverageWaitingQLength = 0.0f;
	float myAverageWaitingTime = 0.0f;

//	//@libsalt wating vehicle information
//	float myNumWaitingVehicle = 0.0f;
//	float myAverageVehWaitingTime = 0.0f;
//	float myAverageVehWaitingLength = 0.0f;

	// speed version 2
	float mySumTravelLength = 0;
	float mySumTravelTime = 0;

//
//	float myObservedVehCount = 0.0f;
//	float myAverageNumVehs = 0.0f;
//
//
//
//
//	float myObservedWaitingQLength = 0.0f;



public:
	string myShape="";
	string myInfo="";


};
}

#endif /* OBJECTT_EDGE_H_ */
