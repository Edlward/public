#ifndef Process_Header
#define Process_Header


//Process operation Cross-Platform
//进程操作，跨平台

//进程优先级暂时考虑windows的



class Process
{
public:
    Process(void);
    ~Process(void);

    enum Priority
    {
        Idle,              //空闲
        Low,              //最低
        Below,           //低于正常
        Normal,         //正常
        Above,          //高于正常
        High,            //最高优先级
        Real,             //实时优先级
    };


    static void* CurrProcess();                //当前进程handle
    static int CurrProcessID();                //当前进程ID

    static bool setCurrProcessPriority(Priority p);     //设置进程程优先级
    static int getCurrProcessPriority();                      //获取进程优先级
};

#endif
