#ifndef DOG_H
#define DOG_H

#include "DiffServ.h"
#include "string.h"

namespace ns3 {
class SPQ : public DiffServ {
public:
    SPQ(int n);
    ~SPQ();

    Ptr<Packet> Schedule();
    uint32_t Classify(Ptr<Packet>);
    bool Enqueue (Ptr<Packet> packet);
    Ptr<Packet> Dequeue ();

    std::vector<TrafficClass*> q_class;
};
}
#endif