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
    // ԭ�Ӳ���
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
                // wait��ʱ���Զ��ͷ��������wait���˻��ȡ��
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

//��Ȩ����������Ϊ����ԭ�����£�ת����ע��Դ��ַ��          https://blog.csdn.net/10km/article/details/52067929        
//
//C++�е�ģ��std::queue�ṩ��һ����������,����������������̰߳�ȫ�ģ�����ڶ��̻߳�����ʹ�ö��У����ǲ���ֱ�������õġ� 
//    ��������һ���̰߳�ȫ�Ķ���Ҳ�������ӡ�������ԭ�������std::mutext�ź�����std::queue���з��ʿ��ƣ��Ա�֤�κ�һ���̶߳��Ƕ�ռʽ���ʣ������������Ĵ��롣

/*
 * threadsafe_queue.h
 *
 *  Created on: 2016��7��26��
 *      Author: guyadong
 */

 
#include <queue>
#include <mutex>
#include <condition_variable>
#include <initializer_list>
 
/*
 * �̰߳�ȫ����
 * TΪ����Ԫ������
 * ��Ϊ��std::mutex��std::condition_variable���Ա,���Դ��಻֧�ָ��ƹ��캯��Ҳ��֧�ָ�ֵ������(=)
 * */
template<typename T>
class threadsafe_queue{
private:
    // data_queue�����ź���
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
     * ʹ�õ�����Ϊ�����Ĺ��캯��,����������������
     * */
    template<typename _InputIterator>
    threadsafe_queue(_InputIterator first, _InputIterator last){
        for(auto itor=first;itor!=last;++itor){
            data_queue.push(*itor);
        }
    }

    explicit threadsafe_queue(const container_type &c):data_queue(c){}
    /*
     * ʹ�ó�ʼ���б�Ϊ�����Ĺ��캯��
     * */
    threadsafe_queue(std::initializer_list<value_type> list):threadsafe_queue(list.begin(),list.end()){
    }
    /*
     * ��Ԫ�ؼ������
     * */
    void push(const value_type &new_value){
        std::lock_guard<std::mutex>lk(mut);
        data_queue.push(std::move(new_value));
        data_cond.notify_one();
    }
    /*
     * �Ӷ����е���һ��Ԫ��,�������Ϊ�վ�����
     * */
    value_type wait_and_pop(){
        std::unique_lock<std::mutex>lk(mut);
        data_cond.wait(lk,[this]{return !this->data_queue.empty();});
        auto value=std::move(data_queue.front());
        data_queue.pop();
        return value;
    }
    /*
     * �Ӷ����е���һ��Ԫ��,�������Ϊ�շ���false
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
     * ���ض����Ƿ�Ϊ��
     * */
    auto empty() const->decltype(data_queue.empty()) {
        std::lock_guard<std::mutex>lk(mut);
        return data_queue.empty();
    }
    /*
     * ���ض�����Ԫ������
     * */
    auto size() const->decltype(data_queue.size()){
        std::lock_guard<std::mutex>lk(mut);
        return data_queue.size();
    }
}; /* threadsafe_queue */
 
#endif
 
#endif
