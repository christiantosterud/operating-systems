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
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
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

/**
 * 
 */ 
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
        long NumofUint32Actual = filelen/4;
        
        //set file pointer back to beginning of file so it can be read
        rewind(fp);

        //grab first uint32 to find number of processes
        uint32_t NumofProcesses;
        fread(&NumofProcesses, 4, 1, fp);
        

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
                fread(&processes[i].remaining_burst_time, 4, 1, fp);
                fread(&processes[i].priority, 4, 1, fp);
                fread(&processes[i].arrival, 4, 1, fp);
            }

        }

        //Was using to double check uint32 values were being set correctly
        //printf("%" PRIu32 "\n",processes[0].remaining_burst_time); 

        //Create dynamic array from pcb array
        dyn_array_t * readyqueue = dyn_array_import(processes, (int)NumofProcesses,sizeof(ProcessControlBlock_t),NULL); 
        return readyqueue;
    }
    else return NULL;
    
    
    
    // This implementation is just pulling in the data into a uint32 array have it
    // in here for now in case I need to reference it or use. Implementation above may or
    // may not be right
    /*
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

        //find the number of uint32s (4 bytes for every uint32, excluding first uint32)
        long NumofUint32 = filelen-4/4;
        
        //set file pointer back to beginning of file so it can be read
        rewind(fp);

        //grab first uint32 to find number of processes
        uint32_t NumofProcesses;
        fread(&NumofProcesses, 4, 1, fp);
        printf("%" PRIu32 "\n", NumofProcesses);

        //allocate uint32 array and then read file into the array
        uint32_t * buffer;
        buffer = (uint32_t*)malloc (sizeof(uint32_t)*NumofUint32);
        if (fp != NULL) 
        {
           fread(buffer, filelen, 1, fp);
        }
       printf("%" PRIu32 "\n",buffer[0]);  
       }
        */
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
