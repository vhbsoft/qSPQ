#include "QSPQ.h"
#include "ns3/log.h"

using namespace std;
namespace ns3 {

NS_LOG_COMPONENT_DEFINE("QSPQ");

NS_OBJECT_ENSURE_REGISTERED(QSPQ);

// QSPQ::QSPQ(int n = 1) : DiffServ(n) {}
QSPQ::QSPQ(int n)
    : DiffServ(n) {}

QSPQ::~QSPQ() {}

void QSPQ::set_QH_probability(double probability) {
  if (probability < 0 || probability > 1) {
    std::cout << "QH_probability must be between 0 and 1" << std::endl;
    return;
  }
  QH_probability = probability;
}

void QSPQ::set_QL_probability(double probability) {
  if (probability < 0 || probability > 1) {
    std::cout << "QL_probability must be between 0 and 1" << std::endl;
    return;
  }
  QSPQ::QL_probability = probability;
}

bool QSPQ::Enqueue (Ptr<Packet> packet) {
  int index = Classify(packet);
  std::vector<TrafficClass*>* q_class = Get_q_class();
  (*q_class)[index]->Enqueue(packet);

  return true;
}

Ptr<Packet> QSPQ::Dequeue () {
  return QSPQ::Schedule(); 
}

Ptr<Packet> QSPQ::Schedule() {
  std::vector<TrafficClass*>* q_class = Get_q_class();
  for (std::size_t i = 0; i < q_class->size(); i++) {
    TrafficClass *queue = (*q_class)[i];
    if (!queue->empty()) {
      return queue->Dequeue();
    }
  }
  
  return nullptr;
}

uint32_t QSPQ::Classify(Ptr<Packet> packet) {
  UdpHeader udpHeader;

  // Parse the headers
  packet->PeekHeader(udpHeader);

  // Use probabilities for low-priority classification
  Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable>();
  double randVal = uv->GetValue();

  // Example: Classify packets based on destination port
  if (udpHeader.GetDestinationPort() == 4000) { // Client 1 destination port
      if (randVal < QH_probability) {
          return 0; // High priority
      } else {
          return 1; // Low priority
      }
      return 0; // High priority queue
  } else if (udpHeader.GetDestinationPort() == 5000) { // Client 2 destination port
      if (randVal < QL_probability) {
          return 1; // Low priority
      } else {
          return 0; // High priority
      }
  }

  return 1; // Low priority queue

  /*
  TrafficClass* queue0 = (*q_class)[0];
  if (queue0->match(packet)) {
    return 0;
  } else {
    return 1;
  }
  */
}

}