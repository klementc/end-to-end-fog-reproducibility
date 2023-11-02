package org.fog.test.perfeval;

import org.cloudbus.cloudsim.Host;
import org.cloudbus.cloudsim.Log;
import org.cloudbus.cloudsim.Pe;
import org.cloudbus.cloudsim.Storage;
import org.cloudbus.cloudsim.core.CloudSim;
import org.cloudbus.cloudsim.power.PowerHost;
import org.cloudbus.cloudsim.provisioners.RamProvisionerSimple;
import org.cloudbus.cloudsim.sdn.overbooking.BwProvisionerOverbooking;
import org.cloudbus.cloudsim.sdn.overbooking.PeProvisionerOverbooking;
import org.fog.application.AppEdge;
import org.fog.application.Application;
import org.fog.application.selectivity.FractionalSelectivity;
import org.fog.entities.*;
import org.fog.placement.Controller;
import org.fog.placement.ModuleMapping;
import org.fog.placement.ModulePlacementMapping;
import org.fog.policy.AppModuleAllocationPolicy;
import org.fog.scheduler.StreamOperatorScheduler;
import org.fog.utils.FogLinearPowerModel;
import org.fog.utils.FogUtils;
import org.fog.utils.distribution.SingleExecutionDistribution;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.LinkedList;
import java.util.List;

public class Bench2Hops {

        static List<FogDevice> fogDevices = new ArrayList<FogDevice>();
        static List<Sensor> sensors = new ArrayList<Sensor>();
        static List<Actuator> actuators = new ArrayList<Actuator>();

        public static FogDevice createFogDevice(String name) {
            // Processing Element of the Host, i.e., CPU
            List<Pe> peList = new ArrayList<Pe>();
            peList.add(new Pe(
                    0, // unique id
                    new PeProvisionerOverbooking(1000) // ?
            ));
            peList.add(new Pe(
                    1,
                    new PeProvisionerOverbooking(1000)
            ));
            System.out.println("PE size: "+peList.size());

            // Host of the device
            PowerHost host = new PowerHost(
                    FogUtils.generateEntityId(), // unique id
                    new RamProvisionerSimple(1000), // Bytes?, USELESS
                    new BwProvisionerOverbooking(6250000), // ? >= 1k otherwise crash
                    1000000, // Storage in Bytes ? >= 20k otherwise crash
                    peList, // List of Processing Element defined above
                    new StreamOperatorScheduler(peList), // ?
                    new FogLinearPowerModel(87.53, 82.44) // Power model of the Host -> maxPower, staticPower
            );

            // Characteristics of the device
            FogDeviceCharacteristics device_1_charac= new FogDeviceCharacteristics(
                    "", // architecture, USELESS
                    "", // os, USELESS
                    "", // vmm, USELESS
                    host, // The Host defined above
                    10, // time zone USELESS
                    3, // time cost, USELESS
                    0.05, // memory cost, USELESS
                    0.001, // storage cost, USELESS
                    0.1  // bandwidth cost, USELESS
            );

            List<Host> hostList = new ArrayList<Host>();
            hostList.add(host);

            FogDevice device = null;
            try { // Forced to use a try-catch block otherwise Eclipse screams
                device = new FogDevice(
                        name, // device name
                        device_1_charac, // Characteristics defined above
                        new AppModuleAllocationPolicy(hostList), // VM allocation policy ?
                        new LinkedList<Storage>(), // additional storage list, USELESS
                        10, // scheduling interval ? 0 -> infinite loop ? < 10 nothing happen ?
                        6.25e6, // uplink bandwidth in bytes/s ?
                        6.25e6, // downlink bandwidth in bytes/s ?
                        0.001, // uplink latency in ms ?
                        0// ?? USELESS
                );
            } catch (Exception e) {}

            fogDevices.add(device);
            return device;
        }

        public static Application createApplication(String name, int brokerId, int flowSize, int execCost) {
            Application application = Application.createApplication(name, brokerId);

            application.addAppModule("client", 10); // adding module Client to the application model
            application.addAppModule("concentration_calculator", 10); // adding module Concentration Calculator to the application model

            application.addAppEdge("EEG", "client", -1, 0, "EEG", Tuple.UP, AppEdge.SENSOR);
            application.addAppEdge("client", "concentration_calculator", execCost, flowSize, "_SENSOR", Tuple.UP, AppEdge.MODULE); // adding edge from Client to Concentration Calculator module carrying tuples of type _SENSOR

            application.addTupleMapping("client", "EEG", "_SENSOR", new FractionalSelectivity(1)); // 0.9 tuples of type _SENSOR are emitted by Client module per incoming tuple of type EEG

            return application;
        }

        public static FogBroker createBroker() {
            FogBroker broker = null;
            try { // Forced to use a try-catch block otherwise Eclipse screams
                broker = new FogBroker("broker");
            } catch (Exception e) {}
            return broker;
        }

        public static Sensor createSensor(String name, FogDevice device, int brokerId, double delay) {
            Sensor sensor = new Sensor(
                    name, // name of the sensor
                    "EEG",  // tuple type ?
                    brokerId, // user id ?
                    "application", // app id ?
                    new SingleExecutionDistribution(10+delay)
                    //new  DeterministicDistribution(10) // delay between two sensor emission
            );
            sensor.setGatewayDeviceId(device.getId());
            sensor.setLatency(0.0);  // latency of connection between EEG sensors and the parent Smartphone is 6 ms
            sensors.add(sensor);
            return sensor;
        }

        public static Controller createController() {
            Controller controller = new Controller(
                    "controller", // controller name
                    fogDevices,  // list of fog devices
                    sensors, // list of sensors
                    actuators // list of actuators
            );
            return controller;
        }

        public static void main(String[] args) {
            Log.disable();

            int num_flows = Integer.parseInt(args[0]);
            double lat_link = Double.parseDouble(args[1]);
            int flowSize = Integer.parseInt(args[2]);
            double delay = Double.parseDouble(args[3]);
            int execCost = Integer.parseInt(args[4]);

            Calendar calendar = Calendar.getInstance();
            boolean trace_flag = false; // mean trace events
            CloudSim.init(1, calendar, trace_flag);

            FogBroker broker = createBroker();

            Application application = createApplication("application", broker.getId(), flowSize, execCost);
            application.setUserId(broker.getId());

            FogDevice device_1 = createFogDevice("device_1");
            FogDevice device_2 = createFogDevice("device_2");
            FogDevice device_3 = createFogDevice("device_3");
            device_1.setUplinkLatency(lat_link);
            device_1.setParentId(device_2.getId());
            device_2.setUplinkLatency(lat_link);
            device_2.setParentId(device_3.getId());
            device_3.setParentId(-1);

            for (int i=0; i<num_flows; i++) {
                createSensor("sensor_"+num_flows, device_1, broker.getId(), delay);
            }


            Controller controller = createController();

            ModuleMapping moduleMapping = ModuleMapping.createModuleMapping();
            moduleMapping.addModuleToDevice("client", "device_1");
            moduleMapping.addModuleToDevice("concentration_calculator", "device_3");

            controller.submitApplication(
                    application, // the application
                    0, // delay before launching the application
                    new ModulePlacementMapping(fogDevices, application, moduleMapping) // module mapping ?
            );

            CloudSim.startSimulation();
            System.out.println("THIS WILL NEVER HAPPEN");
        }
    }
