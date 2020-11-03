/*
 * ParserSALT.h
 *
 *  Created on: 2018. 7. 26.
 *      Author: SDJ
 */

#ifndef SRC_UTILS_PARSERSALT_H_
#define SRC_UTILS_PARSERSALT_H_

#include <utils/config.h>
#include <utils/ParserInterface.h>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace pt = boost::property_tree;

namespace SALT{

class NetworkManager;
class VehicleManager;
class TrafficSignalManager;

class ParserSALT : public ParserInterface{
public:
	ParserSALT() {};
	//ParserSALT(NetworkManager* _NM, VehicleManager* _VM, TrafficSignalManager* _TM): myNM(_NM), myVM(_VM), myTM(_TM){};
	virtual ~ParserSALT(){
	    fRoute.close();
	};
	virtual Result buildNode(string const& _nodeXML, NetworkManager* networkManagerToFill);
	virtual Result buildLink(string const& _linkXML, NetworkManager* networkManagerToFill);
	virtual Result buildConnection(string const& _connectionXML, NetworkManager* networkManagerToFill);

	virtual Result buildRoute(string const& _routeXML, SALTTime beginStep,
			SALTTime endStep, VehicleManager* vehicleManagerToFill, NetworkManager* networkManagerToRef);
	virtual Result loadRouteDynamically(string const& _routeXML, SALTTime interval_start, SALTTime interval_end, SALTTime offset_read_time,  VehicleManager* vehicleManagerToFill, NetworkManager* networkManagerToRef);
	virtual Result buildTLS(string const& _tlsXML, TrafficSignalManager* trafficSignalManagerToFill, NetworkManager* networkManagerToRef);

	Result parseTLS(string const& _tlsXML, TrafficSignalManager* trafficSignalManagerToFill, NetworkManager* networkManagerToRef);

	Result loadRouteXML(string const& _routeXML, SALTTime beginStep,
			SALTTime endStep, VehicleManager* vehicleManagerToFill, NetworkManager* networkManagerToRef);

private:
    //pt::ptree reader;
    //pt::ptree vehicles;
    VehicleInterface* nextVeh = nullptr;
    int numLoaded = 0;
    ifstream fRoute;
};
}
#endif /* SRC_UTILS_PARSERSALT_H_ */
