#ifndef QSPQ_H
#define QSPQ_H

#include "DiffServ.h"
#include "string.h"

namespace ns3 {
class QSPQ : public DiffServ {
public:
    QSPQ(int n=1);
    virtual ~QSPQ();

    Ptr<Packet> Schedule();
    uint32_t Classify(Ptr<Packet>);
    bool Enqueue (Ptr<Packet> packet);
    Ptr<Packet> Dequeue ();
    void set_QH_probability(double probability);
    void set_QL_probability(double probability);

    std::vector<TrafficClass*> q_class;

private:
    double QH_probability;
    double QL_probability;
};

}

#endif