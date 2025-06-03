#include "FilterElement.h"
#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-l4-protocol.h"
#include "ns3/tcp-l4-protocol.h"

namespace ns3 {

    SourceIPAddress::SourceIPAddress(Ipv4Address new_value) {
        value = new_value;
    }
    SourceIPAddress::~SourceIPAddress() {}
    
    bool SourceIPAddress::match(Ptr<Packet> packet) {
        Ipv4Header ipHeader;
        packet->PeekHeader(ipHeader);
        Ipv4Address address = ipHeader.GetSource();

        return address == value;
    }

    SourceMask::SourceMask(Ipv4Address new_base, Ipv4Mask new_value) {
        base = new_base;
        value = new_value;
    }
    SourceMask::~SourceMask() {}

    bool SourceMask::match(Ptr<Packet> packet) {
        uint32_t baseB = base.Get();
        uint32_t maskB = value.Get();

        Ipv4Header ipHeader;
        packet->PeekHeader(ipHeader);
        Ipv4Address address = ipHeader.GetSource();

        uint32_t addressB = address.Get();

        return (addressB & maskB) == baseB;
    }

    SourcePortNumber::SourcePortNumber(uint32_t new_value) {
        value = new_value;
    }

    SourcePortNumber::~SourcePortNumber() {}

    bool SourcePortNumber::match(Ptr<Packet> packet) {
        Ptr<Packet> copyPackey = packet->Copy();
        PppHeader pppHeader;
        copyPackey->RemoveHeader(pppHeader);

        Ipv4Header ipv4Header;
        copyPackey->RemoveHeader(ipv4Header);

        uint16_t protocol = ipv4Header.GetProtocol();

        if (protocol == UdpL4Protocol::PROT_NUMBER) {
            UdpHeader udpHeader;
            copyPackey->PeekHeader(udpHeader);
            return udpHeader.GetSourcePort() == value;
        } else if (protocol == TcpL4Protocol::PROT_NUMBER) {
            TcpHeader tcpHeader;
            copyPackey->PeekHeader(tcpHeader);
            return tcpHeader.GetSourcePort() == value;
        }
        
        return false;
    }

    DestinationIPAddress::DestinationIPAddress(Ipv4Address new_value) {
        value = new_value;
    }
    DestinationIPAddress::~DestinationIPAddress() {}

    bool DestinationIPAddress::match(Ptr<Packet> packet) {
        Ipv4Header ipHeader;
        packet->PeekHeader(ipHeader);
        Ipv4Address address = ipHeader.GetDestination();

        return address == value;
    }

    DestinationMask::DestinationMask(Ipv4Address new_base, Ipv4Mask new_value) {
        base = new_base;
        value = new_value;
    }
    DestinationMask::~DestinationMask() {}

    bool DestinationMask::match(Ptr<Packet> packet) {
        uint32_t baseB = base.Get();
        uint32_t maskB = value.Get();

        Ipv4Header ipHeader;
        packet->PeekHeader(ipHeader);
        Ipv4Address address = ipHeader.GetDestination();

        uint32_t addressB = address.Get();

        return (addressB & maskB) == baseB;
    }

    DestinationPortNumber::DestinationPortNumber(uint32_t new_value) {
        value = new_value;
    }
    DestinationPortNumber::~DestinationPortNumber() {}
    bool DestinationPortNumber::match(Ptr<Packet> packet) {
        Ptr<Packet> copyPackey = packet->Copy();
        PppHeader pppHeader;
        copyPackey->RemoveHeader(pppHeader);

        Ipv4Header ipv4Header;
        copyPackey->RemoveHeader(ipv4Header);

        uint16_t protocol = ipv4Header.GetProtocol();

        if (protocol == UdpL4Protocol::PROT_NUMBER) {
            UdpHeader udpHeader;
            copyPackey->PeekHeader(udpHeader);
            return udpHeader.GetDestinationPort() == value;
        } else if (protocol == TcpL4Protocol::PROT_NUMBER) {
            TcpHeader tcpHeader;
            copyPackey->PeekHeader(tcpHeader);
            return tcpHeader.GetDestinationPort() == value;
        }

        return false;
    }

    ProtocolNumber::ProtocolNumber(uint32_t new_value) {
        value = new_value;
    }
    ProtocolNumber::~ProtocolNumber() {}
    bool ProtocolNumber::match(Ptr<Packet> packet) {
        Ipv4Header ipHeader;
        packet->PeekHeader(ipHeader);
        uint32_t protocol = ipHeader.GetProtocol();

        return protocol == value;
    }
}