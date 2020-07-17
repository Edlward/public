#include "TimedTask.h"


#include<iostream>
#include<string>
#include<memory>
using namespace std;
void EchoFunc(std::string&& s)
{
    std::cout << "test : " << s << endl;
}


#ifdef UNIT_TEST

void func1()
{
    std::cout << "trigger func1" << std::endl;
}

void func2(int x)
{
    std::cout << "trigger func2, x: " << x << std::endl;
}

int Test_TimedTask(int argc, char* argv[])
{
    Timer timer;

    // execute task every timer interval
    std::cout << "--- start period timer ----" << std::endl;
    timer.start(1000, std::bind(func2, 3));
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    timer.stop();
    std::cout << "--- stop period timer ----" << std::endl;

    // execute task once after delay
    std::cout << "--- start one shot timer ----" << std::endl;
    timer.startOnce(1000, func1);
    std::cout << "--- stop one shot timer ----" << std::endl;

    getchar();
    return 0;
}


int Test_TimedTask()
{
    TimedTask t;
    //周期性执行定时任务
    t.StartTimer(1000, std::bind(EchoFunc,"hello world!"));
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "try to expire timer!" << std::endl;
    t.Stop();

    //周期性执行定时任务
    t.StartTimer(1000, std::bind(EchoFunc,  "hello c++11!"));
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "try to expire timer!" << std::endl;
    t.Stop();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    //只执行一次定时任务
    //同步
    t.SyncWait(1000, EchoFunc, "hello world!");
    //异步
    t.AsyncWait(1000, EchoFunc, "hello c++11!");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}

#endif

