#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-control-module.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include "SPQ.h"

using namespace ns3;
using namespace rapidxml;
using namespace std;

int read_xml_file(std::string file_name, Ptr<SPQ> spq);
int read_xml_file(std::string file_name, Ptr<SPQ> spq) {
    std::ifstream xmlFile(file_name);

    if (!xmlFile) {
        std::cerr << "Error: Unable to open XML file." << std::endl;
        return -1;
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(xmlFile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');

    rapidxml::xml_document<> xmlDoc;
    try {
        xmlDoc.parse<0>(&buffer[0]);
    } catch (const rapidxml::parse_error& e) {
        std::cerr << "Error parsing XML file: " << e.what() << std::endl;
    }

    rapidxml::xml_node<>* rootNode = xmlDoc.first_node();
    
    if (!rootNode) {
        std::cerr << "Error: Invalid XML file." << std::endl;
        return -1;
    }

    int nQueue = 0;
    for (rapidxml::xml_node<>* trafficNode = rootNode->first_node("traffic"); trafficNode; trafficNode = trafficNode->next_sibling()) {
        rapidxml::xml_node<>* filtersNode = trafficNode->first_node("filters");
        for (rapidxml::xml_node<>* filterNode = filtersNode->first_node(); filterNode; filterNode = filterNode->next_sibling()) {
            Filter* filter = new Filter();
            rapidxml::xml_node<>* des_ip_node = filterNode->first_node("des_ip");
            if (des_ip_node) {
                std::string des_ip = des_ip_node->value();
                Ipv4Address ipAddress = Ipv4Address(des_ip.c_str());
                filter->addElement(new DestinationIPAddress(ipAddress));
            }

            rapidxml::xml_node<>* des_port_node = filterNode->first_node("des_port");
            if (des_port_node) {
                int des_port = std::stoi(des_port_node->value());
                filter->addElement(new DestinationPortNumber(des_port));
            }

            rapidxml::xml_node<>* src_port_node = filterNode->first_node("src_port");
            if (src_port_node) {
                int src_port = std::stoi(src_port_node->value());
                filter->addElement(new SourcePortNumber(src_port));
            }

            rapidxml::xml_node<>* src_ip_node = filterNode->first_node("src_ip");
            if (src_ip_node) {
                std::string src_ip = src_ip_node->value();
                Ipv4Address ipAddress = Ipv4Address(src_ip.c_str());
                filter->addElement(new SourceIPAddress(ipAddress));
            }

            rapidxml::xml_node<>* src_mask_node = filterNode->first_node("src_mask");
            if (src_mask_node) {
                std::string src_mask = src_mask_node->value();
                Ipv4Mask mask = Ipv4Mask(src_mask.c_str());
                Ipv4Address base = "192.168.0.0";
                filter->addElement(new SourceMask(base, mask));
            }

            rapidxml::xml_node<>* des_mask_node = filterNode->first_node("des_mask");
            if (des_mask_node) {
                std::string des_mask = des_mask_node->value();
                Ipv4Mask mask = Ipv4Mask(des_mask.c_str());
                Ipv4Address base = "192.168.0.0";
                filter->addElement(new DestinationMask(base, mask));
            }

            rapidxml::xml_node<>* protocol_num_node = filterNode->first_node("protocol_num");
            if (protocol_num_node) {
                int protocol_num = std::stoi(protocol_num_node->value());
                ProtocolNumber protocolNumber(protocol_num);
                filter->addElement(new ProtocolNumber(protocol_num));
            }

            spq->getQueue(nQueue)->addFilter(filter);
        }

        rapidxml::xml_node<>* priority_node = trafficNode->first_node("priority");
        if (!priority_node) {
            return -1;
        }
        else {
            uint32_t priority = static_cast<uint32_t>(std::stoul(priority_node->value()));
            spq->getQueue(nQueue)->setWeight(priority);
        }

        nQueue++;
    }

    return 1;
}

void SPQsimulator(std::string file_name)
{
    uint16_t port1 = 4000;
    uint16_t port2 = 5000;
    uint32_t maxPacketCount = 20000;
    
    double PACKET_INTERVAL = 0.0001;
    double SERVER_START_TIME = 1.0;
    double SERVER_END_TIME = 30.0;
    double CLIENT1_START_TIME = 3.0;
    double CLIENT1_END_TIME = 5.0;
    double CLIENT2_START_TIME = 1.0;
    double CLIENT2_END_TIME = 30.0;
    
    std::string DATARATE1 = "40Mbps";
    std::string DATARATE2 = "10Mbps";
    std::string DEFAULT_DELAY = "2ms";
    Ipv4Mask IPV4_MASK = "255.255.255.0";
    Ipv4Address IPV4_ADDR1 = "10.1.1.0";
    Ipv4Address IPV4_ADDR2 = "10.1.2.0";

    Time::SetResolution(Time::NS);

    // 3 Nodes topology
    NodeContainer nodes;
    nodes.Create(3);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue(DATARATE1));
    pointToPoint.SetChannelAttribute("Delay", StringValue(DEFAULT_DELAY));
    NetDeviceContainer device1 = pointToPoint.Install(nodes.Get(0), nodes.Get(1));

    pointToPoint.SetDeviceAttribute("DataRate", StringValue(DATARATE2));
    pointToPoint.SetChannelAttribute("Delay", StringValue(DEFAULT_DELAY));
    NetDeviceContainer device2 = pointToPoint.Install(nodes.Get(1), nodes.Get(2));

    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper address;

    address.SetBase(IPV4_ADDR1, IPV4_MASK);
    Ipv4InterfaceContainer interfaces1 = address.Assign(device1);

    address.SetBase(IPV4_ADDR2, IPV4_MASK);
    Ipv4InterfaceContainer interfaces2 = address.Assign(device2);

    Ptr<SPQ> spq = CreateObject<SPQ>(2);

    if (read_xml_file(file_name, spq) < 0) {
        std::cerr << "Error: Can't read file." << std::endl;
        return;
    }

    nodes.Get(1)->GetDevice(1)->GetObject<PointToPointNetDevice>()->SetQueue(spq);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    UdpServerHelper server1(port1);
    ApplicationContainer serverApp1 = server1.Install(nodes.Get(2));
    serverApp1.Start(Seconds(SERVER_START_TIME));
    serverApp1.Stop(Seconds(SERVER_END_TIME));

    UdpServerHelper server2(port2);
    ApplicationContainer serverApp2 = server2.Install(nodes.Get(2));
    serverApp2.Start(Seconds(SERVER_START_TIME));
    serverApp2.Stop(Seconds(SERVER_END_TIME));

    // Client 1
    Time interPacketInterval = Seconds(PACKET_INTERVAL);
    UdpClientHelper client1(interfaces2.GetAddress(1), port1);
    client1.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
    client1.SetAttribute("Interval", TimeValue(interPacketInterval));
    client1.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApp1 = client1.Install(nodes.Get(0));
    clientApp1.Start(Seconds(CLIENT1_START_TIME));
    clientApp1.Stop(Seconds(CLIENT1_END_TIME));

    // Client 2
    UdpClientHelper client2(interfaces2.GetAddress(1), port2);
    client2.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
    client2.SetAttribute("Interval", TimeValue(interPacketInterval));
    client2.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApp2 = client2.Install(nodes.Get(0));
    clientApp2.Start(Seconds(CLIENT2_START_TIME));
    clientApp2.Stop(Seconds(CLIENT2_END_TIME));

    // store pcap
    pointToPoint.EnablePcapAll("scratch/QoS/SPQ_pcap/SPQ");

    Simulator::Run();
    Simulator::Destroy();
}

