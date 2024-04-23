# Synchronisation-Algorithms-Distributed-Systems
Compilation of Clock, Message and Data Synchronisation Algorithms used in distributed systems for application in cloud computing

---
## Assignment Cloud Computing

### Problem Statement:
Source: Google Classroom
The project requires implementation of lamports Mutual exclusion using logical clock. The project has to be implemented via sockets because you need to communicate between 3 devices. The critical section can be assumed to be access to a file on any one of the 3 machines. As I explained in class, there need to be two threads per device. One thread shall create local event and send event; the send event should only be for mutual exclusion request. The other thread need to act as port listener and hence, receiver thread. The only catch here is to synchronise between sender and receiver thread to update the logical clock. The implementation needs to be done only in C++ or Java; I informed in class that Python is not allowed.

#### Summary:
- Implement Lamports Clock
- Implement Lamports Mutual Exclusion Algorithm
- 3 devices, 2 thread per device, only C++/Java allowed.
- Additional Credit for implementation on Cloud - After Class

### Flow Diagram & Simulation Technique
![FlowChart](https://github.com/Shreyas-SAS/Synchronisation-Algorithms-Distributed-Systems/assets/96542494/d246911a-d6dd-49b7-8533-ee59157c82e2)

### Report and Setup
Refer to this for understanding the project, setup for VM over Azure and Simulation better: [link](https://drive.google.com/file/d/1EBDxr7B4BJQJ7OZcFUKSqY9ZU4oupJs4/view?usp=sharing)

### Run directly without VM
- First turn off the laptop firewall and simulate within a full Global IP network or between devices of same private IP network since private IP conversion was not implemented.
- Download the code from github and keep it ready. 
- Now assuming you have a `.c` file ready.
- Now run `gcc device1.c -o device1` to compile the code
- Then the executable generated can be run using command `./device1` simultaneous to all other devices.
- Note: Try to minimize differences in starting the program executions.
