#include <string>
#include "process.h"

process::process(std::string name, int arrival_time, int total_runtime, int cpu_burst)
{
    this->name = name;
    this->arrival_time = arrival_time;
    this->total_runtime = total_runtime;
    this->cpu_burst = cpu_burst;
    this->cpu_burst_left = cpu_burst;
    this->total_time_left = total_runtime;
}

process::~process()
{
}

// setters
void process::set_quantum_left(int quantum)
{
    this->quantum_left = quantum;
}
void process::set_cpu_burst_left(int cpu_burst_left)
{
    this->cpu_burst_left = cpu_burst_left;
}

void process::set_total_time_left(int total_time_left)
{
    this->total_time_left = total_time_left;
}

void process::set_time_since_blocked(int time_since_blocked)
{
    this->time_since_blocked = time_since_blocked;
}

void process::set_finished_time(int finished_time)
{
    this->finished_time = finished_time;
}
// getters
int process::get_quantum_left()
{
    return this->quantum_left;
}
int process::get_cpu_burst_left()
{
    return this->cpu_burst_left;
}

int process::get_arrival_time()
{
    return this->arrival_time;
}
int process::get_total_time()
{
    return this->total_runtime;
}
int process::get_cpu_burst()
{
    return this->cpu_burst;
}

int process::get_time_since_blocked()
{
    return this->time_since_blocked;
}

int process::get_total_time_left()
{
    return this->total_time_left;
}

int process::get_finished_time()
{
    return this->finished_time;
}

std::string process::get_name()
{
    return this->name;
}