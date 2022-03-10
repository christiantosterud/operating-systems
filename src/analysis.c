#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#include "process_scheduling.c"
#include "dyn_array.c"

#define FCFS "FCFS"
#define RR "RR"
#define SJF "SJF"
#define SRTF "SRTF"

#define UNUSED(x) (void)(x)

//Method that prints all the scheduling resuts
//passes in the ready queue with all of our processes
//the specified scheduling algorithm to print results for
//the quantum (if round robin is chosen)
//the file pointer to the readme
void print_scheduling_results( dyn_array_t *ready_queue, char* scheduling_algo, int quantum, FILE *readme )
{
    //the result for the scheduling algorithm called
    ScheduleResult_t *result = (ScheduleResult_t *)malloc( sizeof( ScheduleResult_t ) * 1 );
    
    // CALL DESRIED SCHEDULING ALGORITHM AND WRITE TO READ ME
    if ( strcmp( scheduling_algo, FCFS ) == 0 )
    {
        //printf("FCFS");
        if ( first_come_first_serve( ready_queue, result ) )
        {
            // WRITE TO README
            fprintf(readme, "First Come First Serve Results:\n");
            fprintf(readme, "   Average Waiting Time: %f \n", result->average_waiting_time );
            fprintf(readme, "   Average Turn Around Time: %f \n", result->average_turnaround_time);
            fprintf(readme, "   Total Clock Time: %ld \n", result->total_run_time);
        }
        else
        {
            fprintf(readme, "First Come First Serve Failed\n");
            return;
        }
    }
    else if ( strcmp( scheduling_algo, RR ) == 0 )
    {
        //printf("RR");
        if ( round_robin( ready_queue, result, quantum) )
        {
            //WRITE TO README
            fprintf(readme, "Round Robin Results:\n");
            fprintf(readme, "   Average Waiting Time: %f \n", result->average_waiting_time );
            fprintf(readme, "   Average Turn Around Time: %f \n", result->average_turnaround_time);
            fprintf(readme, "   Total Clock Time: %ld \n", result->total_run_time);
        }
        else
        {
            fprintf(readme, "Round Robin Failed\n");
            return;
        }
    }
    else if ( strcmp( scheduling_algo, SJF ) == 0 )
    {
        //printf("SJF");
        if( shortest_job_first( ready_queue, result ) )
        {
            //WRITE TO README
            fprintf(readme, "Shortest Job First Results:\n");
            fprintf(readme, "   Average Waiting Time: %f \n", result->average_waiting_time );
            fprintf(readme, "   Average Turn Around Time: %f \n", result->average_turnaround_time);
            fprintf(readme, "   Total Clock Time: %ld \n", result->total_run_time);
        }
        else
        {
            fprintf(readme, "Shortest Job First Failed\n");
            return;
        }
    }
    else if ( strcmp( scheduling_algo, SRTF ) == 0 )
    {
        //printf("STR");
        if ( shortest_remaining_time_first( ready_queue, result ) )
        {
            //WRITE TO README
            fprintf(readme, "Shortest Time Remaining Results:\n");
            fprintf(readme, "   Average Waiting Time: %f \n", result->average_waiting_time );
            fprintf(readme, "   Average Turn Around Time: %f \n", result->average_turnaround_time);
            fprintf(readme, "   Total Clock Time: %ld \n", result->total_run_time);
        }
        else
        {
            fprintf(readme, "Shortest Time Remaining Failed\n");
            return;
        }
    }
    else
    {
        //if the user enters anything other than a valid scheduling algorithm
        printf("Please enter a valid scheduling algorithm...");
    }
    //close file upon completion of writing results
    fclose( readme );
}


// Add and comment your analysis code in this function.
// THIS IS NOT FINISHED.
int main(int argc, char **argv) 
{
    //if there are less than 3 command line arguments, display the correct inputs
    if (argc < 3) 
    {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    //file pointers for the input pcb.bin and the readme file
    FILE *fp, *readme;
    //ready queue to hold all of the processes
    dyn_array_t *ready_queue;
    //quantum taken from the third command line argument
    int quantum = (intptr_t)argv[3];

    // open source file in read mode
    // if error
    // then, exit
    if( ( fp = fopen(argv[1], "r") ) == NULL ) 
    {
        printf("Error...\nCannot open file: %s\n", argv[1]);
        printf("Either the filename is incorrect or it does not exists.\n");
        return -1;
    }
    else if ( ( fp = fopen(argv[1], "r") ) != NULL )
    {
        //if the file pointer is not null load the process control blocks for the specified pcb.bin
        ready_queue = load_process_control_blocks( argv[1] );
    }

    //open the readme file for appending
    readme = fopen("../readme.md", "a+");

    //call method that prints scheduling results to the readme
    print_scheduling_results( ready_queue, argv[2], quantum, readme );

    //close the file and destroy the dynamic array
    fclose( fp );
    dyn_array_destroy( ready_queue );

    return EXIT_SUCCESS;
}