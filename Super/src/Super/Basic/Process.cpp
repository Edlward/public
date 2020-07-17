#include "Process.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <pthread.h>
    #include <unistd.h>
#endif


Process::Process(void)
{
}


Process::~Process(void)
{
}

#ifdef _WIN32
void* Process::CurrProcess()
{
    return GetCurrentProcess();
}

int Process::CurrProcessID()
{
        return GetCurrentProcessId();
}

bool Process::setCurrProcessPriority(Priority prior)
{
    int np=prior;
    switch (np)
    {
        case Idle:       np = IDLE_PRIORITY_CLASS; break;
        case Low:
        case Below:    np = BELOW_NORMAL_PRIORITY_CLASS; break;
        case Normal:  np = NORMAL_PRIORITY_CLASS; break;
        case Above:    np = ABOVE_NORMAL_PRIORITY_CLASS; break;
        case High:     np = HIGH_PRIORITY_CLASS; break;
        case Real:      np = REALTIME_PRIORITY_CLASS; break;
        default:return false; // bad priority value
    }
    return SetPriorityClass (GetCurrentProcess(),np)==TRUE;
}

int Process::getCurrProcessPriority()
{
     int cp=GetPriorityClass(GetCurrentProcess());
     switch (cp)
     {
     case IDLE_PRIORITY_CLASS:                        cp=Idle; break;
     //case Low:
     case BELOW_NORMAL_PRIORITY_CLASS:    cp=Below;break;
     case NORMAL_PRIORITY_CLASS:                cp=Normal;break;
     case ABOVE_NORMAL_PRIORITY_CLASS:    cp=Above;break;
     case HIGH_PRIORITY_CLASS:                      cp=High;break;
     case REALTIME_PRIORITY_CLASS:               cp=Real;break;
     default:return Normal;
     }
     return cp;
}

#else

//tobe tested
void* Process::CurrProcess()
{
   //todo:
    return 0;
}

int Process::CurrProcessID()
{
    return getpid();
}

bool Process::setCurrProcessPriority(Priority prior)
{
    const int policy = (prior <= Normal) ? SCHED_OTHER : SCHED_RR;
    const int minp = sched_get_priority_min (policy);
    const int maxp = sched_get_priority_max (policy);
    struct sched_param param;
    switch (prior)
    {
    case Idle:
    case Low:
    case Below:
    case Normal:    param.sched_priority = 0; break;
    case Above:
    case High:      param.sched_priority = minp + (maxp - minp) / 4; break;
    case Real:       param.sched_priority = minp + (3 * (maxp - minp) / 4); break;
    default: return false;
    }
    return pthread_setschedparam(pthread_self(),policy,&param)==0;
}

int Process::getCurrProcessPriority()
{
    //todo
     return 0;
}

#endif
