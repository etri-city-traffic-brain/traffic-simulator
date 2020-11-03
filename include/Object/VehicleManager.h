/*
 * VehicleManager.h
 *
 *  Created on: 2018. 1. 29.
 *      Author: SDJ
 */

#ifndef OBJECTT_VEHICLEMANAGER_H_
#define OBJECTT_VEHICLEMANAGER_H_

#include <Object/Agent/VehicleInterface.h>
#include <utils/config.h>
#include <iostream>
#include <list>
#include <string>

#ifdef DISTRIBUTED
#include <comm/SyncMessage.hpp>
#endif

namespace SALT{
class Link;
class SimScenarioInfo;
class SimulationController;
class ParserInterface;

class VehicleManager {
private:
	static VehicleManager* instance;

	VehicleManager(){};

public:
	virtual ~VehicleManager();

	static VehicleManager* getInstance();

	void setBelongingController(SimulationController* sc) { myBelongingController=sc;}

	// build
	Result build(string const& _route_file, SALTTime beginStep, SALTTime endStep, const string& _input_type="SALT");
	Result build(SimScenarioInfo* scenario, bool is_dynamic=true);
	
	// update
	// control pointer to route input file
	void setMyRouteFileListIndex(int _index){ myRouteFileListIndex=_index;	}
	void increaseMyRouteFileListIndex();
	// load from route input file to Vehicle instance
	Result loadVehiclesDynamically(SALTTime interval_start, SALTTime interval_end);
	// get ready to insert vehs to road network
	void fillDepartingVehicleList(SALTTime curStep, list<VehicleInterface*>& departingVehicleListToFill);
	// filter out unrelated vehs
	void filterOutSimulationTimeRange(SALTTime beginStep, SALTTime endStep);
	
	// vehicle state tracking functions
	void addStandByVehicle(VehicleInterface* veh);
	void appendRunningVeh(VehicleInterface* veh);
	void appendArrivedVeh(VehicleInterface* veh);
	void clearArrivedVeh(VehicleInterface* veh);

	Link* getDepartingLink(VehicleInterface* veh);
    VehicleInterface* getVehicle(const std::string& vehiceID);
	
	// util
	string print(void);
	void printStatus(SCOPE _scope=MACRO);
	string getCurrentVehicleListInfo();

	// export/import (low priority, not fully implemented)
	void exportVehicleList(SALTTime curStep);
	void updateAccordingToDump(string path_to_dump);
	void importVehicleDump(string path_to_dump, NetworkManager* networkManagerToRef);

	list<VehicleInterface*> getStandbyList();
	set<VehicleInterface*> getRunningSet();
	
#ifdef DISTRIBUTED
	// functions for distributed simulation. not necessary to standalone
	// In the distributed context, "export" stands for send vehs to "another simulation"
	// @ Functions to export vehicle to another worker
	const vector<salt_comm::SyncDataElement>& getExportingSyncDataElmentList() const {
		return exportingSyncDataElementList;
	}
	void registerExportVehicle(VehicleInterface* veh, SALTTime tSent, Link* link);
	void clearExportingSyncDataElementList();

	// @ Functions to import vehicle from another worker
	VehicleInterface* buildVehWithImportInfo(string vehId, string remainingRoute, SALTTime tStart);
	void insertImportingSyncDateElementList(std::vector<salt_comm::SyncDataElement>& importingSyncDataElementList);
#endif

	void clear();

private:
	SimulationController* myBelongingController;

#ifdef DISTRIBUTED
	// TODO - optimization issue : vector or list?
	vector<salt_comm::SyncDataElement> exportingSyncDataElementList;
#endif
	// attrs related with loading
	string myRouteFileType;
	vector<string> myRouteFileList;
	int myRouteFileListIndex = 0;

	ParserInterface* myParser = nullptr;

	// TODO - optimization issue: change to std::set !!!!!
	list<VehicleInterface*> myStandByVehicleList;
	
	// vehs status tracking
	set<VehicleInterface*> myRunningSet;
	int myEnteredVehicleCnt = 0;
	int myArrivedVehicleCnt = 0;

	
	

};


ostream& operator << (ostream& strm, VehicleManager& obj);
}
#endif /* OBJECTT_VEHICLEMANAGER_H_ */
