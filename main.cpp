#include <queue>
#include <vector>
#include "process.h"
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "rr_scheduler.h"

int main(int argc, char *argv[])
{
    // error checking for correct number of arguments. Prompt user correct input format if not.
    if (argc != 4)
    {
        std::cerr << "Invalid number of arguments. Valid command is in this form.\n ./rr_scheduler <input_filename> <block_duration> <quantum>.";
        exit(EXIT_FAILURE);
    }
    // store command-line arguments
    std::string file_name = argv[1];
    int block_duration = std::stoi(argv[2]);
    int quantum = std::stoi(argv[3]);

    // open stream to input file
    std::ifstream input_stream(file_name);
    // error-checking if file specified can be opened or not.
    if (input_stream.fail())
    {
        throw std::runtime_error("Error. File " + file_name + " cannot be opened or is not found.");
    }

    std::string current_line;

    std::string process_name;
    int process_arrival_time;
    int process_total_time;
    int process_cpu_burst;
    // temporary buffer to hold token to be assigned to correct variable
    std::string tmp;
    // positional index to know which field we are reading the token from
    int token_idx = 0;
    std::queue<process> process_list;

    // code to read input and create processes with
    // specified parameters
    while (std::getline(input_stream, current_line))
    {
        std::istringstream stream(current_line);
        while (stream >> tmp)
        {
            if (token_idx == 0)
            {
                process_name = std::string(tmp);
            }
            else if (token_idx == 1)
            {
                process_arrival_time = std::stoi(std::string(tmp));
            }
            else if (token_idx == 2)
            {
                process_total_time = std::stoi(std::string(tmp));
            }
            else
            {
                process_cpu_burst = std::stoi(std::string(tmp));
            }
            token_idx++;
        }
        // reset index
        token_idx = 0;
        // create process object
        process p(process_name, process_arrival_time, process_total_time, process_cpu_burst);
        // since quantum is a dynamically-determined value, set the quantum for each process at runtime.
        p.set_quantum_left(quantum);
        // add created process to queue
        process_list.push(p);
    }

    // make round robin scheduler object
    RR_Scheduler scheduler(process_list, quantum, block_duration);
    scheduler.run();
}
