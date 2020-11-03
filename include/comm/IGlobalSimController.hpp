//**************************************
// You must define class implemented functions of  ILocalSimController(.hpp)
// 
//*************************************

#ifndef IGLOBALSIMCONTROLLER_HPP
#define IGLOBALSIMCONTROLLER_HPP

#include <map>

#include "CmdMessage.hpp"
#include "SyncMessage.hpp"

namespace salt_comm {

class IGlobalSimController 
{
public: 
	virtual ~IGlobalSimController() {};

	virtual void requestGlobalSync(std::map<std::string, salt_comm::WorkerSyncReq> collectedWorkerSyncDataMap) = 0;

};

}
#endif

