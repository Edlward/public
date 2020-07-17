#pragma once

#ifndef TimedTask_H_
#define TimedTask_H_

//简单定时后台任务


#include<functional>
#include<chrono>
#include<thread>
#include<atomic>
#include<memory>
#include<mutex>
#include<condition_variable>

class TimedTask{
public:
    TimedTask() :expired_(true), try_to_expire_(false)
    {

    }
    TimedTask(const TimedTask& t)
    {
        expired_ = t.expired_.load();
        try_to_expire_ = t.try_to_expire_.load();
    }
    ~TimedTask()
    {
        Stop();
    }

    void StartTimer(int interval, std::function<void()> task)
    {
        // is started, do not start again
        if (expired_ == false)
        {
            return;
        }


        // start async timer, launch thread and wait in that thread
        expired_ = false;
        std::thread([this, interval, task]()
        {
            while (!try_to_expire_)
            {
                // sleep every interval and do the task again and again until times up
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                task();
            }
            //std::cout << "stop task..." << std::endl;
            {
                // timer be stopped, update the condition variable expired and wake main thread
                std::lock_guard<std::mutex> locker(mutex_);
                expired_ = true;
                expired_cond_.notify_one();
            }
        }).detach();
    }

    void Stop()
    {
        // do not stop again
        if (expired_)
        {
            return;
        }
        if (try_to_expire_){
            //          std::cout << "timer is trying to expire, please wait..." << std::endl;
            return;
        }
        try_to_expire_ = true;
        {
            std::unique_lock<std::mutex> locker(mutex_);
            expired_cond_.wait(locker, [this]{return expired_ == true; });
            if (expired_ == true){
                //              std::cout << "timer expired!" << std::endl;
                try_to_expire_ = false;
            }
        }
    }

    void startOnce(int delay, std::function<void()> task)
    {
        std::thread([delay, task]() 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            task();
        }).detach();
    }

    //编译不通过，待续
#if 0
    template<typename callable, class ...arguments>
    void SyncWait(int after, callable&& f, arguments&&... args){

        std::function<typename std::result_of<callable(arguments...)>::type()> task
            (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
        std::this_thread::sleep_for(std::chrono::milliseconds(after));
        task();
    }
    template<typename callable, class... arguments>
    void AsyncWait(int after, callable&& f, arguments&&... args){
        std::function<typename std::result_of<callable(arguments...)>::type()> task
            (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

        std::thread([after, task](){
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }).detach();
    }
#endif

private:
    std::atomic<bool> expired_;
    std::atomic<bool> try_to_expire_;
    std::mutex mutex_;
    std::condition_variable expired_cond_;
};





#ifdef UNIT_TEST
int Test_TimedTask();
#endif


#endif

