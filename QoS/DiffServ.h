#ifndef DIFFSERV_H
#define DIFFSERV_H

#include "ns3/queue.h"
#include "ns3/queue-item.h"
#include <vector>
#include "TrafficClass.h"

namespace ns3 {
class DiffServ : public Queue<Packet> {
public:
    DiffServ(int n1);
    ~DiffServ ();

    std::vector<TrafficClass*>* Get_q_class();
    void Set_q_class(std::vector<TrafficClass*>* new_q_class);
    
    TrafficClass* getQueue(uint32_t i) const;

    virtual Ptr<Packet> Schedule() = 0;
    virtual uint32_t Classify(Ptr<Packet>) = 0;
    
private:
    virtual bool Enqueue (Ptr<Packet>) = 0;
    virtual Ptr<Packet> Dequeue () = 0;
    Ptr<const Packet> Peek () const override;
    Ptr<Packet> Remove () override;
    std::vector<TrafficClass*> q_class;
};

}

#endif