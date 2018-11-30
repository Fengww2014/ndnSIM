 /////////////////changed from ndn-wifi
 #include "ns3/core-module.h"
 #include "ns3/network-module.h"
 #include "ns3/applications-module.h"
 #include "ns3/wifi-module.h"
 #include "ns3/mobility-module.h"
 #include "ns3/internet-module.h"
#include "ns3/ndnSIM/helper/ndn-link-control-helper.hpp" 
 #include "ns3/ndnSIM-module.h"
 
 using namespace std;
 namespace ns3 {
 
 NS_LOG_COMPONENT_DEFINE("ndn.WifiExample");
 
 int main(int argc, char* argv[])
 { 
  
   CommandLine cmd;
   cmd.Parse(argc, argv);
 
   
   WifiHelper wifi = WifiHelper::Default();
   wifi.SetStandard(WIFI_PHY_STANDARD_80211a);
   wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode",
                                StringValue("OfdmRate24Mbps"));
 
   YansWifiChannelHelper wifiChannel; // = YansWifiChannelHelper::Default ();
   wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
   wifiChannel.AddPropagationLoss("ns3::ThreeLogDistancePropagationLossModel");
   wifiChannel.AddPropagationLoss("ns3::NakagamiPropagationLossModel");
 
   // YansWifiPhy wifiPhy = YansWifiPhy::Default();
   YansWifiPhyHelper wifiPhyHelper = YansWifiPhyHelper::Default();
   wifiPhyHelper.SetChannel(wifiChannel.Create());
   wifiPhyHelper.Set("TxPowerStart", DoubleValue(0.02));
   wifiPhyHelper.Set("TxPowerEnd", DoubleValue(0.02));
 
   NqosWifiMacHelper wifiMacHelper = NqosWifiMacHelper::Default();
   wifiMacHelper.SetType("ns3::AdhocWifiMac");
   NodeContainer nodes;
   nodes.Create(4);
   MobilityHelper mobility;
   // Put everybody into a line

/*
     Ptr<ListPositionAllocator> initialAlloc = 
     CreateObject<ListPositionAllocator> ();
 


  for (uint32_t i = 0; i < nodes.GetN (); ++i) {
       initialAlloc->Add (Vector (i*400, i,i));
   mobility.SetPositionAllocator(initialAlloc);
   }
*/ 
   ////////////////
   // 1. Install Wifi
   NetDeviceContainer wifiNetDevices = wifi.Install(wifiPhyHelper, wifiMacHelper, nodes);


/////////*******************/////////////fww 

//作者：Sherry
//链接：https://www.zhihu.com/question/56566002/answer/272307252
//来源：知乎
//著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

   mobility.SetPositionAllocator ("ns3::GridPositionAllocator", "MinX", DoubleValue(0.0),                
                   "MinY", DoubleValue (0.0), "DeltaX", DoubleValue(20.0), "DeltaY", DoubleValue(0),  "GridWidth", UintegerValue(5), "LayoutType", StringValue("RowFirst"));
   mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
   mobility.Install (nodes);  
   for (uint n=2 ; n < nodes.GetN() ; n++) 
          {    Ptr<ConstantVelocityMobilityModel> mob =nodes.Get(n)->GetObject<ConstantVelocityMobilityModel>();    
               mob->SetVelocity(Vector(2, 0, 0));         
          }



/////////*******************/////////////fww 
   
// 2. Install Mobility model
  // mobility.Install(nodes);
 
   // 3. Install NDN stack
   NS_LOG_INFO("Installing NDN stack");
   ndn::StackHelper ndnHelper;
   // ndnHelper.AddNetDeviceFaceCreateCallback (WifiNetDevice::GetTypeId (), MakeCallback
   // (MyNetDeviceFaceCallback));
   ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "1000");
   ndnHelper.SetDefaultRoutes(true);
   ndnHelper.Install(nodes);
 
   // Set BestRoute strategy
   ndn::StrategyChoiceHelper::Install(nodes, "/", "/localhost/nfd/strategy/best-route");
 
   // 4. Set up applications
   NS_LOG_INFO("Installing Applications");
 
   ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
   consumerHelper.SetPrefix("/prefix");
   consumerHelper.SetAttribute("Frequency", StringValue("1"));
   
     ApplicationContainer consumer = consumerHelper.Install(nodes.Get(0));
     consumer.Start(Seconds(0));    
     consumer.Stop(Seconds(600)); 
 
   ndn::AppHelper producerHelper("ns3::ndn::Producer");
   producerHelper.SetPrefix("/prefix");
   producerHelper.SetAttribute("PayloadSize", StringValue("1200"));
   producerHelper.Install(nodes.Get(2));
   Simulator::Schedule(Seconds(10.0), ndn::LinkControlHelper::FailLink, nodes.Get(3), nodes.Get(2));
   Simulator::Schedule(Seconds(15.0), ndn::LinkControlHelper::UpLink, nodes.Get(3), nodes.Get(2));
   Simulator::Stop(Seconds(600.0));
 
   Simulator::Run();
   Simulator::Destroy();
 
   return 0;
 }
 
 } // namespace ns3
 
 int
 main(int argc, char* argv[])
 {
   return ns3::main(argc, argv);
 }
