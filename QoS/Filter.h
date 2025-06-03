#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include "ns3/queue.h"
#include "ns3/queue-item.h"
#include "FilterElement.h"

namespace ns3 {
    class Filter {
    public:
        Filter();
        ~Filter();
        bool match(Ptr<Packet> packet);
        void addElement(FilterElement* element);
        std::vector<ns3::FilterElement*> elements;
    };
}

#endif