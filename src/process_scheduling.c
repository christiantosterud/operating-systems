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

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if(ready_queue == NULL || result == NULL)
    {
        return false;
    }

    time_t begin, end;
    time(&begin);

    float total_Turn = 0;
    int number_Jobs = 0;
// 1. Use of FIFO queue 
    bool flag = true;
    do {
        time_t begin_Turn, end_Turn;
        // Execute the front of the queue
        // 2. Pointer to front/first process of the queue
        void* head = dyn_array_front(ready_queue);
        ProcessControlBlock_t * controlHead = (ProcessControlBlock_t *) head;
        if(controlHead->started != true)
        {
            controlHead->started = true;
            time(&begin_Turn);

            // 3. Run the current process
            
            // 4. Remove the element
            flag = dyn_array_pop_front(ready_queue);

            // 5. Keep track of the wait and turn times
            time(&end_Turn);
            total_Turn += end_Turn - begin_Turn;
            number_Jobs++;
        }
    } while (flag != false);
    
    time(&end);
    long total_Time = end - begin;
    result->average_waiting_time = (total_Time - total_Turn)/number_Jobs;  // wait time = time between arrival of the process and the first time the process is scheduled to run on the CPU
    result->average_turnaround_time = total_Turn/number_Jobs; // turnaround time = time a process takes to complete
    result->total_run_time = end - begin; //total run time = total time for completion

    return true;

    
    // for(i = 0; i < (ready_queue->size); i++)
    // {
    //     void* head = dyn_array_front(ready_queue);
    //     ready_queue->size;
    // }
    // UNUSED(ready_queue);
    // UNUSED(result);
    // return false;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
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
        //check if file opened successfully
        if (fp == NULL) return NULL;
        
        //find the size in bytes of the file
        fseek(fp, 0 , SEEK_END);
        long filelen = ftell(fp);

        //find the number of total uint32s in the file
        long NumofUint32Actual = filelen/sizeof(uint32_t);
        
        //set file pointer back to beginning of file so it can be read
        rewind(fp);

        //grab first uint32 to find number of processes
        uint32_t NumofProcesses = fread(&NumofProcesses, sizeof(uint32_t), 1, fp);

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
            }
        }

        //Was using to double check uint32 values were being set correctly
        //printf("%" PRIu32 "\n",processes[0].remaining_burst_time); 

        // create dynamic array
        dyn_array_t * dyn_Array_Process = dyn_array_create(NumofUint32Actual, sizeof(uint32_t), NULL);
        // Imports data read from PCB into dynamic array
        // dyn_array_t * readyqueue = dyn_array_import(processes, (int)NumofProcesses,sizeof(ProcessControlBlock_t),dyn_Array_Process);
        dyn_array_t * readyqueue = dyn_array_import(processes, sizeof(uint32_t),sizeof(ProcessControlBlock_t), dyn_Array_Process); 
        return readyqueue;
    }
    else return NULL;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
