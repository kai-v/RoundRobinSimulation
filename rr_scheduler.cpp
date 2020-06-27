
#include <iostream>
#include "process.h"
#include "rr_scheduler.h"
#include <queue>

RR_Scheduler::RR_Scheduler(std::queue<process> process_list, int quantum, int block_duration)
{
    this->process_list = process_list;
    this->quantum = quantum;
    this->block_duration = block_duration;
}
RR_Scheduler::~RR_Scheduler()
{
    // destructor does nothing
}

// create the round-robin algorithm in a private function
// for encapsulation and security reason
void RR_Scheduler::RR()
{
    std::cout << "*************************************************\n";

    int num_finished = 0;
    int simulation_time = 0;
    // queues to keep track of ready_queue, wait_queue, terminated processes
    std::queue<process> ready_queue;
    std::queue<process> wait_queue;
    std::queue<process> finished_queue;
    bool curr_process_is_valid = true;

    int num_processes = this->process_list.size();
    bool cpu_is_idle = false;

    // status codes for why a process gets switched out
    char status_codes[] = {'I', 'Q', 'B', 'T'};
    char status_code = ' ';

    int start_time = -1;
    int finished_time = -1;
    int terminated_time = -1;

    // keep track of previous process's name, so when a new process start
    // we know to print out the result line for why the previous process
    // get switched out, its start time and how long it run.
    std::string prev_process = "";

    while (num_finished < num_processes)
    {
        // check arrival time of latest process in the list
        if (!process_list.empty() && simulation_time >= process_list.front().get_arrival_time())
        {
            // remove process and add process to ready_queue list
            ready_queue.push(process_list.front());
            process_list.pop();
        }

        if (!ready_queue.empty())
        {
            // if the cpu was previously idle, then we print the result in this iteration.
            if (status_code == 'I')
            {
                finished_time = simulation_time;
                std::cout << start_time << " " << prev_process << " " << finished_time - start_time << " " << status_code << std::endl;
                // reset status code
                status_code = ' ';
            }

            cpu_is_idle = false;

            // current active process running
            process &curr_process = ready_queue.front();
            curr_process_is_valid = true;

            // if this process is different from the previous process
            // update the prev_process and set the start time of the
            // current NEW process to curr_time
            if (curr_process.get_name().compare(prev_process) != 0)
            {
                prev_process = std::string(curr_process.get_name());
                start_time = simulation_time;
            }
            // check if process has used up its quantum
            if (curr_process.get_quantum_left() == 0)
            {
                // process has used its quantum
                // just move to end of ready_queue since it might resume soon
                ready_queue.pop();
                // reset the quantum for next run
                // keep the same cpu_burst_left
                curr_process.set_quantum_left(quantum);
                ready_queue.push(curr_process);

                finished_time = simulation_time;
                status_code = status_codes[1];
                // if there is process is the only process in the queue and gets preempted,
                // it is going to resume again in the same clock-cycle anyway.
                // don't print "Q" output since it actually doesn't really get preempted.
                if (ready_queue.size() > 1)
                {
                    std::cout << start_time << " " << prev_process << " " << finished_time - start_time << " " << status_code << std::endl;
                }
                // current process technically finished BEFORE this iteration, thus, we should
                // decrement the current time, so when a process starts in the next iteration,
                // it is going to begin at the same clock-cycle.
                simulation_time--;
                curr_process_is_valid = false;
            }
            else if (curr_process.get_cpu_burst_left() == 0)
            {
                // if process hasn't used up its quantum, yet it is about to block for i/o
                // remove from ready_queue and add to wait_queue
                ready_queue.pop();
                // set the point in time where this process blocks
                curr_process.set_time_since_blocked(simulation_time);
                // reset quantum_left for this process
                curr_process.set_quantum_left(quantum);
                // reset cpu_burst for next run
                curr_process.set_cpu_burst_left(curr_process.get_cpu_burst());
                wait_queue.push(curr_process);
                // current process is not valid because it's blocked currently
                curr_process_is_valid = false;

                // print output
                finished_time = simulation_time;
                status_code = status_codes[2];
                std::cout << start_time << " " << prev_process << " " << finished_time - start_time << " " << status_code << std::endl;
                // current process is blocked so technically another process should start in this clock-cycle
                // decrement simulation_time so next iteration starts at the same clock-cycle
                simulation_time--;
            }
            // if current process is still active
            if (curr_process_is_valid)
            {
                // decrements
                curr_process.set_cpu_burst_left(curr_process.get_cpu_burst_left() - 1);
                curr_process.set_quantum_left(curr_process.get_quantum_left() - 1);
                curr_process.set_total_time_left(curr_process.get_total_time_left() - 1);
                if (curr_process.get_total_time_left() == 0)
                {
                    // current process has now finished
                    num_finished++;
                    // since we check one iteration too early, so we set finished time to the next clock-cycle.
                    finished_time = simulation_time + 1;
                    curr_process.set_finished_time(finished_time);
                    // removed from ready_queue list and add to finished_queue.
                    ready_queue.pop();
                    finished_queue.push(curr_process);
                    // current process is not active
                    curr_process_is_valid = false;
                    // print output with status "T"
                    status_code = status_codes[3];
                    std::cout << start_time << " " << prev_process << " " << finished_time - start_time << " " << status_code << std::endl;
                    simulation_time--;
                }
            }
        }
        else
        {
            // if there are no active processes to be run,
            // CPU IS IDLE
            cpu_is_idle = true;

            prev_process = std::string("[IDLE]");
            // start time of current [IDLE] state is the last active process's finished time
            start_time = finished_time;
            status_code = status_codes[0];
        }

        // check if any wait_queue process can be moved back to ready_queue list
        if (!wait_queue.empty() && (simulation_time - wait_queue.front().get_time_since_blocked() >= block_duration))
        {
            // if a process has finished blocking, move it back to the ready_queue list
            process active_again = wait_queue.front();
            wait_queue.pop();
            active_again.set_time_since_blocked(-1);
            ready_queue.push(active_again);

            if (cpu_is_idle && !ready_queue.empty())
            {
                simulation_time--;
            }
        }
        simulation_time++;
    }
    std::cout << finished_time << " [END]\n";

    std::cout << "*****************************************************************************************\n";

    int turnaround_sum = 0;
    while (!finished_queue.empty())
    {
        process p = finished_queue.front();
        finished_queue.pop();
        std::cout << "Process " << p.get_name() << " " << p.get_finished_time() - p.get_arrival_time() << ".\n";
        turnaround_sum += (p.get_finished_time() - p.get_arrival_time());
    }

    // print average turnover time
    std::cout << (float)turnaround_sum / num_finished << std::endl;
}

// outsider can only access the public run()
void RR_Scheduler::run()
{
    RR();
}