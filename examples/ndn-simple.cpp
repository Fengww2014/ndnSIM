/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

// ndn-simple.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/ndnSIM/helper/ndn-link-control-helper.hpp"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
namespace ns3 {

/**
 * This scenario simulates a very simple network topology:
 *
 *
 *      +----------+     1Mbps      +--------+     1Mbps      +----------+
 *      | consumer | <------------> | router | <------------> | producer |
 *      +----------+         10ms   +--------+          10ms  +----------+
 *
 *
 * Consumer requests data from producer with frequency 10 interests per second
 * (interests contain constantly increasing sequence number).
 *
 * For every received interest, producer replies with a data packet, containing
 * 1024 bytes of virtual payload.
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-simple
 */
/*
                 +---------------+
		 |    router     |
                 +---------------+    
               /                  \
              /                    \
             /                      \
 +----------+                       +------------+
 | consumer |<--------------------> | producer   |
 +----------+                       +------------+
             \                      /
              \                    /
               \                  /   
                \                /  
                 +---------------+
		 |    router     |
                 +---------------+    
*/
int
main(int argc, char* argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::QueueBase::MaxPackets", UintegerValue(20));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);
  //fww set up wifi
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
   wifiPhyHelper.Set("TxPowerStart", DoubleValue(5));
   wifiPhyHelper.Set("TxPowerEnd", DoubleValue(5));
 
   NqosWifiMacHelper wifiMacHelper = NqosWifiMacHelper::Default();
   wifiMacHelper.SetType("ns3::AdhocWifiMac");
  
  
  // Creating nodes
  NodeContainer nodes;
  nodes.Create(5);
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> initialAlloc = CreateObject<ListPositionAllocator> (); 
  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");


  for (uint32_t i = 0; i < nodes.GetN (); ++i) {
       //initialAlloc->Add (Vector (i*400, i,i));
	
       switch(i)
        {case 0:   initialAlloc->Add (Vector (0, 0, 0)); break;
	 case 1:   initialAlloc->Add (Vector (300, 150, 0)); break;
	 case 2:   initialAlloc->Add (Vector (300, -150, 0));break;
	 case 3:   initialAlloc->Add (Vector (450, 0, 0));break;
	 case 4:   initialAlloc->Add (Vector (600, 0, 0));break;
         
        }
	
   mobility.SetPositionAllocator(initialAlloc);
   }

  mobility.Install(nodes);
 //1.install wifi for all nodes
  NetDeviceContainer wifiNetDevices = wifi.Install(wifiPhyHelper, wifiMacHelper, nodes);      
//     Ptr<ConstantVelocityMobilityModel> mob =nodes.Get(4)->GetObject<ConstantVelocityMobilityModel>();    
     
//     mob->SetVelocity(Vector(50, 0, 0)); 

//mobility.Install(nodes);
    


 
// Connecting nodes using two links
/*
PointToPointHelper p2p;
p2p.Install(nodes.Get(0), nodes.Get(2));
p2p.Install(nodes.Get(2), nodes.Get(3));
p2p.Install(nodes.Get(0), nodes.Get(1));
p2p.Install(nodes.Get(3), nodes.Get(1));
*/
// 2.Install NDN stack on all nodes
ndn::StackHelper ndnHelper;
ndnHelper.SetDefaultRoutes(true);
ndnHelper.InstallAll();

// Choosing forwarding strategy
ndn::StrategyChoiceHelper::InstallAll("/prefix", "/localhost/nfd/strategy/best-route");

// Installing applications

// Consumer
ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
// Consumer will request /prefix/0, /prefix/1, ...
consumerHelper.SetPrefix("/prefix");
consumerHelper.SetAttribute("Frequency", StringValue("10")); // 10 interests a second
consumerHelper.Install(nodes.Get(0));                        // first node

// Producer
ndn::AppHelper producerHelper("ns3::ndn::Producer");
// Producer will reply to all requests starting with /prefix
producerHelper.SetPrefix("/prefix");
producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
producerHelper.Install(nodes.Get(3)); // last node
//Simulator::Schedule(Seconds(10.0), ndn::LinkControlHelper::FailLink, nodes.Get(0), nodes.Get(1));
//Simulator::Schedule(Seconds(15.0), ndn::LinkControlHelper::UpLink, nodes.Get(0), nodes.Get(1));
Simulator::Stop(Seconds(20.0));

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
