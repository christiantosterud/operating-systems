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

unsigned int score;
unsigned int total;

/*
* LOAD PROCESS CONTROL BLOCKS TESTS
**/

TEST (load_process_control_blocks, NullInputFileName)
{
    const char *input_filename = NULL;
    dyn_array_t *test = NULL;
    dyn_array_t *res = load_process_control_blocks(input_filename);
    ASSERT_EQ(test, res);
}

TEST (load_process_control_blocks, BadInputFilename)
{
    const char *input_filename = "/n";
    dyn_array_t *test = NULL;
    dyn_array_t *res = load_process_control_blocks(input_filename);
    ASSERT_EQ(test, res);
}

TEST (load_process_control_blocks, NonExistingInputFilename)
{
    const char *input_filename = "nafile.binary";
    dyn_array_t *test = NULL;
    dyn_array_t *res = load_process_control_blocks(input_filename);
    ASSERT_EQ(test, res);
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

TEST (shortest_job_first, goodInput) 
{
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[4] = 
    {
        [0] = {25,2,10,0},
        [1] = {2,3,1,0},
        [2] = {4,1,2,0},
        [3] = {1,4,3,0},
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = shortest_job_first (pcbs,sr);	
    ASSERT_EQ(true,res);
    float answers[3] = {8.5,.5,35};
    ASSERT_EQ(answers[0],sr->average_turnaround_time);
    ASSERT_EQ(answers[1],sr->average_waiting_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=20;
}



TEST (shortest_job_first, goodInput1) 
{
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[4] = 
    {
        [0] = {350,2,121,0},
        [1] = {23,3,11,0},
        [2] = {7,1,60,0},
        [3] = {1,4,8,0},
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = shortest_job_first (pcbs,sr);	
    ASSERT_EQ(true,res);
    float answers[3] = {95.25,0,471};
    ASSERT_EQ(answers[0],sr->average_turnaround_time);
    ASSERT_EQ(answers[1],sr->average_waiting_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=20;
}


TEST (shortest_job_first, goodInput2) 
{
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[16] = 
    {
        [0] = {7,2,1,0},
        [1] = {6,3,11,0},
        [2] = {13,1,6,0},
        [3] = {40,4,8,0},
        [4] = {15,2,39,0},
        [5] = {20,3,41,0},
        [6] = {9,1,56,0},
        [7] = {4,4,3,0},
        [8] = {3,2,20,0},
        [9] = {6,3,11,0},
        [10] = {13,1,62,0},
        [11] = {40,4,81,0},
        [12] = {7,2,14,0},
        [13] = {6,3,19,0},
        [14] = {13,1,2,0},
        [15] = {40,4,1,0},
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[15]);
    dyn_array_push_back(pcbs,&data[14]);
    dyn_array_push_back(pcbs,&data[13]);
    dyn_array_push_back(pcbs,&data[12]);	
    dyn_array_push_back(pcbs,&data[11]);
    dyn_array_push_back(pcbs,&data[10]);
    dyn_array_push_back(pcbs,&data[9]);
    dyn_array_push_back(pcbs,&data[8]);	
    dyn_array_push_back(pcbs,&data[7]);
    dyn_array_push_back(pcbs,&data[6]);
    dyn_array_push_back(pcbs,&data[5]);
    dyn_array_push_back(pcbs,&data[4]);
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);		
    bool res = shortest_job_first (pcbs,sr);	
    ASSERT_EQ(true,res);
    float answers[3] = {56.5,41.375,243};
    ASSERT_EQ(answers[0],sr->average_turnaround_time);
    ASSERT_EQ(answers[1],sr->average_waiting_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=20;
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


TEST (shortest_remaining_time_first, goodInput) 
{
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[4] = 
    {
        [0] = {25,2,4,0},
        [1] = {5,3,3,0},
        [2] = {5,1,2,0},
        [3] = {6,4,1,0},
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = shortest_remaining_time_first (pcbs,sr);	
    ASSERT_EQ(true,res);
    float answers[3] = {17,6.75,41};
    ASSERT_EQ(answers[0],sr->average_turnaround_time);
    ASSERT_EQ(answers[1],sr->average_waiting_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

}








TEST (shortest_remaining_time_first, goodInput2) 
{
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[16] = 
    {
        [0] = {7,2,1,0},
        [1] = {6,3,11,0},
        [2] = {13,1,6,0},
        [3] = {40,4,8,0},
        [4] = {15,2,39,0},
        [5] = {20,3,41,0},
        [6] = {9,1,56,0},
        [7] = {4,4,3,0},
        [8] = {3,2,20,0},
        [9] = {6,3,11,0},
        [10] = {13,1,62,0},
        [11] = {40,4,81,0},
        [12] = {7,2,14,0},
        [13] = {6,3,19,0},
        [14] = {13,1,2,0},
        [15] = {40,4,1,0},
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[15]);
    dyn_array_push_back(pcbs,&data[14]);
    dyn_array_push_back(pcbs,&data[13]);
    dyn_array_push_back(pcbs,&data[12]);	
    dyn_array_push_back(pcbs,&data[11]);
    dyn_array_push_back(pcbs,&data[10]);
    dyn_array_push_back(pcbs,&data[9]);
    dyn_array_push_back(pcbs,&data[8]);	
    dyn_array_push_back(pcbs,&data[7]);
    dyn_array_push_back(pcbs,&data[6]);
    dyn_array_push_back(pcbs,&data[5]);
    dyn_array_push_back(pcbs,&data[4]);
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);		
    bool res = shortest_job_first (pcbs,sr);	
    ASSERT_EQ(true,res);
    float answers[3] = {56.5,41.375,243};
    ASSERT_EQ(answers[0],sr->average_turnaround_time);
    ASSERT_EQ(answers[1],sr->average_waiting_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=20;
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
