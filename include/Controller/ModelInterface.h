/*
 * ModelInterface.h
 *
 *  Created on: 2018. 3. 29.
 *      Author: SDJ
 */

#ifndef MODELINTERFACE_H_
#define MODELINTERFACE_H_

#ifdef DISTRIBUTED
#include <comm/SyncMessage.hpp>
#endif // end of #ifdef DISTRIBUTED

#include <utils/config.h>
#include <Object/Output/OutputWriter.h>
#include <vector>

namespace SALT{

class VehicleInterface;
class NetworkManager;
class TrafficSignalManager;
class VehicleManager;
class OutputWriter;


class ModelInterface {
	// @brief: interface class to update the states of simulation agent and object 
public:
	ModelInterface(NetworkManager* _nm, VehicleManager* _vm, TrafficSignalManager* _tsm, OutputWriter* _ow) : myNetworkManager(_nm), myVehicleManager(_vm), myTrafficSignalManager(_tsm), myOutputWriter(_ow){};
	virtual ~ModelInterface()=0;

	
	// update main
	virtual void update(SALTTime _step)=0;
	
	// update phase 1
	virtual void updateBeforeSync(SALTTime _step)=0;

	// in standalone doSync function is not needed
#ifdef DISTRIBUTED
	// upate phase 2 sync function  with another simulation 
	virtual void doSync(SALTTime _step, std::vector<salt_comm::SyncDataElement>& importingSyncDataElementList)=0;
#endif

	// update phase 3
	virtual void updateAfterSync(SALTTime _step)=0;

	// dump/import
	virtual void doDump(SALTTime import_step)=0;
	virtual void import(SALTTime import_step, string path_to_dump)=0;

	// util
	virtual void print(SALTTime _step,SCOPE _scope=MACRO)=0;
	
protected:

	static RandomGenerator* myRandomGenerator;
	NetworkManager* myNetworkManager;
	VehicleManager* myVehicleManager;
	TrafficSignalManager* myTrafficSignalManager;
	OutputWriter* myOutputWriter;
};

}
#endif /* MODELINTERFACE_H_ */
