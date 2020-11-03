/****************************************************************************/
// C++ libsalt.vehicle API implementation
/****************************************************************************/

#include <list>
#include <Object/Agent/Route.h>
#include <Object/Agent/VehicleInterface.h>
#include <Object/NetworkManager.h>
#include <utils/ParserSALT.h>
#include <libsalt/LibsaltDefs.h>
#include "Vehicle.h"
#include "Simulation.h"

namespace libsalt {

    // ===========================================================================
    // Getter Implementation
    // ===========================================================================

    int
    Vehicle::getNumVehicles() {
        int num = 0;
        set<SALT::VehicleInterface *> runnings = Simulation::getVehicleManager()->getRunningSet();
        num += runnings.size();

        return num;
    }

    std::vector<LibsaltVehicle>
    Vehicle::getStandbyVehicles() {
        list<SALT::VehicleInterface *> standBys = Simulation::getVehicleManager()->getStandbyList();
        std::vector<LibsaltVehicle> vehicles;
        for (auto v : standBys) {
            LibsaltVehicle vehicle = LibsaltVehicle(v);
            vehicles.push_back(vehicle);
        }

        return vehicles;
    }

    std::vector<LibsaltVehicle>
    Vehicle::getRunningVehicles() {
        set<SALT::VehicleInterface *> runnings = Simulation::getVehicleManager()->getRunningSet();
        std::vector<LibsaltVehicle> vehicles;
        for (auto v : runnings) {
            LibsaltVehicle vehicle = LibsaltVehicle(v);
            vehicles.push_back(vehicle);
        }

        return vehicles;
    }

    std::string
    Vehicle::getShape(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return vehicle->getMyShape();
        }
        return nullptr;
    }

    int
    Vehicle::getDepartTime(const std::string& vehicleID){
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return vehicle->getMyDepartTime();
        }
        return INVALID_INT_VALUE;
    }

    int
    Vehicle::getCellIndex(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return vehicle->getMyCellIndex();
        }
        return INVALID_INT_VALUE;
    }

    std::string
    Vehicle::getRouteString(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return vehicle->getRouteString();
        }
        return nullptr;
    }

    LibsaltRoute
    Vehicle::getRoute(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return LibsaltRoute(vehicle->getRouteString());
        }
        //return nullptr;
        return LibsaltRoute();
    }

    int
    Vehicle::getLength(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return vehicle->getMyLength();
        }
        return INVALID_INT_VALUE;
    }

    float
    Vehicle::getSpeed(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return vehicle->getMySpeed();
        }
        return 0;
    }

    std::string
    Vehicle::getClass(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        std::string classStr("passenger");
        if (vehicle) {
            SALT::VehicleClass vclass = vehicle->getMyClass();
            if (vclass == SALT::BUS) {
                return std::string("bus");
            }
        }
        return classStr;
    }

    LibsaltLink
    Vehicle::getLink(const std::string& vehicleID, int linkIndex) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return LibsaltLink(vehicle->getLink(linkIndex));
        }
        return nullptr;
    }

    std::string
    Vehicle::getLinkID(const std::string& vehicleID, int linkIndex) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return vehicle->getLink(linkIndex)->getID();
        }
        return nullptr;
    }

    LibsaltLink
    Vehicle::getNextLink(const std::string& vehicleID, int hop) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return LibsaltLink(vehicle->getNextLink(hop));
        }
        return nullptr;
    }

    std::string
    Vehicle::getNextLinkID(const std::string& vehicleID, int hop) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return vehicle->getNextLink(hop)->getID();
        }
        return nullptr;
    }

    LibsaltLink
    Vehicle::getNextValidLink(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return LibsaltLink(vehicle->getNextValidLink());
        }
        return nullptr;
    }

    std::string
    Vehicle::getNextValidLinkID(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return vehicle->getNextValidLink()->getID();
        }
        return nullptr;
    }

    LibsaltLink
    Vehicle::getRouteDepartingLink(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return LibsaltLink(vehicle->getRouteDepartingLink());
        }
        return nullptr;
    }

    std::string
    Vehicle::getRouteDepartingLinkID(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return vehicle->getRouteDepartingLink()->getID();
        }
        return nullptr;
    }

    std::string
    Vehicle::getCurrentLinkID(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return vehicle->getCurrentLink()->getID();
        }
        return nullptr;
    }

    LibsaltLink
    Vehicle::getCurrentLink(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            return LibsaltLink(vehicle->getCurrentLink());
        }
        return nullptr;
    }

    LibsaltCell
    Vehicle::getCurrentCell(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            SALT::CellInterface* c = vehicle->getCurrentCell();
            LibsaltCell cell = LibsaltCell(c);
            return cell;
        }
        return nullptr;
    }

    std::string
    Vehicle::getCurrentCellID(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            SALT::CellInterface* cell = vehicle->getCurrentCell();
            return cell->getID();
        }
        return nullptr;
    }

    LibsaltCell
    Vehicle::getNextValidCell(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            SALT::CellInterface* c = vehicle->getNextValidCellCache();
            LibsaltCell cell = LibsaltCell(c);
            return cell;
        }
        return nullptr;
    }

    std::string
    Vehicle::getNextValidCellID(const std::string& vehicleID) {
        SALT::VehicleInterface* vehicle = getVehicle(vehicleID);
        if (vehicle != nullptr) {
            SALT::CellInterface* cell = vehicle->getNextValidCellCache();
            return cell->getID();
        }
        return nullptr;
    }

    // ===========================================================================
    // Setter Implementation
    // ===========================================================================

} // END of namespace libsalt

/****************************************************************************/