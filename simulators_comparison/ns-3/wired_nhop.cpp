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
  uint32_t nbHops = 1;

  CommandLine cmd;

  cmd.AddValue ("flowNumbers", "Number of concurrent flows", flowNumbers);
  cmd.AddValue ("latency", "Latency between devices", devToDevLat);
  cmd.AddValue ("flowSize", "in bytes", deviceToDeviceFlowSize);
  cmd.AddValue ("DelayBetweenFlows", "In seconds", delayBetweenFlows);
  cmd.AddValue ("Seed", "random ns-3 seed", seed);
  cmd.AddValue ("nbHops", "number of links to go through", nbHops);

  cmd.Parse (argc, argv);

  ns3::RngSeedManager::SetSeed (seed);
  RngSeedManager::SetRun(seed);
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1500));
  Config::SetDefault("ns3::TcpSocket::SndBufSize", UintegerValue(6291456));
  Config::SetDefault("ns3::TcpSocket::RcvBufSize", UintegerValue(6291456));

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute("DataRate", StringValue(devToDevBW));
  pointToPoint.SetChannelAttribute("Delay", StringValue(devToDevLat));

  // create ptp between sensors and device 0
  NodeContainer deviceNodes;
  deviceNodes.Create(nbHops+1);
  InternetStackHelper stack;
  stack.Install(deviceNodes);

  NetDeviceContainer netDevNodes;
  Ipv4AddressHelper address;
  Ipv4InterfaceContainer interfacesNodes3;

  if (nbHops == 1) {
    netDevNodes.Add(pointToPoint.Install(deviceNodes.Get(0),deviceNodes.Get(1)));
    address.SetBase("10.1.1.0", "255.255.255.0");
    interfacesNodes3.Add(address.Assign(NetDeviceContainer(netDevNodes.Get(0), netDevNodes.Get(1))));
  } else if (nbHops == 2) {
    netDevNodes.Add(pointToPoint.Install(deviceNodes.Get(0),deviceNodes.Get(1)));
    address.SetBase("10.1.1.0", "255.255.255.0");
    interfacesNodes3.Add(address.Assign(NetDeviceContainer(netDevNodes.Get(0), netDevNodes.Get(1))));

    netDevNodes.Add(pointToPoint.Install(deviceNodes.Get(1),deviceNodes.Get(2)));
    address.SetBase("11.1.1.0", "255.255.255.0");
    interfacesNodes3.Add(address.Assign(NetDeviceContainer(netDevNodes.Get(2), netDevNodes.Get(3))));
  }



  // create flows on port 80
  PacketSinkHelper sinks("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), 80));
  sinks.SetAttribute("StartTime",TimeValue(Seconds(1)));
  if(nbHops==1) {
    ApplicationContainer sa = sinks.Install(deviceNodes.Get(1));
  } else if (nbHops==2) {
    ApplicationContainer sa = sinks.Install(deviceNodes.Get(2));
  }


  Ipv4Address dstAddr = interfacesNodes3.GetAddress(interfacesNodes3.GetN()-1);
  //NS_LOG_UNCOND ("SEND FROM "<<sensorNodes.Get(i)->GetDevice(0)->GetAddress());

  for (int i=0; i<flowNumbers; i++) {
    BulkSendHelper bulkHelper ("ns3::TcpSocketFactory", InetSocketAddress (dstAddr, 80));
    bulkHelper.SetAttribute("MaxBytes", UintegerValue(deviceToDeviceFlowSize));
    bulkHelper.SetAttribute("StartTime", TimeValue(Seconds(15+delayBetweenFlows*i)));
    bulkHelper.Install(deviceNodes.Get(0));
    NS_LOG_UNCOND("Flow: "<<dstAddr<<" "<<deviceNodes.Get(0)->GetDevice(0)->GetAddress());
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
  flowMonitor->SerializeToXmlFile("NameOfFile.xml", true, true);
  Simulator::Destroy();
  return(0);
}
