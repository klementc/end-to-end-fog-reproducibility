"""

    Created on Wed Nov 22 15:03:21 2017

    @author: isaac

"""
import random
import networkx as nx
import argparse
from pathlib import Path
import time
import numpy as np
import sys

from yafs.core import Sim
from yafs.application import Application,Message

from yafs.population import *
from yafs.topology import Topology

from simpleSelection import MinimunPath
from simplePlacement import CloudPlacement
from yafs.stats import Stats
from yafs.distribution import deterministic_distribution
from yafs.application import fractional_selectivity

RANDOM_SEED = 1

def create_application(execCost, flowSize):
    # APLICATION
    a = Application(name="SimpleCase")

    # (S) --> (ServiceA) --> (A)
    a.set_modules([{"Sensor":{"Type":Application.TYPE_SOURCE}},
                   {"ServiceA": {"RAM": 10, "Type": Application.TYPE_MODULE}},
                   {"Actuator": {"Type": Application.TYPE_SINK}}
                   ])
    """
    Messages among MODULES (AppEdge in iFogSim)
    """
    m_a = Message("M.A", "Sensor", "ServiceA", instructions=execCost*10**6, bytes=flowSize)
    m_b = Message("M.B", "ServiceA", "Actuator", instructions=0, bytes=0)

    """
    Defining which messages will be dynamically generated # the generation is controlled by Population algorithm
    """
    a.add_source_messages(m_a)

    """
    MODULES/SERVICES: Definition of Generators and Consumers (AppEdges and TupleMappings in iFogSim)
    """
    # MODULE SERVICES
    a.add_service_module("ServiceA", m_a, m_b, fractional_selectivity, threshold=1.0)

    return a



def create_json_topology(lat):
    """
       TOPOLOGY DEFINITION

       Some attributes of fog entities (nodes) are approximate
       """

    ## MANDATORY FIELDS
    topology_json = {}
    topology_json["entity"] = []
    topology_json["link"] = []

    cloud_dev    = {"id": 0, "model": "cloud","mytag":"cloud", "IPT": 1 * 10 ** 9, "RAM": 40000,"COST": 3,"WATT":20.0}
    sensor_dev   = {"id": 1, "model": "sensor-device", "IPT":1 * 10 ** 9, "RAM": 4000,"COST": 3,"WATT":40.0}
    actuator_dev = {"id": 2, "model": "actuator-device", "IPT": 1 * 10 ** 9, "RAM": 4000,"COST": 3, "WATT": 40.0}
    actuator_dev2 = {"id": 3, "model": "midway-device", "IPT": 1 * 10 ** 9, "RAM": 4000,"COST": 3, "WATT": 40.0}

    link1 = {"s": 0, "d": 3, "BW": 50/8, "PR": lat/1000}
    link1bis = {"s": 3, "d": 1, "BW": 50/8, "PR": lat/1000}
    link2 = {"s": 0, "d": 2, "BW": 50/8, "PR": lat/1000}

    topology_json["entity"].append(cloud_dev)
    topology_json["entity"].append(sensor_dev)
    topology_json["entity"].append(actuator_dev)
    topology_json["entity"].append(actuator_dev2)

    topology_json["link"].append(link1)
    topology_json["link"].append(link2)
    topology_json["link"].append(link1bis)


    return topology_json



# @profile
def main(simulated_time, flowN, lat, flowSize, delayBetweenFlows,execCost):

    random.seed(RANDOM_SEED)
    np.random.seed(RANDOM_SEED)

    folder_results = Path("results/")
    folder_results.mkdir(parents=True, exist_ok=True)
    folder_results = str(folder_results)+"/"

    """
    TOPOLOGY from a json
    """
    t = Topology()
    t_json = create_json_topology(lat)
    t.load(t_json)
    nx.write_gexf(t.G,folder_results+"graph_main1") # you can export the Graph in multiples format to view in tools like Gephi, and so on.

    """
    APPLICATION
    """
    app = create_application(execCost, flowSize)

    """
    PLACEMENT algorithm
    """
    placement = CloudPlacement("onCloud") # it defines the deployed rules: module-device
    placement.scaleService({"ServiceA": 1})

    """
    POPULATION algorithm
    """
    #In ifogsim, during the creation of the application, the Sensors are assigned to the topology, in this case no. As mentioned, YAFS differentiates the adaptive sensors and their topological assignment.
    #In their case, the use a statical assignment.
    pop = Statical("Statical")
    #For each type of sink modules we set a deployment on some type of devices
    #A control sink consists on:
    #  args:
    #     model (str): identifies the device or devices where the sink is linked
    #     number (int): quantity of sinks linked in each device
    #     module (str): identifies the module from the app who receives the messages
    pop.set_sink_control({"model": "actuator-device","number":1,"module":app.get_sink_modules()})

    #In addition, a source includes a distribution function:
    for ic in range(flowN):
        dDistribution = deterministic_distribution(name="Deterministic",time=100+(delayBetweenFlows*ic))
        pop.set_src_control({"model": "sensor-device", "number":1,"message": app.get_message("M.A"), "distribution": dDistribution})

    """--
    SELECTOR algorithm
    """
    #Their "selector" is actually the shortest way, there is not type of orchestration algorithm.
    #This implementation is already created in selector.class,called: First_ShortestPath
    selectorPath = MinimunPath()

    """
    SIMULATION ENGINE
    """

    stop_time = simulated_time
    s = Sim(t, default_results_path=folder_results+"sim_trace")
    s.deploy_app2(app, placement, pop, selectorPath)

    """
    RUNNING - last step
    """
    s.run(stop_time, show_progress_monitor=False)  # To test deployments put test_initial_deploy a TRUE
    s.print_debug_assignaments()

    # s.draw_allocated_topology() # for debugging

if __name__ == '__main__':
    import logging.config
    import os

    flowN = int(sys.argv[1])
    lat = int(sys.argv[2])
    flowSize = int(sys.argv[3])
    delayBetweenFlows= float(sys.argv[4])
    execCost = int(sys.argv[5])
    print(execCost)

    logging.config.fileConfig(os.getcwd()+'/logging.ini')

    start_time = time.time()
    main(simulated_time=200, flowN=flowN, lat=lat, flowSize=flowSize, delayBetweenFlows=delayBetweenFlows, execCost=execCost)

    print("\n--- %s seconds ---" % (time.time() - start_time))

    ### Finally, you can analyse the results:
    # print("-"*20)
    # print("Results:")
    # print("-" * 20)
    # m = Stats(defaultPath="Results") #Same name of the results
    # time_loops = [["M.A", "M.B"]]
    # m.showResults2(1000, time_loops=time_loops)
    # print("\t- Network saturation -")
    # print("\t\tAverage waiting messages : %i" % m.average_messages_not_transmitted())
    # print("\t\tPeak of waiting messages : %i" % m.peak_messages_not_transmitted()PartitionILPPlacement)
    # print("\t\tTOTAL messages not transmitted: %i" % m.messages_not_transmitted())