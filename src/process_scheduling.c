#define _FILE_OFFSET_BITS 64

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include "dyn_array.h"
#include "processing_scheduling.h"
#include "bits/types.h"
#include <time.h>




// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

/**
 * @brief Runs the First Come First Served Process Scheduling algorithm over the incoming ready_queue
 * 
 * @param ready_queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
 * @param result used for first come first served stat tracking \ref ScheduleResult_t
 * @return true if function ran successful
 * @return false for an error
 */
bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if(ready_queue == NULL || result == NULL)
    {
        return false;
    }

    float total_Run = 0;
    float total_Turn = 0;
    float total_Wait = 0;
    float deadTime = 0;
    size_t queueLength = dyn_array_size(ready_queue);
    size_t i;
    for (i = 0; i < queueLength; i++)
    {
        void* head = dyn_array_at(ready_queue, i);
        if(head != NULL)
        {
            ProcessControlBlock_t * controlHead = (ProcessControlBlock_t *) head;
            if(controlHead->started == false)
            {
                controlHead->started = true;
                //calculate the wait time
                total_Wait += (total_Run - controlHead->arrival);

                // if nothing was in the queue yet after the a process was completed. 
                // The CPU would be doing nothing but waiting, therefore adding time to the total time to process all the PCBs
                if(controlHead->arrival > total_Run)
                {
                    deadTime += (controlHead->arrival - total_Run);
                }

                // Run the current process and get the burst time aka "run time"
                total_Run += (controlHead->remaining_burst_time);
                total_Turn += total_Run;
            }
        }
    }
    result->average_waiting_time = (total_Wait + deadTime) / queueLength;
    result->average_turnaround_time = total_Turn / queueLength;
    result->total_run_time = total_Run;

    return true;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

/**
 * @brief DO NOT NEED TO DO.
 * ANDERSEN SAID WE DO NOT NEED TO
 */
bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    //Need to use the bool started in this algorithm
    //Chnadra wouldn't use started he would use a something else from the struct
    //We can create our own variables in the struct
    UNUSED(ready_queue);
    UNUSED(result);
    UNUSED(quantum);
    return false;
}

// Reads the PCB burst time values from the binary file into ProcessControlBlock_t remaining_burst_time field
// for N number of PCB burst time stored in the file.
// \param input_file the file containing the PCB burst times
// \return a populated dyn_array of ProcessControlBlocks if function ran successful else NULL for an error
dyn_array_t *load_process_control_blocks(const char *input_file) 
{
    FILE *fp;
    if (input_file)
    {
        //open the file in read binary mode
        fp = fopen(input_file, "rb");
        
        //check if file opened succesfully
        if (fp == NULL) return NULL;

        //find the size in bytes of the file
        fseek(fp, 0 , SEEK_END);
        long filelen = ftell(fp);

        //find the number of total uint32s in the file
        long NumofUint32Actual = filelen/4;

        //set file pointer back to beginning of file so it can be read
        rewind(fp);

        //grab first uint32 to find number of processes
        uint32_t NumofProcesses;
        fread(&NumofProcesses, sizeof(uint32_t), 1, fp);

        //find the number of expected uint32s in the file
        long NumOfUint32Expected= 1 + 3 * (long)NumofProcesses;

        //Create array of pcbs to be put in dynamic array
        ProcessControlBlock_t * processes = (ProcessControlBlock_t*)malloc(sizeof(ProcessControlBlock_t)*NumofProcesses);

        int i;
        //check if file is still open and all parameters for each process are included in the file
        if (fp != NULL && (NumofUint32Actual == NumOfUint32Expected) ) 
        {
            for (i = 0; i < (int)NumofProcesses; i++)
            {
                fread(&processes[i].remaining_burst_time, sizeof(uint32_t), 1, fp);
                fread(&processes[i].priority, sizeof(uint32_t), 1, fp);
                fread(&processes[i].arrival, sizeof(uint32_t), 1, fp);
                processes[i].started = false;
            }
        }
        else return NULL;

       //Create dynamic array from pcb array
       dyn_array_t * readyqueue = dyn_array_import(processes, (int)NumofProcesses,sizeof(ProcessControlBlock_t),NULL); 
       return readyqueue;
    }
    else return NULL;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    /*
    if(ready_queue == NULL || result == NULL)
    {
        return false;
    }
    bool flag = true;
    uint32_t t = 0;
    int numJobs = 0;
    int queue_Size = dyn_array_capacity(ready_queue);
    //printf("%d", queue_Size);
    if(queue_Size > (int)sizeof(ProcessControlBlock_t)) qsort(ready_queue, queue_Size, sizeof(ProcessControlBlock_t), arrival_sort);
    do {
        ProcessControlBlock_t *head = dyn_array_front(ready_queue);
        if(head != NULL)
        {
            int count = 0;
            count = arrived_pcbs(head, t, count);
            while(count <= 0) {
                t++;
                count = arrived_pcbs(head, t, count);
                
            }
            if(count > 0) qsort(head, count, sizeof(ProcessControlBlock_t), time_remaining_sort);
            
            if(!head->started) {
                numJobs++;
                head->started = true;
                head->timeStarted = t;
                result->average_waiting_time += (t - head->arrival);
                virtual_cpu(head);
                t++;
                if(head->remaining_burst_time == 0) {
                    result->average_turnaround_time += (t - head->arrival);
                    flag = dyn_array_pop_front(ready_queue);
                }
            }
            else if(head->started && head->remaining_burst_time > 0) {
                virtual_cpu(head);
                t++;
                if(head->remaining_burst_time == 0) {
                    result->average_turnaround_time += (t - head->arrival);
                    flag = dyn_array_pop_front(ready_queue);
                }
            } else {
                numJobs++;
                result->average_turnaround_time += (t - head->arrival);
                flag = dyn_array_pop_front(ready_queue);
            }
        }
        else if(head == NULL) {
            flag = dyn_array_pop_front(ready_queue);
        }

    } while(flag == true);

    result->average_turnaround_time = (result->average_turnaround_time / numJobs);
    result->average_waiting_time = (result->average_waiting_time / numJobs);
    result->total_run_time = t;
*/
    return true;
}
