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

  uint32_t flowNumbers=1;
  uint32_t deviceToDeviceFlowSize=50*1e+6/8;

  std::string latsrc0 = "0ms";
  std::string latsrc1 = "0ms";
  std::string latdev = "0ms";
  std::string latdst = "0ms";

  std::string bwsrc0 = "50Mbps";
  std::string bwsrc1 = "50Mbps";
  std::string bwdev = "50Mbps";
  std::string bwdst = "50Mbps";

  double delayBetweenFlows = 0.0;
  uint32_t seed=1;

  CommandLine cmd;

  cmd.AddValue ("flowNumbers", "Number of concurrent flows", flowNumbers);
  cmd.AddValue ("flowSize", "in bytes", deviceToDeviceFlowSize);
  cmd.AddValue ("DelayBetweenFlows", "In seconds", delayBetweenFlows);
  cmd.AddValue ("Seed", "random ns-3 seed", seed);

  cmd.AddValue ("latsrc0", "latency src_0->dev_0", latsrc0);
  cmd.AddValue ("latsrc1", "latency src_1->dev_0", latsrc1);
  cmd.AddValue ("latdev", "latency dev_0->dev_1", latdev);
  cmd.AddValue ("latdst", "latency dev_1->dst", latdst);

  cmd.AddValue ("bwsrc0", "bandwidth src_0->dev_0", bwsrc0);
  cmd.AddValue ("bwsrc1", "bandwidth src_1->dev_0", bwsrc1);
  cmd.AddValue ("bwdev", "bandwidth dev_0->dev_1", bwdev);
  cmd.AddValue ("bwdst", "bandwidth dev_1->dst", bwdst);
  cmd.Parse (argc, argv);

  ns3::RngSeedManager::SetSeed (seed);
  RngSeedManager::SetRun(seed);
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1500));
  Config::SetDefault("ns3::TcpSocket::SndBufSize", UintegerValue(6291456));
  Config::SetDefault("ns3::TcpSocket::RcvBufSize", UintegerValue(6291456));

  PointToPointHelper pointToPoint;

  NodeContainer deviceNodes;
  deviceNodes.Create(5);
  InternetStackHelper stack;
  stack.Install(deviceNodes);

  NetDeviceContainer netDevNodes;
  Ipv4AddressHelper address;
  Ipv4InterfaceContainer interfacesNodes;

  // src_0 -> dev_0
  pointToPoint.SetDeviceAttribute("DataRate", StringValue(bwsrc0));
  pointToPoint.SetChannelAttribute("Delay", StringValue(latsrc0));
  netDevNodes.Add(pointToPoint.Install(deviceNodes.Get(0),deviceNodes.Get(2)));
  address.SetBase("10.1.1.0", "255.255.255.0");
  interfacesNodes.Add(address.Assign(NetDeviceContainer(netDevNodes.Get(0), netDevNodes.Get(1))));

  // src_1 -> dev_0
  pointToPoint.SetDeviceAttribute("DataRate", StringValue(bwsrc1));
  pointToPoint.SetChannelAttribute("Delay", StringValue(latsrc1));
  netDevNodes.Add(pointToPoint.Install(deviceNodes.Get(1),deviceNodes.Get(2)));
  address.SetBase("12.1.1.0", "255.255.255.0");
  interfacesNodes.Add(address.Assign(NetDeviceContainer(netDevNodes.Get(2), netDevNodes.Get(3))));

  // dev_0 -> dev_1
  pointToPoint.SetDeviceAttribute("DataRate", StringValue(bwdev));
  pointToPoint.SetChannelAttribute("Delay", StringValue(latdev));
  netDevNodes.Add(pointToPoint.Install(deviceNodes.Get(2),deviceNodes.Get(3)));
  address.SetBase("13.1.1.0", "255.255.255.0");
  interfacesNodes.Add(address.Assign(NetDeviceContainer(netDevNodes.Get(4), netDevNodes.Get(5))));

  // dev_0 -> dev_1
  pointToPoint.SetDeviceAttribute("DataRate", StringValue(bwdst));
  pointToPoint.SetChannelAttribute("Delay", StringValue(latdst));
  netDevNodes.Add(pointToPoint.Install(deviceNodes.Get(3),deviceNodes.Get(4)));
  address.SetBase("14.1.1.0", "255.255.255.0");
  interfacesNodes.Add(address.Assign(NetDeviceContainer(netDevNodes.Get(6), netDevNodes.Get(7))));


  // create flows on port 80
  PacketSinkHelper sinks("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), 80));
  sinks.SetAttribute("StartTime",TimeValue(Seconds(1)));
  ApplicationContainer sa = sinks.Install(deviceNodes.Get(4));

  Ipv4Address dstAddr = interfacesNodes.GetAddress(interfacesNodes.GetN()-1);
  //NS_LOG_UNCOND ("SEND FROM "<<sensorNodes.Get(i)->GetDevice(0)->GetAddress());

  // src_0 -> dst
  for (int i=0; i<flowNumbers; i++) {
    BulkSendHelper bulkHelper ("ns3::TcpSocketFactory", InetSocketAddress (dstAddr, 80));
    bulkHelper.SetAttribute("MaxBytes", UintegerValue(deviceToDeviceFlowSize));
    bulkHelper.SetAttribute("StartTime", TimeValue(Seconds(15+delayBetweenFlows*i)));
    bulkHelper.Install(deviceNodes.Get(0));
    NS_LOG_UNCOND("Flow: "<<dstAddr<<" "<<deviceNodes.Get(0)->GetDevice(0)->GetAddress());
  }


  // src_1 -> dst
  for (int i=0; i<flowNumbers; i++) {
    BulkSendHelper bulkHelper ("ns3::TcpSocketFactory", InetSocketAddress (dstAddr, 80));
    bulkHelper.SetAttribute("MaxBytes", UintegerValue(deviceToDeviceFlowSize));
    bulkHelper.SetAttribute("StartTime", TimeValue(Seconds(15+delayBetweenFlows*i)));
    bulkHelper.Install(deviceNodes.Get(1));
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
  //flowMonitor->SerializeToXmlFile("NameOfFile.xml", true, true);
  Simulator::Destroy();
  return(0);
}
