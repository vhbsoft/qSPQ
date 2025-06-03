#include "ns3/core-module.h"
#include "DRRsimulator.h"
#include "SPQsimulator.h"
#include "QSPQsimulator.h"

using namespace ns3;
using namespace std;

// void DRRsimulator();
// void SPQsimulator();
// void QSPQsimulator();

int main(int argc, char* argv[])
{
    string qos;
    string filepath;

    CommandLine cmd;
    cmd.AddValue("qos", "'spq' or 'drr' or 'qspq'", qos);
    cmd.AddValue("filepath", "config xml file name", filepath);
    cmd.Parse(argc, argv);

    if (qos == "spq") {
        SPQsimulator(filepath);
    } else if (qos == "drr") {
        DRRsimulator(filepath);
    } else if (qos == "qspq") {
        QSPQsimulator(filepath);
    } else {
        cout << "Please input betwee 'spq' and 'drr'." << endl;
        return 1;
    }

    return 0;
}