/*
 * ParserSUMO.h
 *
 *  Created on: 2018. 7. 18.
 *      Author: SDJ
 */

#ifndef SRC_UTILS_PARSERSUMO_H_
#define SRC_UTILS_PARSERSUMO_H_

#include <utils/config.h>
#include <utils/ParserInterface.h>
#include <string>

namespace SALT{

class NetworkManager;
class VehicleManager;
class TrafficSignalManager;

class ParserSUMO : public ParserInterface{
public:
	ParserSUMO(){};
	//ParserSUMO(NetworkManager* _NM, VehicleManager* _VM, TrafficSignalManager* _TM): myNM(_NM), myVM(_VM), myTM(_TM){};
	virtual ~ParserSUMO(){};

	virtual Result buildNode(string const& _node_json, NetworkManager* networkManagerToFill);
	virtual Result buildLink(string const& _link_json, NetworkManager* networkManagerToFill);
	virtual Result buildConnection(string const& _connection_json, NetworkManager* networkManagerToFill);

	virtual Result buildRoute(string const& _route_json, SALTTime beginStep,
			SALTTime endStep, VehicleManager* vehicleManagerToFill, NetworkManager* networkManagerToRef);
	virtual Result loadRouteDynamically(string const& _routeXML, SALTTime interval_start, SALTTime interval_end, SALTTime offset_read_time, VehicleManager* vehicleManagerToFill, NetworkManager* networkManagerToRef);
	virtual Result buildTLS(string const& _tls_json, TrafficSignalManager* trafficSignalManagerToFill, NetworkManager* networkManagerToRef);

	Result parseTLS(string const& _tls_json, TrafficSignalManager* trafficSignalManagerToFill, NetworkManager* networkManagerToRef);


};
}
#endif /* SRC_UTILS_PARSERSUMO_H_ */
