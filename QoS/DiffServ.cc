#include "DiffServ.h"

using namespace std;
namespace ns3 {

    DiffServ::DiffServ(int n)
    {
        for (int i = 0; i < n; i++) {
            TrafficClass* tc = new TrafficClass();
            q_class.push_back(tc);
        }
    }

    DiffServ::~DiffServ () {
        for (TrafficClass* tc : q_class) {
            delete tc;
        }
    }

    std::vector<TrafficClass*>* DiffServ::Get_q_class() {
        return &q_class;
    }

    void DiffServ::Set_q_class(std::vector<TrafficClass*>* new_q_class) {
        this->q_class = *new_q_class;
    }

    TrafficClass* DiffServ::getQueue(uint32_t i) const {
        return q_class[i];
    }
    
    Ptr<const Packet> DiffServ::Peek () const { return nullptr; }

    Ptr<Packet> DiffServ::Remove () { return nullptr; }

    std::vector<TrafficClass*> q_class;
}