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
   LogComponentEnable("ndn.WifiExample", LOG_LEVEL_ALL);
   NS_LOG_UNCOND ("Test Simulator");

   
   WifiHelper wifi = WifiHelper::Default();
   //wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode",
   //                             StringValue("OfdmRate24Mbps"));
 
   wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                  "DataMode", StringValue ("DsssRate1Mbps"),
                                  "ControlMode", StringValue ("DsssRate1Mbps"));
   YansWifiChannelHelper wifiChannel; // = YansWifiChannelHelper::Default ();
   wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
   wifiChannel.AddPropagationLoss ("ns3::RangePropagationLossModel", 
                                    "MaxRange", DoubleValue(110));
   //wifiChannel.AddPropagationLoss("ns3::ThreeLogDistancePropagationLossModel");
   wifiChannel.AddPropagationLoss("ns3::NakagamiPropagationLossModel");
 
   // YansWifiPhy wifiPhy = YansWifiPhy::Default();
   YansWifiPhyHelper wifiPhyHelper = YansWifiPhyHelper::Default();
   wifiPhyHelper.SetChannel(wifiChannel.Create());
   wifiPhyHelper.Set("TxPowerStart", DoubleValue(0.01));
   wifiPhyHelper.Set("TxPowerEnd", DoubleValue(0.01));
 
   NqosWifiMacHelper wifiMacHelper = NqosWifiMacHelper::Default();
   wifiMacHelper.SetType("ns3::AdhocWifiMac");
   NodeContainer nodes;
   nodes.Create(5);
   MobilityHelper mobility;
   

   // Put everybody into a line
/*

     Ptr<ListPositionAllocator> initialAlloc = 
     CreateObject<ListPositionAllocator> ();
 


  for (uint32_t i = 0; i < Sta_Nodes.GetN (); ++i) {
       initialAlloc->Add (Vector (i*400, i,i));
       Sta_mobility.SetPositionAllocator(initialAlloc);
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

   mobility.SetPositionAllocator ("ns3::GridPositionAllocator", 
					"MinX", DoubleValue(-20.0),               
                   			"MinY", DoubleValue(-20.0), 
					"DeltaX", DoubleValue(20.0), 
					"DeltaY", DoubleValue(20.0),  
					"GridWidth", UintegerValue(10), 
					"LayoutType", StringValue("RowFirst"));
 
  std::stringstream ssSpeed;
  ssSpeed << "ns3::UniformRandomVariable[Min=150|Max=" << 240<< "]";
  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel","Bounds", RectangleValue(Rectangle(-120, 120, -120, 120)), "Speed", StringValue (ssSpeed.str()));
   mobility.Install (nodes);  



/////////*******************/////////////fww 
   
// 2. Install Mobility model
  // mobility.Install(nodes);
 
   // 3. Install NDN stack
   NS_LOG_INFO("Installing NDN stack");
   ndn::StackHelper ndnHelper;
   // ndnHelper.AddNetDeviceFaceCreateCallback (WifiNetDevice::GetTypeId (), MakeCallback
   // (MyNetDeviceFaceCallback));
   ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "1000");
   ndnHelper.SetDefaultRoutes(false);
   ndnHelper.Install(nodes);
   //set FIB
   ///std::string prefix = "/hello/ustc"; // some prefix
   //int metric = 1;
   //ndn::FibHelper::AddRoute(nodes.Get(0), prefix, nodes.Get(1), metric);
   //ndn::FibHelper::AddRoute(nodes.Get(1), prefix, nodes.Get(2), metric);
   //ndn::FibHelper::AddRoute(nodes.Get(2), prefix, nodes.Get(3), metric);
   //ndn::FibHelper::AddRoute(nodes.Get(3), prefix, nodes.Get(4), metric);
    
   // Set BestRoute strategy
   ndn::StrategyChoiceHelper::Install(nodes, "/", "/localhost/nfd/strategy/best-route");
    ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
    ndnGlobalRoutingHelper.InstallAll(); 
   // 4. Set up applications
   NS_LOG_INFO("Installing Applications");
 
   ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
   consumerHelper.SetPrefix("/hello/ustc");
   consumerHelper.SetAttribute("Frequency", StringValue("100"));
   consumerHelper.Install(nodes.Get(0)).Start(Seconds(0.0));
 //  consumerHelper.Install(nodes.Get(3)).Start(Seconds(0.0));
     //ApplicationContainer consumer = consumerHelper.Install(nodes.Get(0));
     //consumer.Start(Seconds(0));    
 //    consumer.Stop(Seconds(6)); 
 
   ndn::AppHelper producerHelper("ns3::ndn::Producer");
   producerHelper.SetPrefix("/hello/ustc");
   producerHelper.SetAttribute("PayloadSize", StringValue("1200"));
   producerHelper.Install(nodes.Get(2)).Start(Seconds(0.0));


   ndnGlobalRoutingHelper.AddOrigins("/hello/ustc", nodes.Get(2));
   ndn::GlobalRoutingHelper::CalculateRoutes();


 
   //tracing
   ndn::L3RateTracer::InstallAll("rate-trace.txt", Seconds(0.5));
   
   Simulator::Stop(Seconds(100.0));
   
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
