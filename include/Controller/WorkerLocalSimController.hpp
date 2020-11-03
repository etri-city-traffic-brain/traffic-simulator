#ifdef DISTRIBUTED
//**************************************
// You must define class implemented functions of  ILocalSimController(.hpp)
// 
//*************************************

#ifndef WORKERLOCALSIMCONTROLLER_HPP
#define WORKERLOCALSIMCONTROLLER_HPP


#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <utils/config.h>

#include "comm/ILocalSimController.hpp"
#include "comm/SyncMessage.hpp"
#include "comm/WorkerCommunicator.hpp"

namespace SALT {
class SimulationController;

	class WorkerLocalSimController : public salt_comm::ILocalSimController {

		// <variable to use API>
		SALT::SimulationController* simCore=NULL;
		salt_comm::WorkerCommunicator *wCommunicator=NULL;

		//<variables related to traffic simulation>
		// IP and port are determined during constructor
		std::string myIP;
		int myPort;

		// fill-up according to subScenarioMSG
		std::string mySimID;
		std::string myPartitionID;

		// <variable to manage time step>
		boost::mutex tsMX;
		long endTimeStep=-1; // controller's end-time might not be same with core's end-time
		long pauseTimeStep=-1;

		void initTimeStepManagement();
		bool checkPause(SALTTime currentTimeStep);
		bool checkEnd(SALTTime currentTimeStep);

		// etc
		int outputPeriod=0;

		// <variable to synchronize simulation thread>
		bool sync_is_done;
		boost::condition_variable cvSync;
		boost::mutex mxSync;

		void waitSyncIsDone();
		void notifySyncIsDone();
		void setSyncIsDoneToFalse();

		//
		bool toBeWorkerTerminated;
		boost::mutex terminateMX;

		void setToBeWorkerTerminated();
		//
		bool canProgress;
		boost::condition_variable cvProgress;
		boost::mutex mxProgress;

		void waitCanProgress();
		void notifyCanProgress();
		void setCanProgressToFalse();

		// <variable to hold message>
		salt_comm::SubScenarioMessage myScenarioInfo;
		salt_comm::SyncMessage syncMsg;
		salt_comm::WorkerSyncReq wSyncData;
		salt_comm::MasterSyncRes mSyncData;

		// fill worker sync-request header
		void initWorkerSyncRequestHeader();
		// fill worker sync-request body
		void fillWorkerSyncRequestBody(bool isFinal, SALTTime ReqTimeStep);

		void clearWorkerSyncRequestBody();

		void clearMasterSyncResponse();

		// <Simulation loop proceed>
		void increaseSimulationStep();

	public:
		WorkerLocalSimController(std::string ip, int port);

		~WorkerLocalSimController();

		//implement functions of ILocalSimController
		void startSimulation(salt_comm::SubScenarioMessage simScenarioInfo);

		void stopSimulation(long timeStep);

		void pauseSimulation(long timeStep);

		void resumeSimulation();

		void receiveSyncData(salt_comm::MasterSyncRes masterSyncData);

		void terminateWorker();


		//is operated  by a thread
		//this function may be main simulation
		void run();

		bool getToBeWorkerTerminated();
		void printStep(SALTTime currentTimeStep, int numTotalPrint=100);
	};
}
#endif


#endif // end of #ifdef DISTRIBUTED

