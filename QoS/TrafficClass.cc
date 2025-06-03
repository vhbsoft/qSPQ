#include "TrafficClass.h"
using namespace std;
namespace ns3 {
    TrafficClass::TrafficClass() {
        packets = 0;
        maxPackets = 20000;
    }

    void TrafficClass::addFilter(Filter* filter) {
        if (filter == nullptr) {
            return;
        } 
        filters.push_back(filter);
    }

    Ptr<Packet> TrafficClass::Front() {
        return m_queue.front();
    }

    bool TrafficClass::Enqueue(Ptr<Packet> packet) {
        if (packets >= maxPackets) {
            return false;
        }

        m_queue.push(packet);
        packets++;
        return true;
    }

    Ptr<Packet> TrafficClass::Dequeue(void) {
        if (m_queue.empty()) {
            return nullptr;
        }

        Ptr<Packet> packet = m_queue.front();
        m_queue.pop();
        packets--;

        return packet;
    }

    void TrafficClass::setWeight(uint32_t priority) {
        priority_level = priority;
    }

    uint32_t TrafficClass::getWeight() {
        return priority_level;
    }

    bool TrafficClass::empty() {
        return m_queue.empty();
    } 

    bool TrafficClass::match(Ptr<Packet> packet) {
        for (std::size_t i = 0; i < filters.size(); i++) {
            Filter* filter = filters[i];
            if (filter->match(packet)) {
                return true;
            }
        }

        return false;
    }
}