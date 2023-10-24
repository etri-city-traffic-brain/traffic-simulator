/*
 * VehicleInterface.h
 *
 *  Created on: 2018. 3. 13.
 *      Author: SDJ
 */

#ifndef VEHICLEINTERFACE_H_
#define VEHICLEINTERFACE_H_

#include <utils/config.h>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <utility>


namespace SALT{
class Route;
class CellInterface;
class Link;
class TrafficSignal;

class VehicleInterface {
public:
	VehicleInterface();
    VehicleInterface(Route* _route, string _departTime, int _routeIndex,  ID _ID);
	VehicleInterface(Route* _route, string _departTime, int _routeIndex,  ID _ID, VehicleClass _class);
	virtual ~VehicleInterface();

	// update function
	virtual void updateStatus(SALTTime nextTime)=0;
	void updateStatusWhenNewCell();
	void updateStatusWhenNewLink();
	void handleStartingLinkException();

	void clearCellSeries();
	void appendCellPosition(int section, int lane);
	const pair<int, int>& getNextCellPosition();

	// getter / setter
	ID& getMyName()  {	return myID; }
	string& getMyShape()  {	return myShape;	}
	SALTTime getMyDepartTime() const {	return myDepartTime;}
	int getMyCellIndex() const {return myCellIndex;	}
	string getRouteString() const;
	int getNextOffsetOfLocalRoute() const;


	Link* getLink(int linkIndex);
	Link* getRouteDepartingLink();
	//	CellInterface* getRouteDepartingCell();
	Link* getNextValidLink();

	Link* getCurrentLink() const;
	//	CellInterface* getCurrentCell();

//	Link* getNextLink(Link* link);
	Link* getJustNextLink();
	Link* getNextLink(int hop);

	Length getMyLength() { return myLength;}

    void setMySpeed(Speed speed) {
	    this->mySpeed = speed;
	}

	Speed getMySpeed() { return mySpeed; }
	VehicleClass getMyClass() {
	    return myClass;
	}

	void setMyCellIndex(int myCellIndex = -1) {
		this->myCellIndex = myCellIndex;
	}

	void setMyLinkIndex(int myLinkIndex = -1) {
		// FIXME range check
		this->myLinkIndex = myLinkIndex;
	}

	void appendMyCellIDMap(int curLinkIndex);

	// utility
	string print(void);
	string getStatusString(void);

	int getMyRouteDepartingLaneIndex() const {
		return myRouteDepartingLaneIndex;
	}

	int getMyRouteDepartingSectionIndex() const {
		return myRouteDepartingSectionIndex;
	}

	bool isOnLastLocalRoute() const;
	string getRemainingRouteOnNextWorker() const;

	tuple<string, string> getboundaryMapKey() const;

	Link* getLastLinkOnLocalRoute() const;

	string getBasicInfo() const;

	const list<Link*>& getMySubNextLinkListCache() const {
		return mySubNextLinkListCache;
	}

	void setMySubNextLinkListCache(const list<Link*>& mySubNextLinkListCache) {
		this->mySubNextLinkListCache = mySubNextLinkListCache;
	}

	CellInterface* getCurrentCell() const {
		return this->myCurrentCell;
	}
	CellInterface* getNextValidCellCache() const {
		return this->myNextValidCell;
	}
	void setNextCellCache(CellInterface* _current, CellInterface* _next){
		this->myCurrentCell = _current;
		this->myNextValidCell = _next;
	}

    // passing information
    void recordEntering(tuple<SALTTime,SALTTime,SALTTime>tEnterTuple);
    void recordWaiting(tuple<SALTTime,SALTTime,SALTTime>tWaitTuple);
    void recordLeaving(tuple<SALTTime,SALTTime,SALTTime>tLeaveTuple);
    void managePassingInfo();
//    void clearPassingInfo(SALTTime currentStep, int periodLength);

    tuple<float,float> computeWaiting(SALTTime curStep);

    bool isSpecificLinkInMyRoute(ID linkid);

protected:
	//	(constant) fixed values during simulation
	//public:
	ID myID;
	//string myType="";
	VehicleClass myClass = PASSENGER; // VehicleClass: Passenger or Bus
	string myShape="";
	// TODO - input file doesn't have the below info(length, max speed)
	Length myLength = 5.0; // Passenger: 5m, Bus: 11m

	// max speed is deprecated because
	// almost vehicle can move with 100km/s, and
	// in urban city, speed limits are under 60km/s
	//	Speed myMaxSpeed = 16.6; // static 60km/h -> 16.6m/s


	// (constant) fixed values during sub-simulation
	Route* myRoute=0;
	string myRouteString="";

	SALTTime myDepartTime=-1;
	int myRouteDepartingLinkIndex = -1;
	int myRouteDepartingSectionIndex = -1;
	int myRouteDepartingLaneIndex = -1;


	// (variable) status - position, speed
	int myLinkIndex = -1; // [0,myRoute.size()]
	int myCellIndex = -1; // [0,myCellSeries.size()] //myCellSeries.size()<=link's #sections
	list<pair<int,int>> myCellSeries;
	// Speed mySpeed=-1;
    Speed mySpeed = 0;


	list<Link*> mySubNextLinkListCache; // [ current link, ... next valid link]
//	tuple<CellInterface*, CellInterface*> myNextValidCellCache; // <current cell, next valid cell>
	CellInterface* myCurrentCell=nullptr;
	CellInterface* myNextValidCell=nullptr;

	map<int,map<int,tuple<SALTTime,SALTTime,SALTTime>>> myPassingInfo;
    // map[link_index][cell_index] = <entered time, time to start waiting, leaved time>
};
ostream& operator << (ostream& strm, VehicleInterface& obj);
}
#endif /* VEHICLEINTERFACE_H_ */
