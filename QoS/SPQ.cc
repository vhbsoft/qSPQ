#include "SPQ.h"
#include "ns3/log.h"

using namespace std;
namespace ns3 {

NS_LOG_COMPONENT_DEFINE("SPQ");

NS_OBJECT_ENSURE_REGISTERED(SPQ);

SPQ::SPQ(int n = 1) : DiffServ(n) {}
SPQ::~SPQ() {}

bool SPQ::Enqueue (Ptr<Packet> packet) {
    int index = Classify(packet);
    std::vector<TrafficClass*>* q_class = Get_q_class();
    (*q_class)[index]->Enqueue(packet);

    return true;
}

Ptr<Packet> SPQ::Dequeue () { 
  return SPQ::Schedule(); 
}

Ptr<Packet> SPQ::Schedule() {
  std::vector<TrafficClass*>* q_class = Get_q_class();
  for (std::size_t i = 0; i < q_class->size(); i++) {
    TrafficClass *queue = (*q_class)[i];
      if (!queue->empty()) {
        return queue->Dequeue();
      }
  }

  return nullptr;
}

uint32_t SPQ::Classify(Ptr<Packet> packet) {
  std::vector<TrafficClass*>* q_class = Get_q_class();

  TrafficClass* queue0 = (*q_class)[0];
  if (queue0->match(packet)) {
    return 0;
  } else {
    return 1;
  }

  // for (std::size_t i = 0; i < q_class->size(); i++) {
  //   TrafficClass* queue = (*q_class)[i];
  //   if (queue->match(packet)) {
  //      return i;
  //   }
  // }

  // return q_class->size() - 1;
}

}