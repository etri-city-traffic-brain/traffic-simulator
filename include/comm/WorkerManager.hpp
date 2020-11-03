#ifndef WORKERMANAGER_HPP
#define WORKERMANAGER_HPP

#include <map>
#include <vector>

#include <boost/thread/mutex.hpp>

#include "WorkerHandler.hpp"


namespace salt_comm {
	
class WorkerManager{
private:
	static WorkerManager* instance_;
	boost::mutex wmMutex_;
	// key: workerIP:workerPort
	std::map<std::string, WorkerHandler_ptr> readyWorkers_;
	std::map<std::string, WorkerHandler_ptr> exeWorkers_;
	
	WorkerManager();

public:
	static WorkerManager* getInstance();
	~WorkerManager();

	bool addWorker(WorkerHandler_ptr worker);
	bool  terminateWorker(std::string workerKey);
	bool terminateWorkers();
	std::vector<WorkerHandler_ptr> changeWorkersFromReadyToExe(int num);
	void changeWorkersFromExeToReady(std::vector<WorkerHandler_ptr> workerList);

	void printWorkers();
	int getAvailableWorkers();
	
	std::map<std::string, WorkerHandler_ptr> getReadyWorkers();
	std::map<std::string, WorkerHandler_ptr> getExeWorkers();

//	void assignWorkersForSim(std::string simId, vector<SimScenarioInfo> simScenarioInfos);
//	void widthrawWorkersFromSim(std::string simId);

	
}; //end of class WorkManager


} //end of namepsace
#endif




