/*
 * NetworkManager.h
 *
 *  Created on: 2018. 1. 30.
 *      Author: SDJ
 */

#ifndef NETWORKMANAGER_H_
#define NETWORKMANAGER_H_

//#include <iostream>

#include <Object/Network/CellInterface.h>
#include <Object/Network/Connection.h>
#include <Object/Network/Link.h>
#include <Object/Network/Node.h>
#include <utils/config.h>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace SALT{

class SimScenarioInfo;

class NetworkManager {
private:
	static NetworkManager* instance;

	NetworkManager(){};

public:
	static NetworkManager* getInstance(); // access to NetworkManager (singleton)

	Result build(SimScenarioInfo* scenario);

	// core function
	Result build(string const& _node_file, string const& _link_file, string const& _connection_file, string const& _input_type="SALT");

	virtual ~NetworkManager();

public:
    void clear();
	Result registerNode( Node* _node);
	Result registerLink( Link* _link);
	Result registerConnection(Connection* _connection);

	//	assign link->prevLinkList, nextLinkList according to connection info
	Result buildLinkGraph(void);
	// generate cell based on lane, section, cell params
	Result buildCellGraph(void); 
	
	// status
	string print(void);
	void printStatus(SCOPE _scope=MACRO);

	// util
	Node* findNodeByID(string id);
	Link* findLinkByID(string id);

	// active cell mngt
	void insertActiveCell(CellInterface* _cell);
	void refreshActiveCellSet();
	const bool testActiveSet() const;

	// @return sample value from uniform distribution [minCellLength, cellLength]
	int getRandomLength(int cellLength);
	bool getTrueWithProb(float prob);
	int getRandomNumber(int low,int high);

	// information needed for visualization
	string getNodeShape(string nodeID);
	string getNodeShape(Node* _node);
	string getLinkShape(string linkID);
	string getLinkShape(Link* _link);

	map<string, string> getNodeShapeList();
	map<string, string> getLinkShapeList();

	// getter
	const std::vector<Node*>& getNodeList() const { return myNodeList;	}
	const std::vector<Link*>& getLinkList() const {	return myLinkList;	}
	const std::vector<Connection*>& getConnectionList() const {	return myConnectionList; }
	const std::vector<CellInterface*>& getCellList() const { return myCellList;	}
	
	const set<CellInterface*>& getMyActiveCellSet() const {	return myActiveCellSet;	}

	const std::vector<Link*>& getValidLinkList() const{	return myValidLinkList;	}
	const std::vector<CellInterface*>& getValidCellList() const{ return myValidCellList; }
		
	void importVehicleDump(string path_to_dump);
private:
	// obj list
	vector<Node*> myNodeList;
	vector<Link*> myLinkList;
	vector<Connection*> myConnectionList;
	vector<CellInterface*> myCellList;

	// valid link, cell list
	vector<Link*> myValidLinkList;
	vector<CellInterface*> myValidCellList;

	// obj getter using id
	map<string, Node*> myNodeMap;
	map<string, Link*> myLinkMap;

	// active cell is a cell that has at least one vehicle.
	set<CellInterface*> myActiveCellSet;

	Length myMinCellLength = -1; // initialized by scenario
	//	Length maxCellLength = 30.0; // not used.
	Length myVehicleLength = -1;

	// FIXME - convert to global variable
	RandomGenerator* myRandomGenerator = NULL;
};

ostream& operator << (ostream& strm, NetworkManager& obj);
}
#endif /* NETWORKMANAGER_H_ */
