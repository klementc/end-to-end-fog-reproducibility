#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/mobility-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/mobility-model.h"
#include "ns3/packet-sink.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/udp-echo-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/energy-module.h"
#include "ns3/wifi-radio-energy-model-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/node-list.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/bulk-send-helper.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WIFISensorsSimulator");

double TimeLastRxPacket = 0;

void PacketSinkRx(/*std::string path, */Ptr<const Packet> p, const Address &address)
{
	float simTime=Simulator::Now().GetSeconds();
	if(TimeLastRxPacket<simTime)
		TimeLastRxPacket=simTime;
}



/**
 * To get more details about functions please have a look at modules/modules.hpp
 */
int main(int argc, char* argv[]){

  uint32_t flowNumbers=2;
  uint32_t deviceToDeviceFlowSize=50*1e+6/8;
  std::string sensorToDevBW = "1Mbps";
  std::string devToDevBW = "50Mbps";
  std::string sensorToDevLat = "1ms";
  std::string devToDevLat = "1ms";
  double delayBetweenFlows = 0.0;
  uint32_t seed=1;

  CommandLine cmd;

  cmd.AddValue ("flowNumbers", "Number of concurrent flows", flowNumbers);
  cmd.AddValue ("latency", "Latency between devices", devToDevLat);
  cmd.AddValue ("flowSize", "in bytes", deviceToDeviceFlowSize);
  cmd.AddValue ("DelayBetweenFlows", "In seconds", delayBetweenFlows);
  cmd.AddValue ("Seed", "random ns-3 seed", seed);

  cmd.Parse (argc, argv);

  ns3::RngSeedManager::SetSeed (seed);
  RngSeedManager::SetRun(seed);
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1500));
  Config::SetDefault("ns3::TcpSocket::SndBufSize", UintegerValue(6291456));
  Config::SetDefault("ns3::TcpSocket::RcvBufSize", UintegerValue(6291456));

  //LogComponentEnable("BulkSendApplication", LOG_LEVEL_INFO);
  //LogComponentEnable("PacketSink", LOG_LEVEL_INFO);

  NodeContainer devicesNodes;
  devicesNodes.Create(2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute("DataRate", StringValue(devToDevBW));
  pointToPoint.SetChannelAttribute("Delay", StringValue(devToDevLat));

  // create ptp between sensors and device 0
  NodeContainer sensorNodes;
  sensorNodes.Create(2);

  NetDeviceContainer netdevSensors1;
  netdevSensors1.Add(pointToPoint.Install(sensorNodes.Get(0),devicesNodes.Get(0)));
  NetDeviceContainer netdevSensors2;
  netdevSensors2.Add(pointToPoint.Install(sensorNodes.Get(1),devicesNodes.Get(0)));

  // ptp between the two devices
  NetDeviceContainer netdevNodes = pointToPoint.Install(devicesNodes);

  InternetStackHelper stack;
  stack.Install(devicesNodes);
  stack.Install(sensorNodes);

  Ipv4AddressHelper address;
  address.SetBase("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfacesDev = address.Assign(netdevSensors1);
  address.SetBase("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer interfacesDev2 = address.Assign(netdevSensors2);
  address.SetBase("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer interfacesNodes3 = address.Assign(netdevNodes);


  // create flows on port 80
  PacketSinkHelper sinks("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), 80));
  sinks.SetAttribute("StartTime",TimeValue(Seconds(1)));
  ApplicationContainer sa = sinks.Install(devicesNodes.Get(1));


  Ipv4Address dstAddr = interfacesNodes3.GetAddress(1);
  //NS_LOG_UNCOND ("SEND FROM "<<sensorNodes.Get(i)->GetDevice(0)->GetAddress());

  for (int i=0; i<flowNumbers; i++) {
    BulkSendHelper bulkHelper ("ns3::TcpSocketFactory", InetSocketAddress (dstAddr, 80));
    bulkHelper.SetAttribute("MaxBytes", UintegerValue(deviceToDeviceFlowSize));
    bulkHelper.SetAttribute("StartTime", TimeValue(Seconds(15+delayBetweenFlows*i)));
    bulkHelper.Install(devicesNodes.Get(0));
  }


  Ipv4GlobalRoutingHelper::PopulateRoutingTables();
//  pointToPoint.EnablePcapAll("test");

  Ptr<FlowMonitor> flowMonitor;
  FlowMonitorHelper flowHelper;
  flowMonitor = flowHelper.InstallAll();

  Config::ConnectWithoutContext("/NodeList/*/ApplicationList/*/$ns3::PacketSink/Rx", MakeCallback(&PacketSinkRx));

  Simulator::Stop(Seconds(150.0));
  Simulator::Run();

  // output flow stats
  for(auto stat: flowMonitor->GetFlowStats()) {
    if (stat.second.rxBytes>=deviceToDeviceFlowSize)
      NS_LOG_UNCOND("Sink received first pckt at "<<stat.second.timeFirstRxPacket.GetSeconds() <<" last packet at "<<stat.second.timeLastRxPacket.GetSeconds() << " size: "<<stat.second.rxBytes);
  }
  //flowMonitor->SerializeToXmlFile("NameOfFile.xml", true, true);
  Simulator::Destroy();
  return(0);
}
