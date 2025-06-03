#ifndef DRR_H
#define DRR_H

#include "ns3/queue-disc.h"
#include "ns3/drop-tail-queue.h"

#include "DiffServ.h"

namespace ns3 {
class DRR : public DiffServ {
public:
    DRR(int n);
    ~DRR();

    void setQuanta(uint32_t quanta);

    Ptr<Packet> Schedule();
    uint32_t Classify(Ptr<Packet>);
    bool Enqueue (Ptr<Packet> packet) override;
    Ptr<Packet> Dequeue () override;

private:
    uint32_t quanta;
    uint32_t round_count = 0;
    std::vector<int> remainedQuanta = {0, 0, 0};
};

}

#endif