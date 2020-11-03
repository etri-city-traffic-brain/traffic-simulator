/*
 * ParserInterface.h
 *
 *  Created on: 2018. 8. 17.
 *      Author: SDJ
 */

#ifndef INCLUDE_UTILS_PARSERINTERFACE_H_
#define INCLUDE_UTILS_PARSERINTERFACE_H_

#include <utils/config.h>
#include <string>


namespace SALT{
class TrafficSignalManager;
class VehicleManager;
class NetworkManager;



class ParserInterface{
public:
	ParserInterface(){};
	//ParserInterface(NetworkManager* _NM, VehicleManager* _VM, TrafficSignalManager* _TM): myNM(_NM), myVM(_VM), myTM(_TM){};
	virtual ~ParserInterface(){};

	virtual Result buildNode(string const& _nodeXML, NetworkManager* networkManagerToFill)=0;
	virtual Result buildLink(string const& _linkXML, NetworkManager* networkManagerToFill)=0;
	virtual Result buildConnection(string const& _connectionXML, NetworkManager* networkManagerToFill)=0;

	virtual Result buildRoute(string const& _routeXML, SALTTime beginStep,
			SALTTime endStep, VehicleManager* vehicleManagerToFill, NetworkManager* networkManagerToRef)=0;

	virtual Result loadRouteDynamically(string const& _routeXML, SALTTime interval_start, SALTTime interval_end, SALTTime offset_read_time, VehicleManager* vehicleManagerToFill, NetworkManager* networkManagerToRef)=0;

	virtual Result buildTLS(string const& _tlsXML, TrafficSignalManager* TrafficSignalManagerToFill, NetworkManager* NetworkManagerToRef)=0;
protected:
//	NetworkManager* myNM=0;
//	VehicleManager* myVM=0;
//	TrafficSignalManager* myTM=0;
};
}


#endif /* INCLUDE_UTILS_PARSERINTERFACE_H_ */
