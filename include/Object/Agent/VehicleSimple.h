/*
 * VehicleSimple.h
 *
 *  Created on: 2018. 3. 13.
 *      Author: SDJ
 */

#ifndef VEHICLESIMPLE_H_
#define VEHICLESIMPLE_H_

#include <Object/Agent/VehicleInterface.h>
#include <utils/config.h>
#include <string>

namespace SALT{
class Route;

class VehicleSimple : public VehicleInterface {
public:
	VehicleSimple();

	VehicleSimple(Route* _route, string _departTime, int _routeIndex, ID _ID="");

	virtual ~VehicleSimple();
	virtual void updateStatus(SALTTime nextTime);
private:


};
}
#endif /* VEHICLESIMPLE_H_ */
