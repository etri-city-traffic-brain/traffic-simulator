import libsalt
import logging

def __get_logger():
    __logger = logging.getLogger('test_logger')
    formatter = logging.Formatter('%(asctime)s:%(module)s:%(levelname)s:%(message)s', '%Y-%m-%d %H:%M:%S')
    stream_handeler = logging.StreamHandler()
    stream_handeler.setFormatter(formatter)
    __logger.addHandler(stream_handeler)
    __logger.setLevel(logging.INFO) # if you want to see logs, change logging.WARNING to logging.DEBUG

    return __logger


def test_computeWaitingVehInfo(salt_cfg):
    logger = __get_logger()

    logger.debug('[TEST-LIBSALT] DEBUG LOGs >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>')
    logger.debug("The SALT Scenario File: {}".format(salt_cfg))

    # 1. Start the SALT
    libsalt.start(salt_cfg)

    # TODO: 2. Check the Begin & End of the simulation
    step = 0

    # 3. Define the target Traffic Signal --> ex: Gildong-4
    # targetTLNode = "cluster_572701114_572701116_572701117_572712746_572712755"   # For Gandong-gu, Seoul
    targetTLNode = "cluster_563103641_563103889_563103894_563103895"     # For Doan, Daejeon

    # 4. Get the Traffic Signal Information (e.g. schdules)  using get function
    tl = libsalt.trafficsignal.getTLSByNodeID(targetTLNode)

    conlinkset = libsalt.trafficsignal.getTLSConnectedLinkID(targetTLNode)

    logger.debug("[Connected Link IDs] {} ".format(libsalt.trafficsignal.getTLSConnectedLinkID(targetTLNode)))
    logger.debug("[Target Traffic Signal ID] {}".format(tl.myTLSID))
    logger.debug("The Number of schedules in the target traffic signal: {}".format(tl.getScheduleMap().size()))

    tsm = tl.getScheduleMap()

    logger.debug("The Schedule IDs in the target traffic signal: {}".format(tsm.keys()))
    logger.debug("The Schedule Detailed >>>>>> ")

    if logger.isEnabledFor(logging.DEBUG):
        for k in tsm.keys():
            logger.debug("[schedule id: {}]".format(k))
            for p in tsm[k].getPhaseVector():
                logger.debug("phase = ({}), duration = {}".format(p[0], p[1]))
        logger.debug("The Phase Vector in Schedule {}  >>>>>>>>>>>>>>".format(tsm.keys()[0]))
        tsps = libsalt.trafficsignal.getTLSPhasesByNodeID(targetTLNode,tsm.keys()[0])
        for pp in tsps:
            logger.debug("({}, {})".format(pp[0], pp[1]))
        logger.debug("The Phase Index {}".format(3))
        tp = libsalt.trafficsignal.getTLSPhaseByNodeID(targetTLNode,tsm.keys()[0],3)
        logger.debug("({}, {})".format(tp[0], tp[1]))


    # 5. Test Scenario
    # >>> 1) define changing rule (e.g. changing order, testing scenarios)
    # >>> 2) test the changeTLPhase function & show results
    # >>> [test example] every 30 step, changing the phase to next (skip the yellow or all red phase for transition of TLS)
    triggerduration = 1000
    computeduration = 30
    orderdic = {}
    turn = 0
    for k in tsm.keys():
        for p in enumerate(tsm[k].getPhaseVector()):
            if p[1][0] > 5:
                pair = []
                pair.append(k)
                pair.append(p[0])
                orderdic[turn] = pair
                turn+=1
    logger.debug("The order of phases: {}".format(orderdic))

    while step <= libsalt.getEndStep():
        curstep = libsalt.getCurrentStep()
        begin = libsalt.getBeginStep()

        if (curstep % triggerduration)==0:
            ordering = (curstep / triggerduration) % len(orderdic)
            curscheduleid = orderdic[ordering][0]
            curphaseindex = orderdic[ordering][1]
            if logger.isEnabledFor(logging.DEBUG):
                logger.debug("Ordering: {}, current ts schedule id: {}, , current ts phase index: {}".format(ordering, curscheduleid, curphaseindex))
                logger.debug("[Trigger Point] time: {}, schdule id: {}  ({})".format(curstep,
                                                                                    libsalt.trafficsignal.getCurrentTLSScheduleIDByNodeID(targetTLNode),
                                                                                    libsalt.trafficsignal.getCurrentTLSPhaseStateByNodeID(targetTLNode)))
            libsalt.trafficsignal.changeTLSPhase(curstep, targetTLNode, curscheduleid, curphaseindex)
            if logger.isEnabledFor(logging.DEBUG):
                logger.debug("[After Changing] schedule id : {} ({})".format(libsalt.trafficsignal.getCurrentTLSScheduleIDByNodeID(targetTLNode),
                                                                                           libsalt.trafficsignal.getCurrentTLSPhaseStateByNodeID(targetTLNode)))

        libsalt.simulationStep()

        if ((curstep % computeduration)== computeduration-1) and (curstep > begin + computeduration):
            lastswitching = libsalt.trafficsignal.getLastTLSPhaseSwitchingTimeByNodeID(targetTLNode)
            if logger.isEnabledFor(logging.DEBUG):
                logger.debug("Last Switching Time: {}, current Step: {}".format(lastswitching, curstep))
                logger.debug("[Traget Signal {} ] ".format(targetTLNode))
                for li in conlinkset:
                    logger.debug("[link: {}] NumWaitVeh: {}, Average veh waiting time: {}, Average veh waiting Q length: {} "
                          .format(li, libsalt.link.getNumWaitingVehicle(li, curstep, lastswitching),
                                  libsalt.link.getAverageVehicleWaitingTime(li, curstep, lastswitching),
                                  libsalt.link.getAverageVehicleWaitingQLength(li, curstep, lastswitching)))

        step += 1

    libsalt.close()
    logger.debug("simulation end!!!")


def test_changeTLPhase(salt_cfg):
    logger = __get_logger()

    logger.debug('[TEST-LIBSALT] DEBUG LOGs >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>')
    logger.debug("The SALT Scenario File: {}".format(salt_cfg))

    # 1. Start the SALT
    libsalt.start(salt_cfg)

    # TODO: 2. Check the Begin & End of the simulation
    step = 0

    # 3. Define the target Traffic Signal
    targetTLNode = "cluster_572701114_572701116_572701117_572712746_572712755"    # targetTLNode = "cluster_572701114_572701116_572701117_572712746_572712755"   # For Gandong-gu, Seoul
    targetTLNode = "cluster_563103641_563103889_563103894_563103895"     # For Doan, Daejeon

    # 4. Get the Traffic Signal Information (e.g. schdules)  using get function
    tl = libsalt.trafficsignal.getTLSByNodeID(targetTLNode)

    logger.debug("[Connected Link IDs] {} ".format(libsalt.trafficsignal.getTLSConnectedLinkID(targetTLNode)))

    logger.debug("[Target Traffic Signal ID] {}".format(tl.myTLSID))
    logger.debug("The Number of schedules in the target traffic signal: {}".format(tl.getScheduleMap().size()))
    tsm = tl.getScheduleMap()

    if logger.isEnabledFor(logging.DEBUG):
        logger.debug("The Schedule IDs in the target traffic signal: {}".format(tsm.keys()))
        logger.debug("The Schedule Detailed >>>>>> ")
        for k in tsm.keys():
            logger.debug("[schedule id: {}]".format(k))
            for p in tsm[k].getPhaseVector():
                logger.debug("phase = ({}), duration = {}".format(p[0], p[1]))
        logger.debug("The Phase Vector in Schedule {}  >>>>>>>>>>>>>>".format(tsm.keys()[0]))
        tsps = libsalt.trafficsignal.getTLSPhasesByNodeID(targetTLNode,tsm.keys()[0])
        for pp in tsps:
            logger.debug("({}, {})".format(pp[0], pp[1]))

        logger.debug("The Phase Index {}".format(3))
        tp = libsalt.trafficsignal.getTLSPhaseByNodeID(targetTLNode,tsm.keys()[0],3)
        logger.debug("({}, {})".format(tp[0], tp[1]))

    # 5. Test Scenario
    # >>> 1) define changing rule (e.g. changing order, testing scenarios)
    # >>> 2) test the changeTLPhase function & show results
    # >>> [test example] every 30 step, changing the phase to next (skip the yellow or all red phase for transition of TLS)
    triggerduration = 30
    orderdic = {}
    turn = 0
    for k in tsm.keys():
        for p in enumerate(tsm[k].getPhaseVector()):
            if p[1][0] > 5:
                pair = []
                pair.append(k)
                pair.append(p[0])
                orderdic[turn] = pair
                turn+=1
    logger.debug("The order of phases: {}".format(orderdic))

    while step <= 32400:
        curstep = libsalt.getCurrentStep()

        if (curstep % triggerduration)==0:
            ordering = (curstep / triggerduration) % len(orderdic)
            curscheduleid = orderdic[ordering][0]
            curphaseindex = orderdic[ordering][1]
            if logger.isEnabledFor(logging.DEBUG):
                logger.debug("Ordering: {}, current ts schedule id: {}, , current ts phase index: {}".format(ordering, curscheduleid, curphaseindex))
                logger.debug("[Trigger Point] time: {}, schdule id: {}  ({})".format(curstep,
                                                                                    libsalt.trafficsignal.getCurrentTLSScheduleIDByNodeID(targetTLNode),
                                                                                    libsalt.trafficsignal.getCurrentTLSPhaseStateByNodeID(targetTLNode)))
            libsalt.trafficsignal.changeTLSPhase(curstep, targetTLNode, curscheduleid, curphaseindex)
            if logger.isEnabledFor(logging.DEBUG):
                logger.debug("[After Changing] schedule id : {} ({})".format(libsalt.trafficsignal.getCurrentTLSScheduleIDByNodeID(targetTLNode),
                                                                                           libsalt.trafficsignal.getCurrentTLSPhaseStateByNodeID(targetTLNode)))

        libsalt.simulationStep()
        step += 1

    libsalt.close()
    logger.debug("simulation end!!!")



def test_setTLS(salt_cfg):
    logger = __get_logger()

    logger.debug('[TEST-LIBSALT] DEBUG LOGs >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>')
    logger.debug("The SALT Scenario File: {}".format(salt_cfg))

    # 1. Start the SALT
    libsalt.start(salt_cfg)

    # TODO: 2. Check the Begin & End of the simulation
    step = 0

    # 3. Define the target Traffic Signal
    # targetTLNode = "cluster_572701114_572701116_572701117_572712746_572712755"   # For Gandong-gu, Seoul
    targetTLNode = "cluster_563103641_563103889_563103894_563103895"     # For Doan, Daejeon

    # 4. Get the Traffic Signal Information (e.g. schdules)  using get function
    tl = libsalt.trafficsignal.getTLSByNodeID(targetTLNode)

    logger.debug("[Connected Link IDs] {} ".format(libsalt.trafficsignal.getTLSConnectedLinkID(targetTLNode)))

    logger.debug("[Target Traffic Signal ID] {}".format(tl.myTLSID))
    logger.debug("The Number of schedules in the target traffic signal: {}".format(tl.getScheduleMap().size()))
    tsm = tl.getScheduleMap()

    if logger.isEnabledFor(logging.DEBUG):
        logger.debug("The Schedule IDs in the target traffic signal: {}".format(tsm.keys()))
        logger.debug("The Schedule Detailed >>>>>> ")
        for k in tsm.keys():
            logger.debug("[schedule id: {}]".format(k))
            for p in tsm[k].getPhaseVector():
                logger.debug("phase = ({}), duration = {}".format(p[0], p[1]))

        logger.debug("The Phase Vector in Schedule {}  >>>>>>>>>>>>>>".format(tsm.keys()[0]))
        tsps = libsalt.trafficsignal.getTLSPhasesByNodeID(targetTLNode,tsm.keys()[0])
        for pp in tsps:
            logger.debug("({}, {})".format(pp[0], pp[1]))

        logger.debug("The Phase Index {}".format(3))
        tp = libsalt.trafficsignal.getTLSPhaseByNodeID(targetTLNode,tsm.keys()[0],3)
        logger.debug("({}, {})".format(tp[0], tp[1]))

    # 5. Testing setTLS
    # >>> 1) define the test scenario
    # >>> 2) test the setTLS & show results
    # >>> [test example] add 1 time step to a duration time of the intermediate phase (e.g. yellow phase) in the 1st schedule of the target TLS
    newsch = libsalt.TLSSchedulesMap()
    for k in enumerate(tsm.keys()):
        if k[0] == 0:
            newpv = libsalt.TLSPhaseVector()
            for p in tsm[k[1]].getPhaseVector():
                if p[0] < 5:
                    newp = libsalt.TLSPhasePair((p[0]+1, p[1]))
                    newpv.append(newp)
                else:
                    orip = libsalt.TLSPhasePair(p)
                    newpv.append(orip)
            if logger.isEnabledFor(logging.DEBUG):
                logger.debug("The number of Phases: {} ".format(newpv.size()))
                logger.debug("(phases) >>>>>>>>>>>> ")
                for pv in newpv:
                    logger.debug(pv)
            newschedule = libsalt.TLSSchedule(tsm[k[1]].getSchedulID(), tsm[k[1]].getOffset(), newpv)
            if logger.isEnabledFor(logging.DEBUG):
                logger.debug("[DEBUG] new libsalt.TLSSchedule ")
                logger.debug("(offset): ".format(newschedule.getOffset()))
                logger.debug("(phases) >>>>>>>>>>>>>")
                for pv in newschedule.getPhaseVector():
                    logger.debug(pv)
            newsch[k[1]] = newschedule
        else:
            newpv2 = libsalt.TLSPhaseVector()
            for p2 in tsm[k[1]].getPhaseVector():
                orip2 = libsalt.TLSPhasePair(p2)
                newpv2.append(orip2)
            newschedule2 = libsalt.TLSSchedule(tsm[k[1]].getSchedulID(), tsm[k[1]].getOffset(), newpv2)
            newsch[k[1]] = newschedule2
            if logger.isEnabledFor(logging.DEBUG):
                logger.debug("[DEBUG] new libsalt.TLSSchedule ")
                logger.debug("(offset): ".format(newschedule2.getOffset()))
                logger.debug("(phases) >>>>>>>>>>>>>")
                for pv in newschedule2.getPhaseVector():
                    logger.debug(pv)

        if logger.isEnabledFor(logging.DEBUG):
            logger.debug("[DEBUG] New TLSSchedule for 'set' information  >>>>>>>>>>>>>>>>>>")
            for k in newsch.keys():
                logger.debug("[schedule id: {}]".format(k))
                for p in newsch[k].getPhaseVector():
                    logger.debug("phase = ({}), duration = {}".format(p[0], p[1]))

    tod = tl.getTODPlan()
    newtod = libsalt.TLSTODMap()
    for k in tod.keys():
        newtod[k] = tod[k]

    newtl = libsalt.TLSLogic(targetTLNode, newsch, newtod)
    logger.debug("Schedule IDs (for setTLS test): {}".format(newtl.getScheduleMap().keys()))
    res = libsalt.trafficsignal.setTLSByNodeID(targetTLNode, newtl)
    logger.debug(res)


def test_changeTLPhaseVector(salt_cfg):
    logger = __get_logger()

    logger.debug('[TEST-LIBSALT] DEBUG LOGs >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>')
    logger.debug("The SALT Scenario File: {}".format(salt_cfg))

    # 1. Start the SALT
    libsalt.start(salt_cfg)

    # TODO: 2. Check the Begin & End of the simulation
    step = 0

    # 3. Define the target Traffic Signal
    # targetTLNode = "cluster_572701114_572701116_572701117_572712746_572712755"   # For Gandong-gu, Seoul
    targetTLNode = "cluster_563103641_563103889_563103894_563103895"     # For Doan, Daejeon

    # 4. Get the Traffic Signal Information (e.g. schdules)  using get function
    tl = libsalt.trafficsignal.getTLSByNodeID(targetTLNode)

    logger.debug("[Connected Link IDs] {} ".format(libsalt.trafficsignal.getTLSConnectedLinkID(targetTLNode)))

    logger.debug("[Target Traffic Signal ID] {}".format(tl.myTLSID))
    logger.debug("The Number of schedules in the target traffic signal: {}".format(tl.getScheduleMap().size()))
    tsm = tl.getScheduleMap()
    currentts = libsalt.trafficsignal.getCurrentTLSScheduleByNodeID(targetTLNode)
    currenttsid = libsalt.trafficsignal.getCurrentTLSScheduleIDByNodeID(targetTLNode)

    if logger.isEnabledFor(logging.DEBUG):
        logger.debug("The Schedule IDs in the target traffic signal: {}".format(tsm.keys()))
        logger.debug("The Schedule Detailed >>>>>> ")
        for k in tsm.keys():
            logger.debug("[schedule id: {}]".format(k))
            for p in tsm[k].getPhaseVector():
                logger.debug("phase = ({}), duration = {}".format(p[0], p[1]))
        logger.debug("The Phase Vector in Schedule {}  >>>>>>>>>>>>>>".format(tsm.keys()[0]))
        tsps = libsalt.trafficsignal.getTLSPhasesByNodeID(targetTLNode,tsm.keys()[0])
        for pp in tsps:
            logger.debug("({}, {})".format(pp[0], pp[1]))

        # logger.debug("The Phase Index {}".format(3))
        # tp = libsalt.trafficsignal.getTLSPhaseByNodeID(targetTLNode,tsm.keys()[0],3)
        # logger.debug("({}, {})".format(tp[0], tp[1]))

    # 5. Test Scenario
    # >>> 1) define changing rule (e.g. changing order, testing scenarios)
    # >>> 2) test the changeTLPhase function & show results
    # >>> [test example] every 30 step, changing the phase vector to new one
    triggerduration = 1000
    orderdic = {}
    turn = 0
    # for k in tsm.keys():
    #     for p in enumerate(tsm[k].getPhaseVector()):
    #         if p[1][0] > 5:
    #             pair = []
    #             pair.append(k)
    #             pair.append(p[0])
    #             orderdic[turn] = pair
    #             turn+=1
    # logger.debug("The order of phases: {}".format(orderdic))

    # print(currentts.getPhaseVector())

    for iter in range(5):
        newpv = libsalt.TLSPhaseVector()
        for p in currentts.getPhaseVector():
            # print(p)
            if p[0] > 5:
                newp = libsalt.TLSPhasePair((p[0]+iter+1, p[1]))
                newpv.append(newp)
            else:
                orip = libsalt.TLSPhasePair(p)
                newpv.append(orip)
        orderdic[iter] = newpv

    logger.debug('[Candidate PhaseVector]: \n')
    for ti in orderdic.keys():
        logger.debug("[oderdic key: {}]".format(ti))
        for tp in orderdic[ti]:
            logger.debug("phase = ({}), duration = {}".format(tp[0], tp[1]))


    while step <= 7200:
        curstep = libsalt.getCurrentStep()

        if (curstep % triggerduration)==0:
            ordering = (curstep / triggerduration) % len(orderdic)
            # curscheduleid = orderdic[ordering][0]
            # curphaseindex = orderdic[ordering][1]
            curscheduleid = libsalt.trafficsignal.getCurrentTLSScheduleIDByNodeID(targetTLNode)
            curphaseindex = libsalt.trafficsignal.getCurrentTLSPhaseStateByNodeID(targetTLNode)
            if logger.isEnabledFor(logging.DEBUG):
                logger.debug("[Trigger Point] time: {}, schdule id: {}  ({})".format(curstep, curscheduleid, curphaseindex))
            libsalt.trafficsignal.setTLSPhaseVector(curstep, targetTLNode, curscheduleid, orderdic[ordering])
            # libsalt.trafficsignal.setTLSPhaseVector(curstep, targetTLNode, curscheduleid, curphaseindex, orderdic[ordering])
            if logger.isEnabledFor(logging.DEBUG):
                logger.debug("[After Changing] schedule id : {} ({})".format(libsalt.trafficsignal.getCurrentTLSScheduleIDByNodeID(targetTLNode),
                                                                             libsalt.trafficsignal.getCurrentTLSPhaseStateByNodeID(targetTLNode)))

        libsalt.simulationStep()
        step += 1

    libsalt.close()
    logger.debug("simulation end!!!")


if __name__ == "__main__":
    salt_cfg = "/Users/hwonsong/CLionProjects/uniq/data/2021-dj_doan_kaist_3rd.json"    # Modify the configuration file & its path
    # test_changeTLPhase(salt_cfg)  # For test the function, changeTLPhase
    # test_computeWaitingVehInfo(salt_cfg)    # For test the function, computeWaitingVehInfo
    # test_setTLS(salt_cfg)    # For test the function, setTLS
    test_changeTLPhaseVector(salt_cfg)
