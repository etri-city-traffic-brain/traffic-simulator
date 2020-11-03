#ifndef MASTERCOMMUNICATOR_HPP 
#define MASTERCOMMUNICATOR_HPP 


#include <map>
#include <tuple>
#include <vector>
#include <string>
#include <boost/thread/mutex.hpp>

#include "WorkerManager.hpp"
#include "CmdMessage.hpp"
#include "SyncMessage.hpp"
#include "IGlobalSimController.hpp"


#define BITS_PER_BYTE 8

namespace salt_comm {

enum GlobalSimState {
	SIM_READY,
	SIM_RUNNING,
	SIM_PAUSE,
	SIM_ON_TERMINATION	
};


class MasterCommunicator {

private:
	std::string simId_;
	GlobalSimState state_;
	std::vector<SubScenarioMessage> simInfos_;
	//boost::mutex tsMX_;
	long workerTimeStep_;
	long masterTimeStep_;
	long endTimeStep_;
	int workerCount_;

//	char* syncBitmap_;
//	std::map<std::string, int> pIdToIndice_;
	std::map<std::string, WorkerHandler_ptr> pIdToWorkers_;

	//manage a buffer for workerSynData
	boost::mutex syncMX;
	//key: partitionId, value:WorkerSyncData
	std::map<std::string, WorkerSyncReq> collectedWorkerSyncDataMap;


	//communicate with the worker manager
	WorkerManager* WM_; 
	//communicate with the simulation controller
	IGlobalSimController* simController_;

//	void allocateSyncBitmap();
//	void initSyncBitmap();
//	void setSyncBitmap(int index);
//	bool isSetSyncOnBitmap(int index);
//	bool isSetAllSyncsOnBitmap(); 


	//send command
public:
	MasterCommunicator(std::string simId, IGlobalSimController* simController);
	~MasterCommunicator();
	int getAvailableWorkers();
	void releaseWorkers();


	void putWorkerSyncData(WorkerSyncReq wSyncData);
	void sendGlobalSyncData(std::map<std::string, salt_comm::MasterSyncRes>& mapMasterSyncDataToSend);


	bool startSimulation(std::vector<salt_comm::SubScenarioMessage>& subScenarioMessageList);
	bool stopSimulation();
	bool pauseSimulation();
	bool resumeSimulation();

	GlobalSimState getGlobalSimState();

	//bool equalWorkerTStoMasterTS();

	//std::string getSubSimIDByBoundaryMapKey(std::tuple<std::string, std::string>& boundaryMapKey_);
}; //enf of class 

} //end of namespace slat_comm


#endif
