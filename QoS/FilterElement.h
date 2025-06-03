#ifndef FILTERELEMENT_H
#define FILTERELEMENT_H

#include "ns3/ipv4-header.h"
#include "ns3/packet.h"
#include "ns3/address.h"
#include "ns3/internet-module.h"

namespace ns3 {
    class FilterElement {
    public:
        virtual ~FilterElement() {}
        virtual bool match(Ptr<Packet> packet) = 0;
    };

    class SourceIPAddress : public FilterElement {
    private:
        Ipv4Address value;
    public:
        public:
        SourceIPAddress(Ipv4Address new_value);
        ~SourceIPAddress();
        bool match(Ptr<Packet> packet) override;
    };

    class SourceMask : public FilterElement {
    private:
        Ipv4Address base;
        Ipv4Mask value;
    public:
        SourceMask(Ipv4Address new_base, Ipv4Mask new_value);
        ~SourceMask();
        bool match(Ptr<Packet> packet) override;
    };

    class SourcePortNumber : public FilterElement {
    private:
        uint32_t value;
    public:
        SourcePortNumber(uint32_t new_value);
        ~SourcePortNumber();
        bool match(Ptr<Packet> packet) override;
    };

    class DestinationIPAddress : public FilterElement {
    private:
        Ipv4Address value;
    public:
        DestinationIPAddress(Ipv4Address new_value);
        ~DestinationIPAddress();
        bool match(Ptr<Packet> packet) override;
    };

    class DestinationMask : public FilterElement {
    private:
        Ipv4Address base;
        Ipv4Mask value;
    public:
        DestinationMask(Ipv4Address new_base, Ipv4Mask new_value);
        ~DestinationMask();
        bool match(Ptr<Packet> packet) override;
    };

    class DestinationPortNumber : public FilterElement {
    private:
        uint32_t value;
    public:
        DestinationPortNumber(uint32_t new_value);
        ~DestinationPortNumber();
        bool match(Ptr<Packet> packet) override;
    };

    class ProtocolNumber : public FilterElement {
    private:
        uint32_t value;
    public:
        ProtocolNumber(uint32_t new_value);
        ~ProtocolNumber();
        bool match(Ptr<Packet> packet) override;
    };
}


#endif /* FILTERELEMENT_H */