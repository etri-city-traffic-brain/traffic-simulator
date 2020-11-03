//**************************************
// You must define class implemented functions of  ILocalSimController(.hpp)
// 
//*************************************

#ifndef ILOCALSIMCONTROLLER_HPP
#define ILOCALSIMCONTROLLER_HPP


#include "CmdMessage.hpp"
#include "SyncMessage.hpp"

namespace salt_comm {

class ILocalSimController 
{
public: 
	virtual ~ILocalSimController() {};

	virtual void startSimulation(SubScenarioMessage simScenarioInfo) = 0;
	virtual void stopSimulation(long timeStep)  = 0;
	virtual void pauseSimulation(long timeStep) = 0;
	virtual void resumeSimulation() = 0;
	virtual void receiveSyncData(MasterSyncRes masterSyncData) = 0;
	virtual void terminateWorker() = 0;

};

}
#endif

