/*
 * CellVersion2.h
 *
 *  Created on: 2018. 4. 24.
 *      Author: dongjin
 */
#ifndef SRC_OBJECT_NETWORK_CELLVERSION2_H_
#define SRC_OBJECT_NETWORK_CELLVERSION2_H_

#include <Object/Network/CellInterface.h>
#include <utils/config.h>
#include <string>
#include <tuple>
namespace SALT{
namespace version2{

enum class VEH_HANDLE_TYPE{
	GO_ON,
	POP,
	EXPORT,
	ERROR
};


class Cell : public SALT::CellInterface {
public:
	Cell(Link* _link,  int _len, int _row, int _col, bool _myExceptionFlag=false, Length _vehLen=5.0f);
	virtual ~Cell();

	virtual void executeVehInsert(VehicleInterface* veh, SALTTime tDepart);

	virtual void executeVehPop(SALTTime t, RunTupleType _running_tuple);
	virtual void executeVehMove(SALTTime tStartWaiting, RunTupleType _running_tuple);
#ifdef DISTRIBUTED
	void executeVehSendInterSimulation(SALTTime tSent, WaitTupleType _wait_tuple);
#endif
	virtual bool executeVehSend(SALTTime t, WaitTupleType _wait_tuple);


	virtual void rollbackVehSend(SALTTime t);
	virtual void executeVehReceive(SALTTime tEnterTime);
	virtual void executeVehDepart(SALTTime tDepart);



	bool isEndOfLocalRoute(VehicleInterface* veh);
	bool isEndOfGlobalRoute(VehicleInterface* veh);

	VEH_HANDLE_TYPE getHowToHandle(VehicleInterface* veh);
	VEH_HANDLE_TYPE getHowToHandleStandalone(VehicleInterface* veh);
	VEH_HANDLE_TYPE getHowToHandleDistributed(VehicleInterface* veh);

	virtual Result insertRunningQ(VehicleInterface* v, SALTTime t);
	virtual Result insertPendingQ(VehicleInterface* v, SALTTime t);
	virtual Result insertReceivingQ(VehicleInterface* v, SALTTime tStartWaiting, CellInterface* fromCell);
	virtual Result insertWaitingQ(VehicleInterface* v, SALTTime t,bool insertToBack=true);

	virtual void updateStatus();
	virtual string getStatusString();

	virtual void preprocess(SALTTime t);
	virtual void process(SALTTime t);
	virtual void postprocess(SALTTime t);

	void tryVehMove(SALTTime t);
	void tryVehSend(SALTTime t);
	void tryVehReceive(SALTTime tEnterTime);
	void tryVehDepart(SALTTime tStartRunning);
	SALTTime computeLeavingTime(VehicleInterface* v, SALTTime currentTime);


	void tryVehSendDistributed(SALTTime t);
	void tryVehSendSingle(SALTTime t);

	// void tryVehSendSingleNew(SALTTime t);

private:
	NetworkManager* myNM;

};

}
}
#endif /* SRC_OBJECT_NETWORK_CELLVERSION2_H_ */
