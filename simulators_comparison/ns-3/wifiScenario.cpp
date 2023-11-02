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
#include "ns3/tuple.h"


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
  std::string devToDevBW = "500Mbps";
  std::string sensorToDevLat = "0ms";
  std::string devToDevLat = "0ms";
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
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue (40));
  // Disable Short Guard Interval: Add ghost signal to the carrier in order to reduce the collision probability with the data (multipath fading etc..)
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/HtConfiguration/ShortGuardIntervalSupported", BooleanValue (false));
  //Config::SetDefault("ns3::TcpSocket::SndBufSize", UintegerValue(6291456));
  //Config::SetDefault("ns3::TcpSocket::RcvBufSize", UintegerValue(6291456));

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute("DataRate", StringValue(devToDevBW));
  pointToPoint.SetChannelAttribute("Delay", StringValue(devToDevLat));

  // create ptp between sensors and device 0
  NodeContainer deviceNodes;
  deviceNodes.Create(2);
  InternetStackHelper stack;
  stack.Install(deviceNodes);

  NetDeviceContainer netDevNodes;
  Ipv4AddressHelper address;
  Ipv4InterfaceContainer interfacesNodes3;

  // create wired zone
  netDevNodes.Add(pointToPoint.Install(deviceNodes.Get(0),deviceNodes.Get(1)));
  address.SetBase("10.1.1.0", "255.255.255.0");
  interfacesNodes3.Add(address.Assign(NetDeviceContainer(netDevNodes.Get(0), netDevNodes.Get(1))));


  // create WIFI zone
  NodeContainer AP;
  AP.Add(deviceNodes.Get(0));
  NodeContainer STA;
  STA.Create(10);
  TupleValue<UintegerValue, UintegerValue, EnumValue, UintegerValue> value;
  value.Set(WifiPhy::ChannelTuple {0, 40, WIFI_PHY_BAND_5GHZ, 0});
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper wifiPhy;
  wifiPhy.SetChannel (wifiChannel.Create ());

  WifiHelper wifiHelper;
  wifiHelper.SetStandard (WIFI_STANDARD_80211n);


  wifiHelper.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                      "DataMode", StringValue ("HtMcs3"), // Before was HtMcs7
                                      "ControlMode", StringValue ("HtMcs3"));
  WifiMacHelper wifiMac;
  Ssid ssid = Ssid ("net1");
  /* Configure STA */
  wifiMac.SetType ("ns3::StaWifiMac", "Ssid", SsidValue (ssid));
  NetDeviceContainer sensorsNetDevices;
  sensorsNetDevices = wifiHelper.Install (wifiPhy, wifiMac, STA);
  /* Configure AP */
  wifiMac.SetType ("ns3::ApWifiMac", "Ssid", SsidValue (ssid));
  NetDeviceContainer apNetDevice;
  apNetDevice = wifiHelper.Install (wifiPhy, wifiMac, AP);

  // Setup AP position (on the origin)
  MobilityHelper mobilityAP;
  Ptr<ListPositionAllocator> posAP = CreateObject <ListPositionAllocator>();
  posAP ->Add(Vector(7.5, 7.5, 0));
  mobilityAP.SetMobilityModel ("ns3::ConstantPositionMobilityModel"); // Sensors are fixed
  mobilityAP.SetPositionAllocator(posAP);
  mobilityAP.Install(NodeContainer(AP));

  // Set station positions homogeneously in circle around the origin
  MobilityHelper mobilitySTA;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject <ListPositionAllocator>();
    double step=2*M_PI/STA.GetN();
    double theta=0;
    for(int i=0;i<STA.GetN();i++){
      double r = 15 * sqrt(((double) rand() / (RAND_MAX)));
      double theta = ((double) rand() / (RAND_MAX)) * 2 * M_PI;
      positionAlloc ->Add(Vector(cos(theta)*r,sin(theta)*r, 0)); //
    }
  mobilitySTA.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobilitySTA.SetPositionAllocator(positionAlloc);
  mobilitySTA.Install(STA);

  stack.Install (NodeContainer(AP,STA));
  Ipv4InterfaceContainer interfaces;
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.33.0.0", "255.255.0.0");
  interfaces=ipv4.Assign(NetDeviceContainer(sensorsNetDevices,apNetDevice));


  PacketSinkHelper sinks("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), 80));
  sinks.SetAttribute("StartTime",TimeValue(Seconds(1)));
  ApplicationContainer sa = sinks.Install(AP.Get(0));

  Ipv4Address dstAddr = interfaces.GetAddress(interfaces.GetN()-1);
  // create flows on port 80
  for (int i=0; i<flowNumbers; i++) {
    BulkSendHelper bulkHelper ("ns3::TcpSocketFactory", InetSocketAddress (dstAddr, 80));
    bulkHelper.SetAttribute("MaxBytes", UintegerValue(deviceToDeviceFlowSize));
    bulkHelper.SetAttribute("StartTime", TimeValue(Seconds(15)));
    bulkHelper.Install(STA.Get(i));
    NS_LOG_UNCOND("Flow: "<<dstAddr<<" "<<STA.Get(i)->GetDevice(0)->GetAddress());
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
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper.GetClassifier ());
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (stat.first);
    if(t.destinationPort >= 80 && t.destinationPort<=90)
    //if (stat.second.rxBytes>=deviceToDeviceFlowSize/2)
      NS_LOG_UNCOND("Sink received first pckt at "<<stat.second.timeFirstRxPacket.GetSeconds() <<" last packet at "<<stat.second.timeLastRxPacket.GetSeconds() << " size: "<<stat.second.rxBytes<<" "<<stat.second.timeLastTxPacket.GetSeconds());
  }
  flowMonitor->SerializeToXmlFile("NameOfFile.xml", true, true);
  Simulator::Destroy();
  return(0);
}
