#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) {
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) {
    // You need to remove this when you begin implementation.
    abort();  // replace me with implementation.
    return false;
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) {
    // You need to remove this when you begin implementation.
   abort();  // replace me with implementation.
   return false;   
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) {
    // You need to remove this when you begin implementation.
    abort();  // replace me with implementation.
    return false;
}

dyn_array_t *load_process_control_blocks(const char *input_file) {
    // You need to remove this when you begin implementation.
    abort();  // replace me with implementation.
    return NULL;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) {
    // You need to remove this when you begin implementation.
    abort();  // replace me with implementation.
    return false;
}
