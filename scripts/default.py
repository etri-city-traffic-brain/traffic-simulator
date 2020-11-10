import argparse
import libsalt
import math
import os

cwd_path = os.getcwd()
this_file_dir_path = os.path.dirname(os.path.realpath(__file__))
salt_core_root_dir = os.path.join(this_file_dir_path, "..")

default_conf_path = os.path.join(salt_core_root_dir, "conf", "salt.conf.json")
default_scenario_path = os.path.join(salt_core_root_dir, "data", "salt.scenario.json")

def parse_args():
    parser = argparse.ArgumentParser(description="Run Dynamic Simulation")
    parser.add_argument('-c', '--conf', nargs='?', default=default_conf_path)
    parser.add_argument('-s', '--scenario', nargs='?', default=default_scenario_path)

    return parser.parse_args()

def main():
    args = parse_args()
    confPath = os.path.join(cwd_path, args.conf)
    scenarioPath = os.path.join(cwd_path, args.scenario)
    print('args', args)
    print('conf path', confPath)
    print('scenario path', scenarioPath)

    #confJson = getJsonObj(confPath)
    #print('conf json', confJson)
    #scenarioJson = getJsonObj(scenarioPath)
    #print('scenario json', scenarioJson)

    # ======================================

    libsalt.start(scenarioPath)
    #libsalt.setCurrentStep(25200)
    step = libsalt.getCurrentStep()
    while step <= 87000:
        libsalt.simulationStep()
        step = libsalt.getCurrentStep()

    libsalt.close()
    print("Python: Simulation End!!!")

    # ======================================

    exit(0)

main()

