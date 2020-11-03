//
// Created by Hyewon Song on 2020/09/14.
//

#ifndef SALT_VEHICLEV3_H
#define SALT_VEHICLEV3_H


#include <Object/Agent/VehicleInterface.h>
#include <utils/config.h>
#include <string>

namespace SALT {
    class Route;

    class VehicleV3 : public VehicleInterface {
    public:
        VehicleV3();

        VehicleV3(Route* _route, string _departTime, int _routeIndex,  ID _ID="", VehicleClass _vehClass=PASSENGER);

        virtual ~VehicleV3();

        virtual void updateStatus(SALTTime nextTime);

    private:

    };
}

#endif //SALT_VEHICLEV3_H
