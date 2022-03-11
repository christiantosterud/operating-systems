#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"

// Using a C library requires extern "C" to prevent function managling
extern "C" 
{
#include <dyn_array.h>
}



#define NUM_PCB 30
#define QUANTUM 5 // Used for Robin Round for process as the run time limit

#define QUANTUM1 4 // Used for Robin Round for process as the run time limit
#define QUANTUM2 5 // Used for Robin Round for process as the run time limit

unsigned int score;
unsigned int total;

/*
* LOAD PROCESS CONTROL BLOCKS TESTS
**/

TEST (load_process_control_blocks, NullFilePath) {
    dyn_array_t *da = load_process_control_blocks(NULL);
    ASSERT_EQ(da, (dyn_array_t*) NULL);
}

TEST (load_process_control_blocks, TrickyBadFileName) {
    const char *filename = "";
    dyn_array_t *da = load_process_control_blocks(filename);
    ASSERT_EQ(da, (dyn_array_t*) NULL);
}

TEST (load_process_control_blocks, TrickyBadFileNameNewLine) {
    const char *filename = "\n";
    dyn_array_t *da = load_process_control_blocks(filename);
    ASSERT_EQ(da, (dyn_array_t*) NULL);
}

TEST (load_process_control_blocks, EmptyFile) {
    const char *filename = "JIMRTESTANSWERS.JK"; 
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
    close(fd);
    dyn_array_t *da = load_process_control_blocks(filename);
    ASSERT_EQ(da, (dyn_array_t*) NULL);
}

TEST (load_process_control_blocks, nullFilePath) 
{
    dyn_array_t* da = load_process_control_blocks (NULL);
    ASSERT_EQ(da,(dyn_array_t*) NULL);

    score+=5;
}

TEST (load_process_control_blocks, notFoundFile) 
{
    dyn_array_t* da = load_process_control_blocks ("NotARealFile.Awesome");
    ASSERT_EQ(da,(dyn_array_t*)NULL);

    score+=5;
}

TEST (load_process_control_blocks, emptyFoundFile) 
{
    const char* fname = "EMPTYFILE.DARN";
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    int fd = open(fname, flags, mode);
    close(fd);
    dyn_array_t* da = load_process_control_blocks (fname);
    ASSERT_EQ(da,(dyn_array_t*)NULL);

    score+=5;
}

TEST (load_process_control_blocks, incorrectPCBFoundFile) 
{
    const char* fname = "CANYOUHANDLETHE.TRUTH";
    uint32_t pcb_num = 10;
    uint32_t pcbs[5][3] = {{1,1,1},{2,2,2},{3,3,3},{4,4,4},{5,5,5}};
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    int fd = open(fname, flags, mode);
    write(fd,&pcb_num,sizeof(uint32_t));
    write(fd,pcbs,5 * sizeof(uint32_t)*3);
    close(fd);
    dyn_array_t* da = load_process_control_blocks (fname);
    ASSERT_EQ(da,(dyn_array_t*)NULL);

    score+=5;
}

TEST (load_process_control_blocks, fullFoundFile) 
{
    const char* fname = "PCBs.bin";
    uint32_t pcb_num = NUM_PCB;
    uint32_t pcbs[NUM_PCB][3];
    for (uint32_t p = 0; p < pcb_num; ++p) 
    {
        pcbs[p][0] = rand() % 35 + 1;
        //        printf("%d, ", pcbs[p][0]);
        pcbs[p][1] = p;
        pcbs[p][2] = p;
    }
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    int fd = open(fname, flags, mode);
    write(fd,&pcb_num,sizeof(uint32_t));
    write(fd,pcbs,pcb_num * sizeof(uint32_t)*3);
    close(fd);
    dyn_array_t* da = load_process_control_blocks (fname);
    ASSERT_NE(da, (dyn_array_t*) NULL);
    for (size_t i = 0; i < dyn_array_size(da); ++i) 
    {
        ProcessControlBlock_t * pPCB = (ProcessControlBlock_t *)dyn_array_at(da, i);
        ASSERT_EQ(pPCB->remaining_burst_time, pcbs[i][0]);
        ASSERT_EQ(pPCB->priority, pcbs[i][1]);
        ASSERT_EQ(pPCB->arrival, pcbs[i][2]);
    }
    dyn_array_destroy(da);

    score+=10;
}


/*
* FIRST COME FIRST SERVE TESTS
**/





TEST (first_come_first_serve, NullQueue) 
{
    dyn_array_t *readyqueue = NULL;
    ScheduleResult_t *result = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    bool res = first_come_first_serve(readyqueue, result);
    EXPECT_EQ(false, res);
}

TEST (first_come_first_serve, NullResult)
{
    dyn_array_t *readyqueue = (dyn_array_t*)malloc(sizeof(ScheduleResult_t));
    ScheduleResult_t *result = NULL;
    bool res = first_come_first_serve(readyqueue, result);
    EXPECT_EQ(false, res);
}

TEST (first_come_first_serve, goodInputA) 
{
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[1] = 
    {
        [0] = {5,2,0,0}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = first_come_first_serve (pcbs,sr);	
    ASSERT_EQ(true,res);
    float answers[3] = {5,0,5};
    ASSERT_EQ(answers[0],sr->average_turnaround_time);
    ASSERT_EQ(answers[1],sr->average_waiting_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;
}

TEST (first_come_first_serve, goodInputB) 
{
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[3] = 
    {
        [0] = {24,2,0,0},
        [1] = {3,3,0,0},
        [2] = {3,1,0,0}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[0]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[2]);	
    bool res = first_come_first_serve (pcbs,sr);	
    ASSERT_EQ(true,res);
    float answers[3] = {27,17,30};
    ASSERT_EQ(answers[0],sr->average_turnaround_time);
    ASSERT_EQ(answers[1],sr->average_waiting_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;
}

TEST (first_come_first_serve, goodInputC) 
{
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[4] = 
    {
        [0] = {6,3,0,0},
        [1] = {8,2,0,0},
        [2] = {7,4,0,0},
        [3] = {3,1,0,0},
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[0]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[2]);		
    dyn_array_push_back(pcbs,&data[3]);	
    bool res = first_come_first_serve (pcbs,sr);	
    ASSERT_EQ(true,res);
    float answers[3] = {16.25,10.25,24};
    ASSERT_EQ(answers[0],sr->average_turnaround_time);
    ASSERT_EQ(answers[1],sr->average_waiting_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;
}


/* 
* SHORTEST JOB FIRST TESTS
**/

TEST (shortest_job_first, NullQueue) 
{
    dyn_array_t *readyqueue = NULL;
    ScheduleResult_t *result = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    bool res = shortest_job_first(readyqueue, result);
    EXPECT_EQ(false, res);
}

TEST (shortest_job_first, NullResult)
{
    dyn_array_t *readyqueue = (dyn_array_t*)malloc(sizeof(ScheduleResult_t));
    ScheduleResult_t *result = NULL;
    bool res = shortest_job_first(readyqueue, result);
    EXPECT_EQ(false, res);
}

/* 
* PRIORITY TESTS
**/

TEST (priority, NullQueue) 
{
    dyn_array_t *readyqueue = NULL;
    ScheduleResult_t *result = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    bool res = priority(readyqueue, result);
    EXPECT_EQ(false, res);
}

TEST (priority, NullResult)
{
    dyn_array_t *readyqueue = (dyn_array_t*)malloc(sizeof(ScheduleResult_t));
    ScheduleResult_t *result = NULL;
    bool res = priority(readyqueue, result);
    EXPECT_EQ(false, res);
}

// TEST(priority, goodInput){
//         ScheduleResult_t *sr = new ScheduleResult_t;
//         dyn_array_t* da = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
//         memset(sr,0,sizeof(ScheduleResult_t));
//         ProcessControlBlock_t times[] = {{4,1,false},{7,2,false},{3,3,false}
//         };	
//         dyn_array_push_back(da,&times[2]);
//         dyn_array_push_back(da,&times[1]);
//         dyn_array_push_back(da,&times[0]);
//         bool res = priority (da,sr);
//         ASSERT_EQ(true,res);  // stop if not

//         float answers[3] = {4.33,9,14};
//         EXPECT_EQ(answers[0],sr->average_latency_time);
//         EXPECT_EQ(answers[1],sr->average_wall_clock_time);
//         EXPECT_EQ(answers[2],sr->total_run_time);
//         dyn_array_destroy(da);
//         delete sr;
// }

/*
* ROUND ROBIN TESTS
**/

TEST (round_robin, NullQueue) 
{
    dyn_array_t *readyqueue = NULL;
    ScheduleResult_t *result = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    bool res = round_robin(readyqueue, result, QUANTUM);
    EXPECT_EQ(false, res);
}

TEST (round_robin, NullResult)
{
    dyn_array_t *readyqueue = (dyn_array_t*)malloc(sizeof(ScheduleResult_t));
    ScheduleResult_t *result = NULL;
    bool res = round_robin(readyqueue, result, QUANTUM);
    EXPECT_EQ(false, res);
}

TEST(round_robin , ZeroQuantum){
    dyn_array_t* da = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    ScheduleResult_t* sr = new ScheduleResult_t;
    size_t q = 0;
    bool res = round_robin(da,sr,q);
    EXPECT_EQ(false,res);
    dyn_array_destroy(da);
    delete sr;
}

TEST (round_robin, ourTest) 
{
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[4] = 
    {
        [0] = {15,0,0,0,15},
        [1] = {10,0,1,0,10},
        [2] = {5,0,2,0,5},
        [3] = {20,0,3,0,20}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);     
    dyn_array_push_back(pcbs,&data[0]); 
    bool res = round_robin (pcbs,sr,4);  
    ASSERT_EQ(true,res);
    float answers[3] = {36.5,24,50};
    ASSERT_FLOAT_EQ(answers[0],sr->average_turnaround_time);
    ASSERT_EQ(answers[1],sr->average_waiting_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=20;
} 

TEST (round_robin, Test3Processes) 
{
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[3] = 
    {
        [0] = {10,0,1,0,10},
        [1] = {5,0,2,0,5},
        [2] = {20,0,3,0,20}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);     
    dyn_array_push_back(pcbs,&data[0]); 
    bool res = round_robin (pcbs,sr,4);  
    ASSERT_EQ(true,res);
    float answers[3] = {24,12.333333,35};
    ASSERT_FLOAT_EQ(answers[0],sr->average_turnaround_time);
    ASSERT_EQ(answers[1],sr->average_waiting_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=20;
} 

TEST (round_robin, Test5Processes) 
{
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[5] = 
    {
        [0] = {10,0,1,0,10},
        [1] = {5,0,2,0,5},
        [2] = {20,0,3,0,20},
        [3] = {24,0,4,0,24},
        [4] = {3,0,5,0,3}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[4]);
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);     
    dyn_array_push_back(pcbs,&data[0]); 
    bool res = round_robin (pcbs,sr,4);  
    ASSERT_EQ(true,res);
    float answers[3] = {36.6,24.2,62};
    ASSERT_FLOAT_EQ(answers[0],sr->average_turnaround_time);
    ASSERT_EQ(answers[1],sr->average_waiting_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=20;
} 


/*
* SHORTEST TIME REMAINING TESTS
**/

TEST (shortest_remaining_time_first, NullQueue) 
{
    dyn_array_t *readyqueue = NULL;
    ScheduleResult_t *result = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    bool res = shortest_remaining_time_first(readyqueue, result);
    EXPECT_EQ(false, res);
}

TEST (shortest_remaining_time_first, NullResult)
{
    dyn_array_t *readyqueue = (dyn_array_t*)malloc(sizeof(ScheduleResult_t));
    ScheduleResult_t *result = NULL;
    bool res = shortest_remaining_time_first(readyqueue, result);
    EXPECT_EQ(false, res);
}

class GradeEnvironment : public testing::Environment 
{
    public:
        virtual void SetUp() 
        {
            score = 0;
            total = 210;
        }

        virtual void TearDown()
        {
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", total);
            std::cout << "SCORE: " << score << '/' << total << std::endl;
        }
};



int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}
