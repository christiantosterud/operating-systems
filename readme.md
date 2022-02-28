# Assignment 2

## Edited by: Trevor Prodziniski, Jack Seiler, Kevin Sutcliffe, and Chris Tosterud

Milestone 0: CMake and Unit Testing filled out.

Milestone 1: PCB file loading and First Come First Served.

Milestone 2: Shortest Job First, Shortest Remaining Time First, Round Robin, and analysis of algorithms.

Note:
You can manually copy the time analysis from console and paste it to this readme file, but directly output from your program is strongly recommended.

---

## Add your scheduling algorithm analysis below this line in a readable format.

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
    