#include "Filter.h"

using namespace std;

namespace ns3 {
    Filter::Filter() {}
    Filter::~Filter() {
        for (auto element : elements) {
            delete element;
        }
    }

    void Filter::addElement(FilterElement* element) {
        elements.push_back(element);
    }

    bool Filter::match(Ptr<Packet> packet) {
        if (elements.size() == 0){
            return false;
        }
        for (auto element: this->elements) {
            if (!element->match(packet)) {
                return false;
            }
        }
        return true;
    }
}
