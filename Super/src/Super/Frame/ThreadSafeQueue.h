#ifndef THREAD_POOL_H
#define THREAD_POOL_H


#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>



//to be tested
template <class T>
class ThreadSafeQueue
{
protected:
    // Data
    std::queue<T> _queue;
    typename std::queue<T>::size_type _size_max;

    // Thread gubbins
    std::mutex _mutex;
    std::condition_variable _fullQue;
    std::condition_variable _empty;

    // Exit
    // 原子操作
    std::atomic_bool _quit; //{ false };
    std::atomic_bool _finished; // { false };

public:
    ThreadSafeQueue(const size_t size_max) :_size_max(size_max)
    {
        _quit = ATOMIC_VAR_INIT(false);
        _finished = ATOMIC_VAR_INIT(false);
    }
    bool push(T& data)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while (!_quit && !_finished)
        {
            if (_queue.size() < _size_max)
            {
                _queue.push(std::move(data));
                //_queue.push(data);
                _empty.notify_all();
                return true;
            }
            else
            {
                // wait的时候自动释放锁，如果wait到了会获取锁
                _fullQue.wait(lock);
            }
        }

        return false;
    }


    bool pop(T &data)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while (!_quit)
        {
            if (!_queue.empty())
            {
                //data = std::move(_queue.front());
                data = _queue.front();
                _queue.pop();

                _fullQue.notify_all();
                return true;
            }
            else if (_queue.empty() && _finished)
            {
                return false;
            }
            else
            {
                _empty.wait(lock);
            }
        }
        return false;
    }

    // The queue has finished accepting input
    void finished()
    {
        _finished = true;
        _empty.notify_all();
    }

    void quit()
    {
        _quit = true;
        _empty.notify_all();
        _fullQue.notify_all();
    }

    int length()
    {
        return static_cast<int>(_queue.size());
    }
};



//vs2012 not support initializer_list
#if 0

//版权声明：本文为博主原创文章，转载请注明源地址。          https://blog.csdn.net/10km/article/details/52067929        
//
//C++中的模板std::queue提供了一个队列容器,但这个容器并不是线程安全的，如果在多线程环境下使用队列，它是不能直接拿来用的。 
//    基于它做一个线程安全的队列也并不复杂。基本的原理就是用std::mutext信号量对std::queue进行访问控制，以保证任何一个线程都是独占式访问，下面是完整的代码。

/*
 * threadsafe_queue.h
 *
 *  Created on: 2016年7月26日
 *      Author: guyadong
 */

 
#include <queue>
#include <mutex>
#include <condition_variable>
#include <initializer_list>
 
/*
 * 线程安全队列
 * T为队列元素类型
 * 因为有std::mutex和std::condition_variable类成员,所以此类不支持复制构造函数也不支持赋值操作符(=)
 * */
template<typename T>
class threadsafe_queue{
private:
    // data_queue访问信号量
    mutable std::mutex mut;
    mutable std::condition_variable data_cond;
    using queue_type = std::queue<T>;
    queue_type data_queue;
public:
    using value_type= typename queue_type::value_type;
    using container_type = typename queue_type::container_type;
    threadsafe_queue()=default;
    threadsafe_queue(const threadsafe_queue&)=delete;
    threadsafe_queue& operator=(const threadsafe_queue&)=delete;


    /*
     * 使用迭代器为参数的构造函数,适用所有容器对象
     * */
    template<typename _InputIterator>
    threadsafe_queue(_InputIterator first, _InputIterator last){
        for(auto itor=first;itor!=last;++itor){
            data_queue.push(*itor);
        }
    }

    explicit threadsafe_queue(const container_type &c):data_queue(c){}
    /*
     * 使用初始化列表为参数的构造函数
     * */
    threadsafe_queue(std::initializer_list<value_type> list):threadsafe_queue(list.begin(),list.end()){
    }
    /*
     * 将元素加入队列
     * */
    void push(const value_type &new_value){
        std::lock_guard<std::mutex>lk(mut);
        data_queue.push(std::move(new_value));
        data_cond.notify_one();
    }
    /*
     * 从队列中弹出一个元素,如果队列为空就阻塞
     * */
    value_type wait_and_pop(){
        std::unique_lock<std::mutex>lk(mut);
        data_cond.wait(lk,[this]{return !this->data_queue.empty();});
        auto value=std::move(data_queue.front());
        data_queue.pop();
        return value;
    }
    /*
     * 从队列中弹出一个元素,如果队列为空返回false
     * */
    bool try_pop(value_type& value){
        std::lock_guard<std::mutex>lk(mut);
        if(data_queue.empty())
            return false;
        value=std::move(data_queue.front());
        data_queue.pop();
        return true;
    }
    /*
     * 返回队列是否为空
     * */
    auto empty() const->decltype(data_queue.empty()) {
        std::lock_guard<std::mutex>lk(mut);
        return data_queue.empty();
    }
    /*
     * 返回队列中元素数个
     * */
    auto size() const->decltype(data_queue.size()){
        std::lock_guard<std::mutex>lk(mut);
        return data_queue.size();
    }
}; /* threadsafe_queue */
 
#endif
 
#endif
