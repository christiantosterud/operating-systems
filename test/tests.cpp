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

TEST(first_come_first_serve, goodInput){
        ScheduleResult_t *sr = new ScheduleResult_t;
        dyn_array_t* da = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
        memset(sr,0,sizeof(ScheduleResult_t));
        ProcessControlBlock_t times[] = {{4,0,false},{7,0,false},{3,0,false}
        };	
        dyn_array_push_back(da,&times[2]);
        dyn_array_push_back(da,&times[1]);
        dyn_array_push_back(da,&times[0]);
        bool res = first_come_first_serve (da,sr);
        ASSERT_EQ(true,res);  // stop if not

        float answers[3] = {5,9.67,14};
        EXPECT_EQ(answers[0],sr->average_latency_time);
        EXPECT_EQ(answers[1],sr->average_wall_clock_time);
        EXPECT_EQ(answers[2],sr->total_run_time);
        dyn_array_destroy(da);
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

TEST(priority, goodInput){
        ScheduleResult_t *sr = new ScheduleResult_t;
        dyn_array_t* da = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
        memset(sr,0,sizeof(ScheduleResult_t));
        ProcessControlBlock_t times[] = {{4,1,false},{7,2,false},{3,3,false}
        };	
        dyn_array_push_back(da,&times[2]);
        dyn_array_push_back(da,&times[1]);
        dyn_array_push_back(da,&times[0]);
        bool res = priority (da,sr);
        ASSERT_EQ(true,res);  // stop if not

        float answers[3] = {4.33,9,14};
        EXPECT_EQ(answers[0],sr->average_latency_time);
        EXPECT_EQ(answers[1],sr->average_wall_clock_time);
        EXPECT_EQ(answers[2],sr->total_run_time);
        dyn_array_destroy(da);
        delete sr;
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

TEST(round_robin , zeroQuantum){
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
