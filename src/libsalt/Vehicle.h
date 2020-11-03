// C++ libsalt.vehicle API implementation
/****************************************************************************/
#pragma once

#include <limits>
#include <vector>
#include <algorithm>
#include <Object/Agent/VehicleInterface.h>
#include <Object/VehicleManager.h>
#include <libsalt/LibsaltDefs.h>
#include "Simulation.h"

/**
 * @class Vehicle
 * @brief C++ libsalt.vehicle API implementation
 */
namespace libsalt {
    class Vehicle {
    private:
        /// Invalidates standard constructor
        Vehicle() = delete;

        static SALT::VehicleInterface*
        getVehicle(const std::string& vehicleID) {
            set<SALT::VehicleInterface *> runnings = Simulation::getVehicleManager()->getRunningSet();
            for (SALT::VehicleInterface* r : runnings) {
                if (r && r->getMyName() == vehicleID) {
                    return r;
                }
            }

            list<SALT::VehicleInterface *> standbys = Simulation::getVehicleManager()->getStandbyList();
            for (SALT::VehicleInterface* s : standbys) {
                if (s && s->getMyName() == vehicleID) {
                    return s;
                }
            }

            return nullptr;
        }

    public:
        static int getNumVehicles();
        static std::vector<LibsaltVehicle> getStandbyVehicles();
        static std::vector<LibsaltVehicle> getRunningVehicles();

        static std::string getShape(const std::string& vehicleID);
        static int getDepartTime(const std::string& vehicleID);
        static int getCellIndex(const std::string& vehicleID);
        static std::string getRouteString(const std::string& vehicleID);
        static LibsaltRoute getRoute(const std::string& vehicleID);
        static int getLength(const std::string& vehicleID);
        static float getSpeed(const std::string& vehicleID);
        static std::string getClass(const std::string& vehicleID);

        static LibsaltLink getLink(const std::string& vehicleID, int linkIndex);
        static std::string getLinkID(const std::string& vehicleID, int linkIndex);

        static LibsaltLink getNextLink(const std::string& vehicleID, int hop);
        static std::string getNextLinkID(const std::string& vehicleID, int hop);

        static LibsaltLink getNextValidLink(const std::string& vehicleID);
        static std::string getNextValidLinkID(const std::string& vehicleID);

        static LibsaltLink getRouteDepartingLink(const std::string& vehicleID);
        static std::string getRouteDepartingLinkID(const std::string& vehicleID);

        static LibsaltLink getCurrentLink(const std::string& vehicleID);
        static std::string getCurrentLinkID(const std::string& vehicleID);

        static LibsaltCell getCurrentCell(const std::string& vehicleID);
        static std::string getCurrentCellID(const std::string& vehicleID);

        static LibsaltCell getNextValidCell(const std::string& vehicleID);
        static std::string getNextValidCellID(const std::string& vehicleID);
    };

} // end of namespace

/****************************************************************************/