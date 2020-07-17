#ifndef THREAD_POOL_H
#define THREAD_POOL_H



#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>



//Task��װ�����ͺ�������,������㷨��Ҫ��������ָ�룬Ԥ��һ��������ָ��,�������ݲ���
//bool �����߳������˳�,
typedef std::function<void(bool*,void*,void*,void*)> TaskFun;
class Task
{
public:
    //TaskFun mfun=0,�ᱨ��ִ���
    Task(TaskFun mfun=nullptr,void* pv0=0,void* pv1=0,void* pv2=0)
        :m_taskFun(mfun),p0(pv0),pd1(pv1),pd2(pv2)
    {

    }
    ~Task(){}
    TaskFun m_taskFun;
    bool* pCtrl;
    void* p0;
    void* pd1;
    void* pd2;
    void doCallFun()
    {
        //if (this->m_taskFun!=NULL)   //ÿ���ж��˷�����,���������ʱ�жϼ���
        {
            this->m_taskFun(pCtrl,p0,pd1,pd2);
        }
    }

    //bool operator < (const Task &R) const  
    //{  
    //    return R.p0<this->p0;  
    //} 
};




//�̳߳�,�����ύ��κ�������ķ����ʽ����������ִ��,���Ի�ȡִ�з���ֵ
//��֧�����Ա����, ֧���ྲ̬��Ա������ȫ�ֺ���,Opteron()������
class ThreadPool
{
    //using Task = std::function<void()>;  //vs2012 error
    //typedef std::function<int(void*,void*)> Task;
    std::vector<std::thread> ListThreadPool;        //�̳߳�
    std::queue<Task> ListTask;                            //�������
    std::mutex m_lock;                                         //ͬ��
    std::condition_variable cv_task;                     //��������
    bool stoped;                                                   //�Ƿ�ر��ύ  std::atomic<bool> 
    std::atomic<unsigned int>  idlThreadNum;    //�����߳�����
    unsigned int maxthreadNum;
public:
    inline ThreadPool(unsigned int  threadNum=4):stoped(false)
    {
          maxthreadNum=threadNum>20?20:threadNum;   //��������߳�����,̫��û������
          idlThreadNum = maxthreadNum;
    }
    inline ~ThreadPool()
    {
        Stop();
    }
    //�����߳�����
    unsigned int idlThreadCount() { return idlThreadNum;}
    bool addTask(Task task)
    {
        if (task.m_taskFun==nullptr)  //����ָ��Ϊ�գ��Ƿ����񣬲����
        {
            return false;
        }

        task.pCtrl=&stoped;   //���ݿ����˳���ָ��
        ListTask.push(task);
        return true;
    }
    void Start()
    {
        for (unsigned int  n= 0;n< maxthreadNum;++n)
        {   
            //��ʼ���߳�����
            ListThreadPool.emplace_back(
                [this]
            { 
                // �����̺߳���
                while(!this->stoped)
                {
                    Task task;
                    {   
                        // ��ȡһ����ִ�е� task
                        // unique_lock ��� lock_guard �ĺô��ǣ�������ʱ unlock() �� lock()
                        std::unique_lock<std::mutex> lock(this->m_lock);
                        this->cv_task.wait(lock,[this] 
                        {
                            //return this->stoped.load() || !this->ListTask.empty();
                            return this->stoped||!this->ListTask.empty();
                        }
                        ); // wait ֱ���� task

                        if (this->stoped && this->ListTask.empty())
                        {
                            return;
                        }
                        task = std::move(this->ListTask.front()); // ȡһ�� task
                        this->ListTask.pop();
                    }

                    idlThreadNum--;
                    //task();
                    task.doCallFun();

                    idlThreadNum++;
                }
            }
            );
        }
    }
    void Stop()
    {
        //stoped.store(true);
        stoped=true;
        cv_task.notify_all(); // ���������߳�ִ��
        for (std::thread& thread : ListThreadPool) 
        {
            //thread.detach(); // ���̡߳���������
            if(thread.joinable())
            {
                thread.join(); // �ȴ���������� ǰ�᣺�߳�һ����ִ����
            }
        }
    }
public:
};




//https://www.cnblogs.com/lzpong/p/6397997.html
//VS2012��ʱ��֧�ֿɱ����������
//
#if 0

#define  MAX_THREAD_NUM 256

//�̳߳�,�����ύ��κ�������ķ����ʽ����������ִ��,���Ի�ȡִ�з���ֵ
//��֧�����Ա����, ֧���ྲ̬��Ա������ȫ�ֺ���,Opteron()������
class ThreadPool
{
    //using Task = std::function<void()>;  //vs2012 error
    typedef std::function<void()> Task;

    // �̳߳�
    std::vector<std::thread> pool;
    // �������
    std::queue<Task> tasks;
    // ͬ��
    std::mutex m_lock;
    // ��������
    std::condition_variable cv_task;
    // �Ƿ�ر��ύ
    std::atomic<bool> stoped;
    //�����߳�����
    std::atomic<int>  idlThrNum;

public:
    inline ThreadPool(unsigned short size = 4) :stoped(false)
    {
        idlThrNum = size < 1 ? 1 : size;
        for (size = 0; size < idlThrNum; ++size)
        {   //��ʼ���߳�����
            pool.emplace_back(
                [this]
            { // �����̺߳���
                while(!this->stoped)
                {
                    std::function<void()> task;
                    {   
                        // ��ȡһ����ִ�е� task
                        // unique_lock ��� lock_guard �ĺô��ǣ�������ʱ unlock() �� lock()
                        std::unique_lock<std::mutex> lock(this->m_lock);

                        this->cv_task.wait(lock,[this] 
                        {
                                return this->stoped.load() || !this->tasks.empty();
                        }
                        ); // wait ֱ���� task

                        if (this->stoped && this->tasks.empty())
                        {
                            return;
                        }
                        task = std::move(this->tasks.front()); // ȡһ�� task
                        this->tasks.pop();
                    }
                    idlThrNum--;
                    task();
                    idlThrNum++;
                }
            }
            );
        }
    }
    inline ~ThreadPool()
    {
        stoped.store(true);
        cv_task.notify_all(); // ���������߳�ִ��
        for (std::thread& thread : pool) 
        {
            //thread.detach(); // ���̡߳���������
            if(thread.joinable())
            {
                thread.join(); // �ȴ���������� ǰ�᣺�߳�һ����ִ����
            }
        }
    }

public:
    // �ύһ������
    // ����.get()��ȡ����ֵ��ȴ�����ִ����,��ȡ����ֵ
    // �����ַ�������ʵ�ֵ������Ա��
    // һ����ʹ��   bind�� .commit(std::bind(&Dog::sayHello, &dog));
    // һ������ mem_fn�� .commit(std::mem_fn(&Dog::sayHello), &dog)
    template<class F,class ...Args>
    auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))>
    {
        if (stoped.load())    // stop == true ??
            throw std::runtime_error("commit on ThreadPool is stopped.");

        using RetType = decltype(f(args...)); // typename std::result_of<F(Args...)>::type, ���� f �ķ���ֵ����
        auto task = std::make_shared<std::packaged_task<RetType()> >
            (
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );   
        // wtf !
        std::future<RetType> future = task->get_future();
        {    // ������񵽶���
            std::lock_guard<std::mutex> lock{ m_lock };//�Ե�ǰ���������  lock_guard �� mutex �� stack ��װ�࣬�����ʱ�� lock()��������ʱ�� unlock()
            tasks.emplace(
                [task]()
            { // push(Task{...})
                (*task)();
            }
            );
        }
        cv_task.notify_one(); // ����һ���߳�ִ��

        return future;
    }

    //�����߳�����
    int idlCount() { return idlThrNum; }

};
#endif


#ifdef UNIT_TEST
int Test_ThreadPool();
#endif


#endif
