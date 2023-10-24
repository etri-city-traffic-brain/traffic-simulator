/*
 * Route.h
 *
 *  Created on: 2018. 1. 29.
 *      Author: SDJ
 */

#ifndef OBJECTT_ROUTE_H_
#define OBJECTT_ROUTE_H_

#include <string>
#include <vector>

#include <utils/config.h>



//////////////////////////////////////////////////////////
// XXX - consideration
// Should Route class have departing position(link, lane, offset) of vehicle?
//////////////////////////////////////////////////////////
namespace SALT{

class NetworkManager;
class Link;

class Route {
public:
	Route();
	Route(string _linkListString, NetworkManager* networkManagerToRef, int _localOffsetFromGlobal=0, string _departLane="");
	Route(vector<Link*> _linkList);

	~Route();

	Link* getLink(int index);

	//	Link* getDepartingLink();
	Link* getNextLink(Link* link);
	Link* getNextLink(int from_index);

	//Link* getNextValidLink(Link* link);
	Link* getNextValidLink(int from_index);
	int getOffsetToNextValidLink(int from_index);
	bool validateConnectivity();
	bool isAllInvalidLink();
	bool isSimulatable(){ return flagSimulatable; }
	string getGlobalRouteString() const { return myGlobalRouteString;}

	// @ brief: compute next localOffsetFromGlobal used in the next worker.
	// return ( localOffsetFromGlobal + #(localRoute's valid link) )
	// this value is used in the next worker that receive the vehicle following this route.
	void setNextLocalRouteOFfset(int nextOffset){
		nextLocalRouteOffset=nextOffset;
	}
	int getNextLocalRouteOffset() const { return nextLocalRouteOffset;}

	// determine when to call popVeh()
	// if last local route and last link -> call popVeh()
	bool isLastLocalRoute() { return flagLastLocalRoute;}
	void setLastLocalRoute(){	flagLastLocalRoute = true;	}

	bool isSpecificLinkInRoute(string linkid);


	// @brief: given global route and the local offset, build localRoute
	// @return: success or failure to build local route
	Result buildLocalRoute(string _linkListString, NetworkManager* networkManagerToRef);
	string getRemainingRoute(int startIndexOfRemainingRoute);

	Link* getLastLinkOnLocalRoute();
//private:
	// [fixed during total simulation]
	vector<string> myGlobalRoute;
	string myGlobalRouteString;

	// [fixed during sub simulation, but varying per sub simulation]
	int myLocalRouteOffset=0;
	//	// a sub-simulation doesn't have Link* belonging to other sub-simulation!!
	vector<Link*> myLocalRoute; // <- defined by myGlobalRoute and localOffsetFromGlobal
	int nextLocalRouteOffset=-1; // := myLocalOFfsetFromGlobal + |myLocalRoute|
	bool flagLastLocalRoute = false;

	// XXX - issue
	int myDepartLane=0;

	bool flagSimulatable=true;

	// startingOffset is deprecated. not gonna use the 'offset of a single link'
	// XXX - myStartingIndex is deprecated, because Vehicle has its current position



	static RandomGenerator* myRandomGenerator;

};
}
#endif /* OBJECTT_ROUTE_H_ */
