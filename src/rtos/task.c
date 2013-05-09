#include "stddefs.h"
#include "task.h"

/******* TEMP *******/
UI16_t RtosGetTime();
UI16_t RtosTimestamp = 0;
UI16_t RtosGetTime()
{
    return RtosTimestamp;
}


void RtosKernelRestoreTask(RtosTask_t* task);
void RtosKernelStoreTask(RtosTask_t* task);


static RtosTask_t RtosTaskIdleObj;
static UI08_t RtosTaskIdleStk[64];

static RtosTask_t* RtosActiveTask;
volatile UI08_t* RtosKernelStackPos;



void RtosTaskIdleFnc()
{
    while(1);
}

void RtosTaskInit()
{
    RtosTaskCreate(&RtosTaskIdleObj, "Idle", RtosTaskIdleFnc, 0, RtosTaskIdleStk, sizeof(RtosTaskIdleStk));
}

void RtosTaskCreate(RtosTask_t* task, char* name, void* function, UI08_t priority, UI08_t* stack, UI16_t stackSize)
{
    if (task == &RtosTaskIdleObj)
    {
        // Initialize linked-list for first element.
        task->list = NULL;
    }
    else
    {
        // Initialize linked-list for next element
        task->list = RtosTaskIdleObj.list;
        RtosTaskIdleObj.list = (struct RtosTask_t*) task;
    }

#ifdef RTOS_DEBUG
    task->name = name;
    task->stackMaxUsage = 0;
    task->stackUsage = 0;
    task->timeRan = 0;
    task->lastRun = 0;
    task->stackSize = stackSize;

    // Fill stack with test pattern so we chan stack-growth
    memset(stack, 0x55, stackSize);
#endif
    task->method = function;
    task->nextRun = 0;
    task->priority = priority;
    task->stack = stack;
    task->state = TASK_STATE_READY;

    RtosKernelPortInitStack(task);
    
}
void RtosTaskDelay(RtosTime_t time)
{
    RtosActiveTask->nextRun = RtosGetTime() + time;

    RtosKernelContextSuspend();
}
void RtosTaskWake(RtosTask_t* task)
{
    RtosActiveTask->nextRun = 0;
}

void RtosTaskRun()
{
    RtosKernelRestoreTask(&RtosTaskIdleObj);
    RtosKernelPortTimerStart();
    RtosKernelContextStart();
}

void RtosTaskChange()
{
    RtosKernelStoreTask(RtosActiveTask);

    RtosTimestamp ++;

    RtosTask_t* next = &RtosTaskIdleObj;
    RtosTask_t* t = &RtosTaskIdleObj;
    while (t != NULL)
    {
        if (t->nextRun  <= RtosGetTime() &&
            t->priority >= next->priority)
            next = t;
        t = t->list;
    }
    
    RtosKernelRestoreTask(next);
}

// Location helper methods:
void RtosKernelRestoreTask(RtosTask_t* task)
{
    RtosActiveTask = task;
    RtosKernelStackPos = task->stackPosition;
    task->state = TASK_STATE_RUNNING;
}

void RtosKernelStoreTask(RtosTask_t* task)
{
    task->stackPosition = RtosKernelStackPos;
    task->state = TASK_STATE_READY;
#ifdef RTOS_DEBUG
    task->lastRun = RtosGetTime();
    //TODO: Analayze stack
#endif
}