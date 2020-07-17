#ifndef Process_Header
#define Process_Header


//Process operation Cross-Platform
//���̲�������ƽ̨

//�������ȼ���ʱ����windows��



class Process
{
public:
    Process(void);
    ~Process(void);

    enum Priority
    {
        Idle,              //����
        Low,              //���
        Below,           //��������
        Normal,         //����
        Above,          //��������
        High,            //������ȼ�
        Real,             //ʵʱ���ȼ�
    };


    static void* CurrProcess();                //��ǰ����handle
    static int CurrProcessID();                //��ǰ����ID

    static bool setCurrProcessPriority(Priority p);     //���ý��̳����ȼ�
    static int getCurrProcessPriority();                      //��ȡ�������ȼ�
};

#endif
