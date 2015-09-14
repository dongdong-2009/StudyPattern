
#ifndef _TimerQueueDemo_h_
#define _TimerQueueDemo_h_

#include "ace/Timer_Queue.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Wheel.h"
#include "ace/Timer_Hash.h"
#include "ace/Timer_List.h"

#include "AceDemo/Callback.h"
#include "AceDemo/TimerDispatcher.h"

inline int TimerQueueMain(int argc, char *argv[])
{
    ACE_Timer_Queue *timer_queue;

    ACE_NEW_RETURN(timer_queue, ACE_Timer_List, -1);

    // setup the timer queue
    Timer::instance ()->set (timer_queue);

    CB cb[10];
    long args[10];
    for (long i = 0; i < 10 ; i++)
    {
        ACE_Time_Value const timeout (i);
        long timerID = Timer::instance ()->schedule(&cb[i],
            &args[i],
            timer_queue->gettimeofday () + ACE_Time_Value(5),
            timeout);

        // Set the timerID state variable of the handler.
        cb[i].setID (timerID);

        // Implicitly send the handler it's timer id.
        args[i] = timerID;
    }

    // "run" the timer.
    Timer::instance ()->wait_for_event ();
    
    return 0;
}

#endif