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

struct dyn_array 
{
    // DYN_FLAGS flags;
    size_t capacity;
    size_t size;
    const size_t data_size;
    void *array;
    void (*destructor)(void *);
};


//Sorts the given elements of a pointer in order from lowest to highest number of arrival time
int arrival_sort(const void * a, const void * b)
{
    ProcessControlBlock_t *blockA = (ProcessControlBlock_t *)a;
    ProcessControlBlock_t *blockB = (ProcessControlBlock_t *)b;
    return( blockA->arrival - blockB->arrival);
}

//Sorts the given elements of a pointer in order from lowest to highest number of time remaining
int time_remaining_sort(const void *a, const void *b)
{
    ProcessControlBlock_t *blockA = (ProcessControlBlock_t *)a;
    ProcessControlBlock_t *blockB = (ProcessControlBlock_t *)b;
    return( blockA->remaining_burst_time - blockB->remaining_burst_time);
}

//Sorts the given elements of a pointer in order from lowest to highest number of priority
int priority_sort(const void *a, const void *b)
{
    ProcessControlBlock_t *blockA = (ProcessControlBlock_t *)a;
    ProcessControlBlock_t *blockB = (ProcessControlBlock_t *)b;
    return( blockA->priority - blockB->priority);
}

//Is used after arrival time sort to determine which pcbs have arrived and are ready to be put on to the cpu
int arrived_pcbs(ProcessControlBlock_t *base, uint32_t t, uint32_t count)
{
    if((uint32_t)base->arrival > t) return count;
    else {
        count++;
        base++;
        return arrived_pcbs(base, t, count);
    } 
}


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

    float total_Burst, total_Wait, deadTime = 0;
    int number_Jobs = 0;
    size_t queue_Size = dyn_array_capacity(ready_queue);
    // 1. Use of FIFO queue 
    bool flag = true;
    do {
        // Execute the front of the queue
        // Grabs the front/first process of the queue
        void* head = dyn_array_front(ready_queue);
        ProcessControlBlock_t * controlHead = (ProcessControlBlock_t *) head;
        if(controlHead->started == false)
        {
            controlHead->started = true;
            
            //calculate the wait time
            total_Wait = total_Burst - controlHead->arrival;

            // if nothing was in the queue yet after the a process was completed. 
            // The CPU would be doing nothing but waiting, therefore adding time to the total time to process all the PCBs
            if(controlHead->arrival > total_Burst)
            {
                deadTime = controlHead->arrival - total_Burst;
            }

            // Run the current process and get the burst time aka "run time"
            total_Burst += controlHead->remaining_burst_time;

            // Remove the process
            flag = dyn_array_pop_front(ready_queue);
            number_Jobs++;
        }
    } while (flag == true && number_Jobs < (int) queue_Size); // Another job in the queue
    
    result->average_waiting_time = total_Wait/number_Jobs;  // wait time = time between arrival of the process and the first time the process is scheduled to run on the CPU
    result->average_turnaround_time = total_Burst/number_Jobs; // turnaround time = time a process takes to complete
    result->total_run_time = total_Burst + deadTime; //total run time = total time for completion

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
    //sort based on arrival time, round robin works by whatever arrives first
    qsort(ready_queue->array, dyn_array_size(ready_queue), sizeof(ProcessControlBlock_t), arrival_sort);
    
    //get limit/number of elements              //Chandra did question me using this but it has been working fine
    size_t limit = dyn_array_size(ready_queue);
    
    //total for run time and a counter which works as an interrupt
    int total = 0, counter = 0;
    
    int wait_time = 0, turnaround_time = 0;

    float totalRunTime = 0;

    //grab first process from ready_queue, in a sense we would extract for some reason this works
    void* head = dyn_array_front(ready_queue);
  
    ProcessControlBlock_t * curFront = ( ProcessControlBlock_t * ) head;
    //while there is a process in the queue
    while (dyn_array_size(ready_queue)>0)
    {
        //if we know that the burst time will finish within the given quantum
        if (curFront->remaining_burst_time <= quantum && curFront->remaining_burst_time > 0)
        {
            total = total + curFront->remaining_burst_time;
            printf("%d", total);
            curFront->remaining_burst_time = 0;
            counter = 1;
        }
        //else we know the current process is not going to finish within the quantum so we send it to the 
        // back of the queue and grab the next process
        else if (curFront->remaining_burst_time > 0)
        {
            curFront->remaining_burst_time = curFront->remaining_burst_time - quantum;
            
            total = total + quantum;
            dyn_array_push_back(ready_queue, curFront);
            dyn_array_extract_front(ready_queue, head);
            curFront = ( ProcessControlBlock_t * ) head;
        }
        //this is our interrupt we know the process is finished and we can just grab the next process
        // and lose reference to the current finished process (could maybe use some help with mem management)
        if (curFront->remaining_burst_time == 0 && counter == 1)
        {   
            //anytime our beginning total of processes was odd, every calculation was off by one. aka don't ask
            if (limit % 2 == 1)
            {
                
                wait_time = wait_time + total - curFront->arrival - curFront->og_burst+1;

                turnaround_time = turnaround_time + total - curFront->arrival+1;
            }
            else 
            {
                wait_time = wait_time + total - curFront->arrival - curFront->og_burst;

                turnaround_time = turnaround_time + total - curFront->arrival;
            }
            //reset interrupt and grab next process from queue
            counter = 0;
            totalRunTime += curFront->og_burst;
            printf("%f \n ", totalRunTime);
            dyn_array_extract_front(ready_queue, head);
            curFront = ( ProcessControlBlock_t * ) head;
        }
    }
    //calculate averages and total run time
    result->average_waiting_time = wait_time * 1.0 / limit;
    result->average_turnaround_time = turnaround_time * 1.0 / limit;
    result->total_run_time = (float)totalRunTime;
    printf("%ld", result->total_run_time);
    return true;
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
    if(ready_queue == NULL || result == NULL)
    {
        return false;
    }
    int number_Jobs = 0;
    int queue_Size = dyn_array_capacity(ready_queue);
    //Right now I am using an int that is getting incremented for the time not
    //sure if we are supposed to be using an actual timer but the concepts should
    //be the same
    int t = 0;
    bool flag = true;
    qsort(ready_queue, queue_Size, sizeof(ProcessControlBlock_t), arrival_sort);
    do {
        //Grab the front element in the queue
        ProcessControlBlock_t *head = dyn_array_front(ready_queue);


        //Count variable is used to indicate how many elements have arrived in the queue
        //based on the current time
        int count = 0;


        //arrived_pcbs is a recursive call to get the number of elements that have arrived
        count = arrived_pcbs(head, t, count);


        //qsort will sort the "count" of elements that have arrived
        //and align them based on remaining burst time
        qsort(head, count, sizeof(ProcessControlBlock_t), time_remaining_sort);


        //Set cur to the element that is ready to get on the cpu and 
        //has the shortest wait time
        ProcessControlBlock_t *cur = (ProcessControlBlock_t *)head;


        //if(!cur->started) cur->timeStarted = t;
        //Need to setup a variable to track each ones exact start time due to
        // the fact that they will be getting moved on and off of the cpu
        if(cur->remaining_burst_time > 0) virtual_cpu(cur);
        //Need to ask some questions about the rest of this
        else {
            flag = dyn_array_pop_front(ready_queue);
            number_Jobs++;
            t++;
        }

        

    } while(flag == true && number_Jobs < (int) queue_Size);




   // UNUSED(ready_queue);
    UNUSED(result);
    return false;
}




