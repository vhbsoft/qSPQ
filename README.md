# 👋 QSPQ

## Location
Put the whole folder under `ns-3.38/scratch`.

## How to run it
Make sure the QSPQ simulator is uncommented in `main.cc`

Run this in terminal under `ns-3.38/`:
```./ns3 run main -- --qos=qspq --filepath=scratch/QoS/qspq_config.xml```

## ✅ Completed Work
- Created QSPQ module files (`QSPQ.cc` / `QSPQ.h`)
- QSPQ module is wired into the simulation environment
- Set up two queues: `q_class[0]` (high priority), `q_class[1]` (low priority)
- Implemented basic priority classification using `return 0 / return 1` (currently hardcoded)
- Simulation has been run and output `.pcap` files are available in the `QSPQ_pcap` folder

## 🚧 Incomplete Work
- Dynamic queue-size-based classification logic not yet implemented
- Threshold variable for queue size not yet added (currently hardcoded)
- No classification statistics (e.g., how many packets assigned high vs. low priority)
- Not integrated with DiffServ or Filter modules
- No testing of edge cases like queue overflows or threshold dynamics

## 🔧 Suggested Next Steps
- Add queue-size-based logic in `QSPQ.cc`, such as:
```cpp
if (queue0->GetQueueSize() > threshold) {
    return 1; // assign to low priority
} else {
    return 0; // assign to high priority
}
```
- Introduce a configurable threshold parameter (e.g., `int threshold = 100;`)
- Add logging via `NS_LOG_DEBUG` or similar to trace priority classification
- Optionally, track classification stats (e.g., number of packets per priority)
- Finally, integrate the logic into the broader DiffServ or traffic filtering framework

## 📦 Simulation Output & Testing
- Simulation results are saved in the `QSPQ_pcap/` folder
- Use Wireshark or `tcpdump` to analyze packet behaviors
- The module was tested using bash scripts or manual compilation

---
Feel free to refer to `SPQ.cc` as a structural reference. The foundation is already in place — the main next step is completing the queue-based classification and enriching the logic with data tracking. Good luck! 🙌