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
    if (input_file){
        fp = fopen(input_file, "rb");
        if (fp == NULL) return NULL;
        fseek(fp, 0 , SEEK_END);
        long filelen = ftell(fp);
        printf("%ld", filelen);
        filelen = filelen/4;
        rewind(fp);
        uint32_t * buffer;
        buffer = (uint32_t*)malloc (sizeof(uint32_t)*13);
        if (fp != NULL) 
        {
           fread(&buffer, filelen, 1, fp);
        }
       
       /* int reader = open(input_file, O_RDONLY);
        //check it didn't return -1 on failure to open file
        if (reader != -1)
        {
            //fstat informs the file size in bytes and stores the data in the struct stat
            struct stat reader_stored;
           long length =  lseek(reader, 0 , SEEK_END);
           //long filelen = ftell(reader);

            stat(input_file, &reader_stored);

           // uint32_t * buffer = (uint32_t*)malloc (sizeof(uint32_t)*13);
           
            //long filelen2 = sizeof(reader_stored);
            //long filelen = reader_stored.st_size;
            //read(reader,buffer,52);
            printf("%li", length);
           // printf("%d", buffer[2]);
         */   
       }
        
       
   //}
    return NULL;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
