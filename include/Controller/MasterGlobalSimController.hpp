/**************************************************************
* This is a sample for traffic simulation on master
* You have to correct SampleM(.hpp, .cpp)  and SaltMaster at least
**************************************************************/
#ifndef MASTERGLOBALSIMCONTROLLER_HPP 
#define MASTERGLOBALSIMCONTROLLER_HPP 

#include <time.h>
#include <vector>
#include <boost/thread/mutex.hpp>
#include <chrono>
#include "comm/IGlobalSimController.hpp"
#include "comm/MasterCommunicator.hpp"
#include "comm/CmdMessage.hpp"
#include "comm/SyncMessage.hpp"

namespace SALT {


	class MasterGlobalSimController : public salt_comm::IGlobalSimController {
	private:
		std::chrono::time_point<std::chrono::system_clock> startWallClockTime, endWallClockTime;
		clock_t start, end;
		salt_comm::MasterCommunicator *MC;
		std::string simId;

		long currentTimeStep;
		long endTimeStep;

		//bool toBeSimTerminated;
		bool isFinishedSim;
		bool sync_is_done;
		boost::condition_variable cv;
		boost::mutex syncMX;

		std::map<std::string, salt_comm::WorkerSyncReq> syncDataMap;

		void notifyController();

		void waitController();

		void setSyncIsDoneToFalse();
		// return (bool) : whether the request is final or not
		void replyWorkerSyncRequest();

	public:
		MasterGlobalSimController(std::string simId, long _endTimeStep);

		~MasterGlobalSimController();

		//implement functions of IGlobalSimController
		//key: partitionId(fromPartition), valeu:WorkerSyncData
		// #WorkerSyncData has SyncDataElement where each has own toPartition info.

		// input : fromPartition, vector<SDE> workerSDEList
		// update: toPartition, vector<SDE> workerSDEList' (re-arranged)
		// extract toPartition for each SDE, and register Master
		void requestGlobalSync(std::map<std::string, salt_comm::WorkerSyncReq> collectedWorkerSyncDataMap);

		//used for a thread
		//this function may be main simulation
		void run();


		//functions to controll simulation
		void startSim(std::vector<salt_comm::SubScenarioMessage>& subScenarioMessageList);


		void stopSim();

		void pauseSim();

		void resumeSim();


		//is used to be terminated the thread
		//void setToBeSimTerminated();
		bool checkFinishedSim();


		void printStep(int numTotalPrint=100);

	};
}

#endif


