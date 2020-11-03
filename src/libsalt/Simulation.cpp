/****************************************************************************/
// C++ libsalt API implementation
/****************************************************************************/

#include <cstring>
#include <utils/config.h>
#include <utils/etc.h>

#include "LibsaltDefs.h"
#include "Simulation.h"
#include "VisClient.hpp"

namespace libsalt {
    // ===========================================================================
    // static member initializations
    // ===========================================================================
    Clock::time_point Simulation::timeStart;
    Clock::time_point Simulation::timeEnd;
    std::string Simulation::scenarioFile;
    std::string Simulation::partitionID;

    SALT::SimulationController* Simulation::SC = nullptr;
    VisClient* VC = nullptr;

    // ===========================================================================
    // static member definitions
    // ===========================================================================
    void
    Simulation::load(std::string argv) {

        if (argv == "") {
            std::cerr << "--- Error: Scenario File Not Specified ---" << std::endl;
            return;
        } else {
            scenarioFile = argv;
            std::cout << "scenarioFile: " << scenarioFile << std::endl;
        }

        std::cout << "[SALT Simulator 2.0] " << std::endl;
        std::cout << "--- Scenario File: " << scenarioFile << std::endl;

        // Create simulation controller
        SC = new SALT::SimulationController();

        SALT::Result configResult = SC->configureSimulation(scenarioFile);
        if (configResult == SALT::FAILURE) {
            std::cerr << "--- Error: Configuration Failed ---" << std::endl;
            return;
        }
        cout << "--- Configuration Done ---" << endl;

        SC->printSimulationSetting();

// #define RENDER
#ifdef RENDER
            render()
#endif

        // ===========================================
        // Start TCP server
        // master's host and port
        string host = SC->getScenarioInfo()->simhost;
        int port = SC->getScenarioInfo()->simport;
        if (port != 0) {
            VC = new VisClient(SC, host, port);
            VC->start();
        }
        // ===========================================

        // Run simulation
        timeStart = Clock::now();
        std::cout << "[Simulation Start]" << std::endl;
        SC->assertReady();
    }

    void
    Simulation::render() {
        // Road Network Rendering
        auto nodes = SC->getNodeList();
        for (auto node: nodes) {
            std::cout << "Node " << node->getID() << " shape: " << node->getShape() << std::endl;
        }

        auto links = SC->getLinkList();
        for (auto link: links){
            std::cout << "Link " << link->getID() << " #section: " << link->getNumSection() << " #lane: " << link->getNumLane() << " shape: " << link->getShape() << std::endl;
        }

        auto cells = SC->getCellList();
        for(auto cell: cells){
            std::cout << "Cell " << cell->getMyLink()->getID() << "-" << cell->getSection() << "-" << cell->getLane() << ": " << cell->getMyCurrentVolume() << std::endl;
        }

        // Traffic Signal Rendering
        auto trafficSignals = SC->getTrafficSignalList();
        for (auto ts : trafficSignals){
            SALT::Node* signalizedNode = ts->getMyNode();
            string currentSignalState = signalizedNode->getCurrentTrafficSignalState(); //
            // iterate over all the connections on the node
            cout << signalizedNode->getID() << endl;
            auto conns = signalizedNode->getOrderedConnectionList();
            for (unsigned order = 0; order < conns.size(); order++){
                SALT::Connection* targetConnection = conns[order];
                // get connection's info and traffic signal on the connection
                SALT::Link* fromLink = targetConnection->getFromLink(); // where to render
                int fromLane = targetConnection->getFromLane(); // where to render
                SALT::Vector2D direction = targetConnection->getDirection(); // what to render (vector img)
                std::string rotationDir = targetConnection->getRotationDir(); // "L", "S", "R"
                char& currentSignal = currentSignalState[order]; // one of 'G', 'g', 'r','y'

                // render a colored(signal) arrow(direction vector) at location(fromLink,fromLane)
             std::cout << "Render a colored(" << string(1, currentSignal) << ") turn info(" << rotationDir << ")\t at (" << fromLane << ")th lane of Link" << fromLink->getID() << std::endl;
            }
            std::cout << "Done\n";;
        }
    }

    bool
    Simulation::isLoaded() {
        if (SC->getSimulationState() != SALT::SIMULATION_READY) {
            return false;
        }
        return true;
    }

    void
    Simulation::step(const int timestep) {
        SALT::SALTTime endStep = (SALT::SALTTime)timestep;
        SALT::SALTTime currentStep;

        do {
            auto timeStepStart = Clock::now();
            currentStep = SC->getCurrentStep();
#ifdef DEBUG
            std::cout << "(Step " << currentStep << ")" << std::endl;
#endif

            // update status of vehicles, traffic signals and events
            SC->doSimulationStep(currentStep);

// #define RENDER
#ifdef RENDER
            // access only the activated cells at the current step
            const auto activeCells = SC->getActiveCellSet();
            for (auto activeCell: activeCells){
                string cellID= activeCell->getID();
                int currentVehNum = activeCell->getMyCurrentVolume();
                float density = (5.0f * currentVehNum) / activeCell->getMyLength();
                std::cout << "Cell id: " << cellID << " current #(veh): " << currentVehNum << " density: " << density << std::endl;
            }
#endif

            SC->printStep(currentStep, 20);

            auto timeStepEnd = Clock::now();
            auto elapsedTime = chrono::duration_cast<std::chrono::milliseconds>(timeStepEnd - timeStepStart).count();
            SC->writeProgress(currentStep, std::to_string(elapsedTime));

            // Send visualization data to VisServer
            if (VC != nullptr) {
                int ratio = 1; // Send every ratio'th data to VisServer
                if (currentStep % ratio == 0) {
                    VC->sendDataMessageInternal();
                }
            }

            if (endStep > currentStep) {
                SC->increaseCurrentStep();
            }
        } while (endStep > currentStep);

    }

    void
    Simulation::setCurrentStep(const int timestep) {
        SALT::SALTTime endStep = (SALT::SALTTime)timestep;
        SALT::SALTTime currentStep = SC->getCurrentStep();

        std::cout << "[Jump currentStep from " << currentStep << " to " << endStep << "]" << std::endl;

        do {
            // std::cout << "(Step " << currentStep << ")" << std::endl;
            if (endStep > currentStep) {
                SC->increaseCurrentStep();
            }
            currentStep = SC->getCurrentStep();
        } while (endStep > currentStep);

    }

    void
    Simulation::close(const std::string& reason) {
        timeEnd = Clock::now();
        std::cout << "[Simulation End]" << std::endl;
        auto elapsedTime = chrono::duration_cast<std::chrono::seconds>(timeEnd - timeStart).count();
        std::cout << "Elapsed Time: " << elapsedTime << " seconds" << std::endl;

        SC->stop();
        SC = nullptr;

        VC->stop();
        VC = nullptr;
    }

    int
    Simulation::getCurrentStep() {
        return SC->getCurrentStep();
    }

    int
    Simulation::getBeginStep() {
        return SC->getBeginStep();
    }

    int
    Simulation::getEndStep() {
        return SC->getEndStep();
    }

    SALT::NetworkManager*
    Simulation::getNetworkManager() {
        return SC->myNetworkManager;
    }

    SALT::VehicleManager*
    Simulation::getVehicleManager() {
        return SC->myVehicleManager;
    }

    SALT::TrafficSignalManager*
    Simulation::getTrafficSignalManager() {
        return SC->myTrafficSignalManager;
    }

} // end of namespace libsalt

/****************************************************************************/