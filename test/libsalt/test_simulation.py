import libsalt 

def test(salt_scenario):
    libsalt.start(salt_scenario)

    step = libsalt.getCurrentStep()
    while step <= 5000:
        if (step % 1000 == 0):
            print("Simulation Step: {}".format(step))
        libsalt.simulationStep()
        step = libsalt.getCurrentStep()

    libsalt.close()
    print("Python: Simulation End!!!")

if __name__ == "__main__":
    salt_scenario = r"/home/mclee/project/traffic-simulator/data/dj_sample_data/2020-dj_sample.json"
    test(salt_scenario)
