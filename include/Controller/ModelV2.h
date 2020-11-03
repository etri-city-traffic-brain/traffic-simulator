/*
 * ModelSimpleMeso.h
 *
 *  Created on: 2018. 4. 24.
 *      Author: SDJ
 */

#ifndef MODELMESOVERSION2_H_
#define MODELMESOVERSION2_H_

#include <Controller/ModelInterface.h>
#include <utils/config.h>
#include <list>
#include <tuple>

namespace SALT {

class VehicleInterface;
namespace version2{

class Model : public SALT::ModelInterface{
public:
	Model(NetworkManager* _nm, VehicleManager* _vm, TrafficSignalManager* _tsm, OutputWriter* _ow) : ModelInterface(_nm,_vm,_tsm, _ow){};
	virtual ~Model();

	
	

	// update main
	virtual void update(SALTTime _step);

	// update phase 1
	virtual void updateBeforeSync(SALTTime _step);

	// in standalone doSync function is not needed
#ifdef DISTRIBUTED
	// upate phase 2 sync function  with another simulation 
	virtual void doSync(SALTTime _step, std::vector<salt_comm::SyncDataElement>& importingSyncDataElementList);
#endif

	// update phase 3
	virtual void updateAfterSync(SALTTime _step);


	// dump/import
	virtual void doDump(SALTTime import_step);
	virtual void import(SALTTime import_step, string path_to_dump);
	// version specific dump/import
	void fillupVehicleManager(string path_to_dump);
	void deployVehicles(SALTTime import_step); // not implemented yet

	// util
	virtual void print(SALTTime _step, SCOPE _scope);
	
	// this version specific function
	void planVehicleInsertion(SALTTime _step, list<VehicleInterface*>& listVehToInsert);

	// @distributed
	void sendSyncData(list<tuple<VehicleInterface*, SALTTime> >& syncData); // send a list of SyncData to Master
	void receiveSyncData(list<tuple<VehicleInterface*, SALTTime> >& syncData); // receive a list of SyncData from Master
	
	const string& getMyMasterIp() const { return myMasterIP;	}
	void setMasterIp(const string& masterIP) { 	myMasterIP = masterIP;	}
	
	void setDistributedMode(){ 		distributedMode = true; 	}
	bool isDistributedMode(){ 	return distributedMode; 	}

private:
	// @distributed
	string myMasterIP="";
	bool distributedMode=false;
};
}

}
#endif /* MODELMESOVERSION2_H_ */
