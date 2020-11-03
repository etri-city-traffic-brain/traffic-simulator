/*
 * CellSimple.h
 *
 *  Created on: 2018. 3. 14.
 *      Author: SDJ
 */

#ifndef CELLSIMPLE_H_
#define CELLSIMPLE_H_


#include <iostream>

#include "../../utils/config.h"
#include "CellInterface.h"

namespace SALT{
namespace version1{

class Cell : public SALT::CellInterface {
public:
	Cell(Link* _link,  int _len, int _row, int _col);
	virtual ~Cell();

	virtual void executeVehDepart(SALTTime tDepart);
	virtual void executeVehInsert(VehicleInterface* veh, SALTTime tDepart);


	virtual Result insertRunningQ(VehicleInterface* v, SALTTime t);
	virtual Result insertPendingQ(VehicleInterface* v, SALTTime t);
	virtual Result insertReceivingQ(VehicleInterface* v, SALTTime tStartWaiting, CellInterface* fromCell);
	virtual Result insertWaitingQ(VehicleInterface* v, SALTTime t,bool insertToBack=true);

	virtual void updateStatus();
	virtual string getStatusString();

	virtual void preprocess(SALTTime t);
	virtual void process(SALTTime t);
	virtual void postprocess(SALTTime t);

	Result popVehicle(VehicleInterface* veh);


	void Depart2LowPend(VehicleInterface* veh, SALTTime tDepart);

	void Run2Wait(SALTTime t);
	void Wait2HighPend(SALTTime t);

	void HighPend2Run(SALTTime tEnterTime);
	void executeHighPend2Run(VehicleInterface* veh, SALTTime tEnterTime, CellInterface* prevCell);
	void rollBackWait2HighPend(VehicleInterface* veh, SALTTime tStartWaiting, CellInterface* prevCell);

	void LowPend2Run(SALTTime tStartRunning);



	//	1-1) depart -> pend(low)
	//	1-2) run -> wait
	//	1-3) wait-> pend(high) (same link)
	//
	//	2-1) wait -> pend(high) (next link)
	//
	//	3-1 pend(high) -> run (same link)
	//		3-1-1) update cell status
	//		3-1-2) update veh status (cell index ++)
	//	3-2 pend(high) -> run (same link)
	//			3-2-1) update cell status (original cell current volume++
	//			3-2-2) update veh status (link index ++, cell index=0)
	//	3-3 pend(low) -> run
	//	3-4 pend(high) -> wait(prev cell)
private:



};

}
}
#endif /* CELLSIMPLE_H_ */
