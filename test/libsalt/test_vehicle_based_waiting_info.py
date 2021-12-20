import argparse
import math
import os
import json
import re
import libsalt
import logging

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

def __get_logger():
    __logger = logging.getLogger('test_logger')
    formatter = logging.Formatter('%(asctime)s:%(module)s:%(levelname)s:%(message)s', '%Y-%m-%d %H:%M:%S')
    stream_handeler = logging.StreamHandler()
    stream_handeler.setFormatter(formatter)
    __logger.addHandler(stream_handeler)
    __logger.setLevel(logging.DEBUG) # if you want to see logs, change logging.WARNING to logging.DEBUG

    return __logger

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

    scenarios = []

    libsalt.start(scenarioPath)
    libsalt.setCurrentStep(beginStep)
    step = libsalt.getCurrentStep()

    logger = __get_logger()

    # 3. Define the target Traffic Signal
    targetTLNode = "cluster_563101055_563101057_563101523_563101524_563101565_563101566"


    # 4. Get the Traffic Signal Information (e.g. schdules)  using get function
    tl = libsalt.trafficsignal.getTLSByNodeID(targetTLNode)
    conlinkset = libsalt.trafficsignal.getTLSConnectedLinkID(targetTLNode)

    triggerduration = 120

    while step <= endStep:
        libsalt.simulationStep()
        step = libsalt.getCurrentStep()

        curstep = libsalt.getCurrentStep()
        begin = libsalt.getBeginStep()

        if (curstep % triggerduration)==0:
            if logger.isEnabledFor(logging.DEBUG):
                print('The State of Waiting Vehicles Over TLS Duration at {}:'.format(targetTLNode))

                for conl in conlinkset:
                    lanelist = libsalt.link.getLaneList(conl)
                    for lnc in lanelist:
                        lane = lnc.toString()
                        print('[Connected Link: {} Lane: {}]: state = {}, avg_wt = {}, sum_wt = {}'.format(
                            conl, lane, libsalt.lane.getStateOfWaitingVehicleOverTLSDuration(lane, curstep),
                            libsalt.lane.getCurrentAverageWaitingTimeBaseVehicle(lane, curstep),
                            libsalt.lane.getCurrentWaitingTimeSumBaseVehicle(lane, curstep)))

                    print('[Connected Link {}] state = {}, avg_wt = {}, sum_wt = {}'.format(
                        conl, libsalt.link.getStateOfWaitingVehicleOverTLSDuration(conl, curstep),
                        libsalt.link.getCurrentAverageWaitingTimeBaseVehicle(conl, curstep),
                        libsalt.link.getCurrentWaitingTimeSumBaseVehicle(conl, curstep)))

    libsalt.close()
    print("Python: Simulation First End!!!")

    exit(0)

main()
