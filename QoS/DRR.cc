#include "DRR.h"

using namespace std;
namespace ns3 {

    DRR::DRR(int n = 1) : DiffServ(n) {}
    DRR::~DRR() { }

    void DRR::setQuanta(uint32_t quanta) {
        quanta = quanta;
    }

    bool DRR::Enqueue (Ptr<Packet> packet) {
        int index = Classify(packet);
        std::vector<TrafficClass*>* q_class = Get_q_class();

        if (index < 0) {
            (*q_class)[q_class->size() - 1]->Enqueue(packet);
        } else {
            (*q_class)[index]->Enqueue(packet);
        }

        return true;
    }

    Ptr<Packet> DRR::Dequeue () { 
        return Schedule();
    }

    Ptr<Packet> DRR::Schedule() {
        std::vector<TrafficClass*>* q_class = Get_q_class();
        std::vector<Ptr<Packet>> packets;
        int empty_count = 0;

        while (empty_count <= 3) {
            uint32_t i = round_count % 3;
            TrafficClass* tc = (*q_class)[i];
            uint32_t curQuanta = quanta * tc->getWeight();

            if (!tc->empty()) {
                empty_count = 0;
                if (curQuanta + remainedQuanta[i] > tc->Front()->GetSize()) {
                    remainedQuanta[i] += curQuanta;
                    remainedQuanta[i] -= tc->Front()->GetSize();
                    round_count++;
                    return tc->Dequeue();
                } else {
                    remainedQuanta[i] += curQuanta;
                    round_count++;
                    continue;
                }
            } else {
                round_count++;
                empty_count++;
            }
            
        }

        return nullptr;
    }

    uint32_t DRR::Classify(Ptr<Packet> packet) {
        std::vector<TrafficClass*>* q_class = Get_q_class();

        for (std::size_t i = 0; i < q_class->size(); i++) {
            TrafficClass* queue = (*q_class)[i];
            
            if (queue->match(packet)) {
                return i;
            }
        }
        return -1;
    }

}