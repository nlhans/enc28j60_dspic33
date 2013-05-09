#ifndef TASK_H
#define TASK_H
//#define RTOS_DEBUG

#include "stddefs.h"
#include "rtos_time.h"

typedef enum RtosTaskState_e
{
    TASK_STATE_SUSPENDED,
    TASK_STATE_RUNNING,
    TASK_STATE_DELAY,
    TASK_STATE_EVENT,
    TASK_STATE_READY
} RtosTaskState_t;

typedef struct RtosTask_s
{
    struct RtosTask_t* list;
    void* method;
    UI08_t* stack;
    UI08_t* stackPosition;
    UI08_t priority;

    RtosTaskState_t state;

    //Time_t lastRun;
    RtosTime_t nextRun;

#ifdef RTOS_DEBUG
    RtosTime_t lastRun;
    RtosTime_t timeRan;
    UI16_t stackUsage;
    UI16_t stackMaxUsage;
    char* name;
#endif

} RtosTask_t;

void RtosTaskInit();
void RtosTaskCreate(RtosTask_t* task, char* name, void* function, UI08_t priority, UI08_t* stack, UI16_t stackSize);
void RtosTaskDelay(RtosTime_t time);
void RtosTaskWake(RtosTask_t* task);

void RtosTaskRun();

void RtosTaskTick();
void RtosTaskChange();

extern void RtosKernelPortInitStack(RtosTask_t* task);
extern void RtosKernelPortTimerStart();
extern void RtosKernelPortTimerClear();
extern inline void RtosKernelContextStart();
extern inline void RtosKernelContextSuspend();

#endif