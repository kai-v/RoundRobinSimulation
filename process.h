#ifndef PROCESS_H
#define PROCESS_H

#include <string>
class process
{
private:
    std::string name;
    int arrival_time = -1;
    int total_runtime = -1;
    int cpu_burst = -1;
    int quantum_left = -1;
    int cpu_burst_left = -1;
    int time_since_blocked = -1;
    int total_time_left = -1;
    int finished_time = -1;

public:
    process(std::string name, int arrival_time, int total_runtime, int cpu_burst);
    ~process();
    // setters
    void set_quantum_left(int quantum_left);
    void set_cpu_burst_left(int cpu_burst_left);
    void set_time_since_blocked(int time_since_blocked);
    void set_total_time_left(int total_time_left);
    void set_finished_time(int finished_time);
    // getters
    int get_quantum_left();
    int get_cpu_burst_left();
    int get_arrival_time();
    int get_total_time();
    int get_cpu_burst();
    int get_time_since_blocked();
    int get_total_time_left();
    int get_finished_time();
    std::string get_name();
};

#endif