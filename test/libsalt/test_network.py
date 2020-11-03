import libsalt 
import math

def test(salt_scenario):
#    for i in range(2):
        libsalt.start(salt_scenario)
        #libsalt.setCurrentStep(25200)
        step = libsalt.getCurrentStep()
        while step <= 87000:
        #while step <= 36000:
            if (step % 10 == 0):
                #print("Simulation Step: ", step)
                test_funcs()
            libsalt.simulationStep()
            step = libsalt.getCurrentStep()

        libsalt.close()
        print("Python: Simulation End!!!")

def test_funcs():
#    nodes = libsalt.node.getNodeList()
#    print("nodes: ", len(nodes))
#    for node in nodes:
#        print(node.toString())
#        signal = libsalt.node.getCurrentTrafficSignalState(node.id)
#        if (signal != ""):
#            print("node.id={}, currentTrafficSignalState={}".format(node.id, signal))
#            conns = libsalt.node.getOrderedConnectionList(node.id)
#            for conn in conns:
#                print("conn={}".format(conn.toString()))

#    links = libsalt.link.getLinkList()
#    print("#links: ", len(links))
#    for link in links:
#        if (libsalt.link.getAverageVehicleSpeed(link.id) != 0):
#            print("Link: {}, avgSpeed: {}, avgVehicleSpeed: {}".format(link.id, libsalt.link.getAverageSpeed(link.id), libsalt.link.getAverageVehicleSpeed(link.id)))
#        print(link.toString())
        # if (libsalt.link.getAverageWaitingTime(link.id) != 0):
        #     print("{}: averageWaitingTime={}, averageWaitingQLength={}, numSections={}, numLanes={}".format(link.id, libsalt.link.getAverageWaitingTime(link.id), libsalt.link.getAverageWaitingQLength(link.id),
#                                                    libsalt.link.getNumSection(link.id), libsalt.link.getNumLane(link.id)))
#        lanes = libsalt.link.getLaneList(link.id)
#        print("lanes: ", len(lanes), ", averageVehicleSpeed:", libsalt.link.getAverageVehicleSpeed(link.id))
#        for lane in lanes:
#            print("Lane: {}, Length: {}, AverageVehicleSpeed: {}".format(lane.toString(), libsalt.lane.getLength(lane.id), libsalt.lane.getAverageVehicleSpeed(lane.id)))


    cells = libsalt.cell.getCellList()
    #print("cells: ", len(cells))
#    for cell in cells:
        #if (libsalt.cell.getAverageVehicleSpeed(cell.id) != 0 and libsalt.cell.getAverageWaitingQLength(cell.id) != 0.0):
#        if (libsalt.cell.getAverageWaitingTime(cell.id) != 0):
#            print("Cell: {}, currentVolume: {}, vehPassed: {}, avgWaitingQLength: {}, avgWaitingTime: {}, avgSpeed: {}, avgVehicleSpeed: {}".format(cell.toString(), libsalt.cell.getCurrentVolume(cell.id), libsalt.cell.getNumVehPassed(cell.id), libsalt.cell.getAverageWaitingQLength(cell.id), libsalt.cell.getAverageWaitingTime(cell.id), libsalt.cell.getAverageSpeed(cell.id), libsalt.cell.getAverageVehicleSpeed(cell.id)))

#    conns = libsalt.connection.getConnectionList()
#    print("conns: ", len(conns))
#    for conn in conns:
#        print(conn.toString())

#    activeCells = libsalt.cell.getActiveCellList()
#    print("activeCells: ", len(activeCells))
#    for activeCell in activeCells:
#        print(activeCell.toString())

#    validLinks = libsalt.link.getValidLinkList()
#    print("validLinks: ", len(validLinks))
#    for validLink in validLinks:
#        print(validLink.toString())

#    validCells = libsalt.cell.getValidCellList()
#    print("validCells: ", len(validCells))
#    for validCell in validCells:
#        print(validCell.toString())


if __name__ == "__main__":
    salt_scenario = r"/home/mclee/project/traffic-simulator/data/dj_sample_data/2020-dj_sample.json"
    test(salt_scenario)
