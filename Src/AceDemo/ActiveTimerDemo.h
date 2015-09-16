#ifndef _ActiveTimerDemo_h_
#define _ActiveTimerDemo_h_

#include "ace/Timer_Queue_Adapters.h"
#include "ace/Timer_Heap.h"

typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> ActiveTimer;

class ActiveCb : public ACE_Event_Handler
{
public:
  ActiveCb (int id) : id_(id) { }

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg)
  {
    ACE_TRACE (ACE_TEXT ("CB::handle_timeout"));

    const int *val = (const int*)arg;
    ACE_ASSERT((*val) == id_);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Expiry handled by thread %t\n")));
    return 0;
  }
private:
  int id_;
};


inline int ActiveTimerMain(int argc, char *argv[])
{     
    // Create an "active" timer and start its thread.
    ActiveTimer atimer;
    atimer.activate ();

    ActiveCb cb1(1);
    ActiveCb cb2(2);
    int arg1 = 1;
    int arg2 = 2;

    // Schedule timers to go off 3 & 4 seconds from now
    // and then with an interval of 1.1 seconds.
    const ACE_Time_Value curr_tv = ACE_OS::gettimeofday ();
    ACE_Time_Value interval = ACE_Time_Value (1, 1000);

    long tid1 = atimer.schedule (&cb1,
        &arg1,
        curr_tv + ACE_Time_Value (3L),
        interval);
    long tid2 = atimer.schedule (&cb2,
        &arg2,
        curr_tv + ACE_Time_Value (4L),
        interval);

    ACE_Thread_Manager::instance ()->wait ();  // Wait forever.
    
    return 0;
}

#endif