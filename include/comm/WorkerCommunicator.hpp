#ifndef WORKERCOMMUNICATOR_HPP 
#define WORKERCOMMUNICATOR_HPP 


#include <boost/thread/mutex.hpp>
#include "SyncClient.hpp"
#include "MasterHandler.hpp"
#include "ILocalSimController.hpp"


namespace salt_comm {

enum LocalSimState {
	UNREADY,
	READY,
	RUNNING, 
	PAUSE,
	TERMINATED
};


class WorkerCommunicator {
private:
	static WorkerCommunicator* instance_;
	LocalSimState state_;

	boost::mutex lmutex_;
	SyncClient* syncClient_;
	MasterHandler* masterHandler_;
	
	ILocalSimController* simController_;
	WorkerCommunicator();
	
public:
	static WorkerCommunicator* getInstance();
	LocalSimState getSimState();
	void prepareWorkerEnv(SyncClient* syncClient, ILocalSimController* simController);	
	bool setMasterHandler(MasterHandler* masterHandler);
	bool changeSimStateToReady();
	bool changeSimStateToRunning();
	bool changeSimStateToPause();
	void sendSyncData(WorkerSyncReq workerSyncData);
	
//	bool chnageStateToTerminated();
};

} //end of namespace salt_comm

#endif
