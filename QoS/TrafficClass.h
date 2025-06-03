#ifndef TRAFFIC_CLASS_H
#define TRAFFIC_CLASS_H

#include <math.h>
#include <queue>
#include "ns3/queue.h"
#include "ns3/queue-item.h"
#include <vector>
#include "Filter.h"

namespace ns3 {
class TrafficClass {
public:
    TrafficClass();
    bool Enqueue(Ptr<Packet>);
    Ptr<Packet> Dequeue(void);
    bool match(Ptr<Packet>);
    bool empty();
    void setWeight(uint32_t priority);
    uint32_t getWeight();
    Ptr<Packet> Front();
    void addFilter(Filter* filter);

private:
    uint32_t bytes;
    uint32_t packets;
    uint32_t maxPackets;
    uint32_t maxBytes;
    double_t weight;
    uint32_t priority_level;
    std::queue<Ptr<Packet>> m_queue;
public:
    std::vector<ns3::Filter*> filters;
};
}
#endif