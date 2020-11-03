#ifndef LIBSALT_SIMULATION_H
#define LIBSALT_SIMULATION_H
/****************************************************************************/
// C++ libsalt API implementation
/****************************************************************************/
#pragma once

#include <vector>
#include <chrono>
#include <Controller/SimulationController.h>
#include "LibsaltDefs.h"

typedef std::chrono::high_resolution_clock Clock;

/**
 * @class Simulation
 * @brief C++ libsalt API implementation
 */
namespace libsalt {

    class Simulation {
    public:
        /// @brief load a simulation with the given arguments
        static void load(std::string argv);

        /// @brief return whether a simulation (network) is present
        static bool isLoaded();

        /// @brief close simulation
        static void close(const std::string& reason = "Libsalt requested termination.");

        /// @brief advances by one step (or up to the given timestep)
        static void step(const int timestep = 0);

        static int getCurrentStep();
        static int getBeginStep();
        static int getEndStep();
        static void setCurrentStep(int timestep);

        static SALT::NetworkManager* getNetworkManager();

        static SALT::VehicleManager* getVehicleManager();

        static SALT::TrafficSignalManager* getTrafficSignalManager();

    private:

        static SALT::SimulationController* SC;

        static Clock::time_point timeStart;
        static Clock::time_point timeEnd;
        static std::string scenarioFile;
        static std::string partitionID;

        static void render();

        /// @brief invalidated standard constructor
        Simulation() = delete;
    };

} // end of namespace libsalt
#endif