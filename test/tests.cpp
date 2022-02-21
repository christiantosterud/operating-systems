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
    ScheduleResult_t *result = NULL;
    size_t quantum = 5;
    bool res = round_robin(readyqueue, result, quantum);
    EXPECT_EQ(false, res);
}

/*
* SHORTEST TIME REMAINING TESTS
**/

TEST (shortest_remaining_time_first, NullQueue) 
{
    dyn_array_t *readyqueue = NULL;
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
