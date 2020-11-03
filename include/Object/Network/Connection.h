/*
 * Connection.h
 *
 *  Created on: 2018. 1. 29.
 *      Author: SDJ
 */

#ifndef OBJECTT_CONNECTION_H_
#define OBJECTT_CONNECTION_H_

#include <list>
#include <string>
#include <utils/config.h>


namespace SALT{

class Node;
class Link;
class CellInterface;
class VehicleInterface;

class Connection {
public:
	Connection();
	//	Connection(Lane* _fromLane, Lane* _toLane);
	Connection(Link* _from, Link* _to, int _myFromLane, int _myToLane, int _myLinkIndex=-1, string info="");
	virtual ~Connection();

	Link* getFromLink() {
		return myFromLink;
	}
	Link* getToLink() {
		return myToLink;
	}

	int getFromLane() {
		return myFromLane;
	}
	int getToLane() {
		return myToLane;
	}
	int getLinkIndex(){
		return myLinkIndex;
	}

	const Vector2D& getDirection() const {
		return myDirection;
	}
	const std::string getRotationDir() const{
		return myRotationDir;
	}

	bool testCorrespond(Link* _from, Link* _to, int _FromLane, int _ToLane);



private:
	Link* myFromLink=0;
	Link* myToLink=0;
	int myFromLane=-1;
	int myToLane=-1;

	int myLinkIndex = -1;

//	Node* myNode=0;

	// myDirection = fromLink's Direction + toLink's Direction
	Vector2D myDirection;
	std::string myRotationDir="s"; // r: right-turn, l: left-turn, s: straight

//	CellInterface* myFromCell=0;
//	CellInterface* myToCell=0;


	list<VehicleInterface*> myOnVehicleList;
	list<VehicleInterface*> myWaitingVehicleList;
public:
	string myInfo="";
};
}
#endif /* OBJECTT_CONNECTION_H_ */
