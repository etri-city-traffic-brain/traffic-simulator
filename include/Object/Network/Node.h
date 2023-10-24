/*
 * Node.h
 *
 *  Created on: 2018. 1. 29.
 *      Author: SDJ
 */

#ifndef OBJECTT_NODE_H_
#define OBJECTT_NODE_H_

#include <utils/config.h>
#include <map>
#include <vector>
#include <list>
#include <set>



namespace SALT{
class TrafficSignal;
class Connection;
class Link;
class Node {
public:
	Node() {};
	Node(ID _ID, TypeName _type="priority") : myID(_ID), myIntersectionType(_type){};
	Node(ID _ID, float x=0,float y=0, TypeName _type="priority");
	virtual ~Node();
	ID getID(){ return myID; };
	string getStatusString();
	float computeDistance(const Node & v) const;
	Vector2D computeLinkDirection(const Node & v) const;

	const Position& getMyLocation() const {
		return myLocation;
	}

    const TypeName& getMyIntersectionType() const {
        return myIntersectionType;
    }

	TrafficSignal* getMyTrafficSignal() {
		return myTrafficSignal;
	}

	void setMyTrafficSignal(TrafficSignal* ts) {
		myTrafficSignal=ts;
	}
	const std::vector<Connection*>& getOrderedConnectionList() const {
		return myOrderedConnectionList;
	}

	//@brief: return "x,y"
	string getShape();

	// @brief: return the string that encodes traffic signal state (e.g. "GGggrryy") in the current simulation step
	string getCurrentTrafficSignalState();

	// @brief: insert Connection instance to myUnorderedConnectionList
	void addConnection(Connection* connection);
	// @brief: search the connection that satisfying a given condition
	Connection* findConnection(Link* fromLink, Link* toLink, int fromLane, int toLane);
	// @brief: record the pair <the order of Connection, Connection itself)
	void registerConnectionOrder(int order, Connection* connection);
	// @brief: construct ordered connection list after all registerConnectionOrder functions needed are called
	void constructOrderedConnectionList();

	const std::set<Connection*>& getMyConnectionSet() const {
	    return myConnectionSet;
	}

	bool isFromOtherWorker() const {
		return myFromOtherWorker;
	}

	void recordOtherWorker() {
		this->myFromOtherWorker = true;;
	}

	const std::string& getOwner() const { return myOwner; }

	void setOwner(const std::string& owner = "") {
		myOwner = owner;
	}

	const std::map<int, Connection*>& getMapOrderConnection() const {
		return myMapOrderConnection;
	}

private:

	ID myID;
	Position myLocation;
	TypeName myIntersectionType;
	TypeName myRole;

	bool myFromOtherWorker=false;
	std::string myOwner=""; //sub-sim id // partitionID

	std::set<Connection*> myConnectionSet; // connectionArray[x]'s signal state is assigned the (x)th character of the node's signal state

	TrafficSignal* myTrafficSignal=nullptr;
	std::vector<Connection*> myOrderedConnectionList; // connectionArray[x]'s signal state is assigned the (x)th character of the node's signal state
	std::map<int,Connection*> myMapOrderConnection;


};
}
#endif /* OBJECTT_NODE_H_ */
