import argparse
import math
import os
import json
import re
import libsalt 

cwd_path = os.getcwd()
this_file_dir_path = os.path.dirname(os.path.realpath(__file__))
salt_core_root_dir = os.path.join(this_file_dir_path, "../..")

default_conf_path = os.path.join(salt_core_root_dir, "conf", "salt.conf.json")
default_scenario_path = os.path.join(salt_core_root_dir, "data", "scenario.default.json")

def parse_args():
    parser = argparse.ArgumentParser(description="Run Dynamic Simulation")
    parser.add_argument('-c', '--conf', nargs='?', default=default_conf_path)
    parser.add_argument('-s', '--scenario', nargs='?', default=default_scenario_path)

    return parser.parse_args()

def getJsonObj(_jsonPath):
    f = open(_jsonPath, "r")
    #return json.load(f)
    data = re.sub("//.*", "", f.read(), re.MULTILINE)
    return json.loads(re.sub("//.*", "", data, re.MULTILINE))

def main():
    args = parse_args()
    confPath = os.path.join(cwd_path, args.conf)
    scenarioPath = os.path.join(cwd_path, args.scenario)
    print('args', args)
    print('conf path', confPath)
    print('scenario path', scenarioPath)

    #confJson = getJsonObj(confPath)
    #print('conf json', confJson)
    scenarioJson = getJsonObj(scenarioPath)
    #print('scenario json', scenarioJson)
    beginStep = scenarioJson['scenario']['time']['begin']
    endStep = scenarioJson['scenario']['time']['end']


    libsalt.start(scenarioPath)
    libsalt.setCurrentStep(beginStep)
    step = libsalt.getCurrentStep()
    while step <= endStep:
        libsalt.simulationStep()
        step = libsalt.getCurrentStep()

    libsalt.close()
    print("Python: Simulation End!!!")

    exit(0)

main()
