/*
 * SimulationController.h
 *
 *  Created on: 2018. 3. 29.
 *      Author: SDJ
 */

#ifndef SIMULATIONCONTROLLER_H_
#define SIMULATIONCONTROLLER_H_

#include <iostream>
#include <utils/config.h>
#include <string>

#include "Object/NetworkManager.h"
#include "Object/VehicleManager.h"
#include "Object/TrafficSignalManager.h"
#include "Object/Output/OutputWriter.h"

#ifdef DISTRIBUTED
#include <comm/SyncMessage.hpp>
#endif // end of #ifdef DISTRIBUTED


namespace SALT{

class SimScenarioInfo;
class ModelInterface;


class SimulationController {
public:
	SimulationController(){};
	virtual ~SimulationController();
	//////////////////////////////////////////////////////////////////
	// @Functions that prepare simulation
	//////////////////////////////////////////////////////////////////

	// @brief: read configuration file and load simulation env and agent
	Result configureSimulation(string scenarioFile, string partitionID="");
	Result configureSimulationWithOutDirPrefix(string scenarioFile, string outdirPrefix);


private:
	// @brief: execute to read configuration file and load simulation env and agent
	// assumption: input file attributes are determined
	Result configureSimulation();

public:
	// @brief: read configuration file and !!simulation dump!!
	// this function is for the elastic simulation
	// NOTE - not fully implemented. (low priority funtionality)
	Result importSimulation(string scenarioFile, SALTTime importStep, string importFile, string partitionID="");
	
	// @brief : call each manager setup function
	NetworkManager* loadNetwork();
	TrafficSignalManager* loadTrafficSignalManager();
	VehicleManager* loadVehicles();
	OutputWriter* prepareOutputWriter();

	void printAnalysisSummary();
	// @brief: simulation state control
	void setSimulationReady(){ mySimulationState = SIMULATION_READY; }
	void setSimulationRunning(){ mySimulationState = SIMULATION_RUNNING; }
	void setSimulationEnd(){ mySimulationState = SIMULATION_END; }



	//////////////////////////////////////////////////////////////////
	// @Functions that update the state of simulation env and agent //
	//////////////////////////////////////////////////////////////////

	// @brief: do actual update the state of agent, enviroment and etc (e.g. myCurrentStep)
	void doSimulationStep(SALTTime _currentStep);

#ifdef DISTRIBUTED
	// # [master-worker] functions
	
	// @ brief: do update the state of agent,env before/during/after sync phase
	void doPreSyncSimulationStep(SALTTime _currensetSimulationReadytStep);
	
	void doSync(SALTTime _currentStep, salt_comm::MasterSyncRes _syncRes);
	
	void doPostSyncSimulationStep(SALTTime _currentStep);
#endif
	// @brief: iterate simulationStep until _end
	void runSimulation(SALTTime _end);


	//////////////////////////////////////////////////
	// @Functions that control simulation execution //
	//////////////////////////////////////////////////

	// @brief: start simulation by calling runSimulation(endTime from scenario)
	void start();

	// @brief: pause simulation by changing simulation state to SIMULATION_PAUSED
	void pause();

	// @brief: resume (paused) simulation by calling runSimulation(endTime from scenario)
	void resume();

	// @brief: stop simulation by changing simulation state to SIMULATION_END
	void stop();


	///////////////////////////////////////////////////
	// @Functions that get the result of simulation. //
	///////////////////////////////////////////////////

	// @brief: get the current generated output level (e.g. cell/section/level)
	std::string getOutputLevelValue() {return mySimulationScenario->outputlevel; 	}

	///////////////////////////////////////////////////
	// @//
	///////////////////////////////////////////////////
	void doDump(SALTTime currentStep);
	void doImport(SALTTime importStep, string path_to_dump);

#ifdef DISTRIBUTED
	// @brief: get the (worker-sync-data)
	//	salt_comm::WorkerSyncData& getWorkerSyncData() { return exportingSyncData; }
	//	salt_comm::WorkerSyncData exportingSyncData;
	//	void fillUpSyncData(SALTTime tSent, bool isFinalSync);

	const vector<salt_comm::SyncDataElement>& getExportingSyncDataElmentList() const;

	//clearExportingSyncDataElementList() must be called in the function that handles receiving masterSyncData
	void clearExportingSyncDataElementList();
#endif

	/////////////////////////
	// @ Utility functions //
	/////////////////////////

	// for debug
	void printSimulationResult(SALTTime currentStep);
	void printStep(int curTimeStep, int numTotalPrint=100);
    int getProgress();
	void writeProgress(SALTTime currentStep, string elapsedTime="");

	// @brief: increase this simulation's current step (myCurrentStep++)
	// @NOTE THAT : increaseCurrentStep() is called in doSimulationStep() to progress next simulation step
	void increaseCurrentStep();
	// @brief: Check the condition breaks simulation loop
	bool checkEnd();
	// @brief: Check the current simulation state to run simulation
	bool isRunnable();
	void assertReady();

	std::string getSimID() const { return mySimulationScenario->simid;	}
	// @Simulation state getter functions
	SALTTime getBeginStep() const { return myBeginStep; }
	SALTTime getEndStep() const { return myEndStep; }
	SALTTime getCurrentStep() const {return myCurrentStep;}

	void setCurrentStep(SALTTime _step) { myCurrentStep = _step; }
	void setBeginStep(SALTTime _beginStep) { myBeginStep = _beginStep; }
	void setEndStep(SALTTime _endStep) { myEndStep = _endStep; }

	SimulationState getSimulationState() const {return mySimulationState;}

	// @Simulation Road Network element getter functions
	const std::vector<Node*>& getNodeList() const;
	const std::vector<Link*>& getLinkList() const;
	const std::vector<Connection*>& getConnectionList() const;
	// @brief: get all cell list.
	const std::vector<CellInterface*>& getCellList() const;
	// @brief: get only active cells that holds at least one vehicle.
	const std::set<CellInterface*>& getActiveCellSet() const;

	// @Simulation Traffic Signal element getter functions
	const std::list<TrafficSignal*>& getTrafficSignalList() const;

	// @return: map<cell-id, cell class instance>
	// @input:  list<link-id> // list of id of Link on the GUI view
	// @brief: get only the information of the cells that are currently rendered on the GUI view
	// assume returning map's size is small.
	const std::map<std::string, CellInterface*>& getCurrentRenderingCellInfo(std::list<std::string> listOfLinkID);

	// XXX - deprecated
	map<string, string> getNodeShapeList();
	map<string, string> getLinkShapeList();

	void printSimulationSetting();

	
	SimScenarioInfo* getScenarioInfo() const{
		return mySimulationScenario;
	}
	void checkAndLoadVehicle(SALTTime _currentStep);
	void loadVehiclesDynamically(SALTTime interval_start, SALTTime interval_end);

public:
	NetworkManager* myNetworkManager=NULL;
	VehicleManager* myVehicleManager=NULL;
	TrafficSignalManager* myTrafficSignalManager=NULL;
	OutputWriter* myOutputWriter = NULL;

	std::map<std::string, CellInterface*> currentRenderingCellInfo;
private:
	SimScenarioInfo* mySimulationScenario=0;
	
	SimulationState mySimulationState=SIMULATION_NOT_READY;
	SALTTime myCurrentStep=0;


	SALTTime myBeginStep=-1;
	SALTTime myEndStep=-1;
	ModelInterface* myModel = 0;

	SALTTime myVehLoadTimeUnit=3600;
	SALTTime myNextVehLoadStep=0;



};
}
#endif /* SIMULATIONCONTROLLER_H_ */
