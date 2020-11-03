import libsalt 

def test(salt_scenario):
    libsalt.start(salt_scenario)
    libsalt.setCurrentStep(0)
    step = libsalt.getCurrentStep()
    while step <= 2000:
        if (step % 100 == 0):
            print("Simulation Step: ", step)
            test_funcs()
        libsalt.simulationStep()
        step = libsalt.getCurrentStep()

    libsalt.close()
    print("Python: Simulation End!!!")

def test_funcs():
    standbys = libsalt.vehicle.getStandbyVehicles()
    runnings = libsalt.vehicle.getRunningVehicles()
    print("#Running Vehicles: ", len(runnings))
    #for vehicle in runnings:
    #    print("\t", vehicle.toString())

    #for vehicle in standbys:
    #    print("\t", vehicle.toString())

    for vehicle in runnings:
        # print("Running Vehicle)", vehicle.id, ":", libsalt.vehicle.getRoute(vehicle.id).toString())
        print("Running Vehicle)", vehicle.id, ":", vehicle.toString())

    #print("#Standby Vehicles: ", len(standbys))

    #for vehicle in standbys:
        # print("Standby Vehicle)", vehicle.id, ":", libsalt.vehicle.getRouteString(vehicle.id))
        #print("Standby Vehicle)", vehicle.id, ":", vehicle.toString())

if __name__ == "__main__":
    salt_scenario = r"/home/mclee/project/traffic-simulator/data/dj_sample_data/2020-dj_sample.json"
    test(salt_scenario)
