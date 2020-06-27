#ifndef RR_SCHEDULER_H
#define RR_SCHEDULER_H

#include <queue>
#include "process.h"
class RR_Scheduler
{
private:
    std::queue<process> process_list;
    int block_duration;
    int quantum;
    void RR();

public:
    RR_Scheduler(std::queue<process> process_list, int quantum, int block_duration);
    ~RR_Scheduler();
    void run();
};

#endif