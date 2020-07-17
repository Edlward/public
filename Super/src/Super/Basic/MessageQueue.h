#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_

#include "global.h"
#include <vector>
#include <memory.h>

#include<atomic>  //for uuid

#include "RingBuffer.h"   //线程安全的环形缓冲区

NAME_SPACE_MAIN_BEGIN


//常用消息 比较常用与具体业务无关的消息
enum EBaseMessageType
{
    BaseMsg_Begin=0X0000,
    BaseMsg_UnKnow=BaseMsg_Begin,

    //常用基础消息类型
    BaseMsg_TimeOut,           //定时器到时消息

    BaseMsg_Max,                
    BaseMsg_User,                 //用户消息类型起始值
};

struct Message
{
    int type;
    char buf[256];               //消息内容
    unsigned int msglen;     //消息长度 改进 根据长度调用拷贝构造函数，避免无用拷贝
    typedef Message self;
    Message()
    {
        type=BaseMsg_UnKnow;
        msglen=0;
    }
    //根据长度调用拷贝构造函数，避免无用拷贝，只拷贝有效数据
    Message(const self& R)            //拷贝构造
    {
        copyFrom(R);
    }
    self& operator=(const self& R)  //赋值构造
    {
        copyFrom(R);
        return *this;
    }
    void copyFrom(const self& R)
    {
        this->type=R.type;
        this->msglen=R.msglen;
        if (R.msglen>0)
        {
            memcpy(this->buf,R.buf,R.msglen);
        }
    }
};


template<typename T,size_t inSize> class RingBuffer;
//使用注意:一般不要使用阻塞push,除非确定的地方get处理消息，否则队列满了之后会程序阻塞运行

//class MessageQueuePrivate;
//template <size_t bufferSizeMax=256>
class BASIC_API MessageQueue
{
public:
    MessageQueue(void):pRingBuf(256),pParent(NULL)
    {
        MqID=-1;
        MqType=0;
        mqUUID=gMessageQueueIDIndex++;
    }
    ~MessageQueue(void)
    {
        clear();
    }

    void clear();         //清除消息队列
    //NOTE:PushKey根据PushType的需要可能是指针，为了兼容64位程序用int64_t
    //为效率推数据用引用
    bool pushMsg(const Message& msg,bool bBlock=false,int PushType=Push_ALL,int64_t PushKey=-1);
    
    //强制推送,若队列已满了,移除队列尾再推送
    bool pushMsgForce(const Message& msg,int PushType=Push_ALL,int64_t PushKey=-1);  
    bool getMsg(Message& msg,bool flag);

    //直接指定延时去等待 timeout=0XFFFFFFFF会无限延时阻塞  MqID=-1默认同时推送给全部子队列，否则只推送到指定的队列
    bool pushMsg(const Message& msg,unsigned int timeout=Wait_Dafault,int PushType=Push_ALL,int64_t PushKey=-1);   
    bool getMsg(Message& msg,unsigned int timeout=Wait_Dafault);

    //通过父指针，向其兄弟队列推送信息,不再推送给自己
    bool pushToAllBrother(const Message& msg,unsigned int timeout=Wait_Dafault,int PushType=Push_ALL,int64_t PushKey=-1);

    //通过父指针，向其兄弟队列推送信息,不再推送给自己
    bool pushToAllBrother(const Message& msg,bool bBlock,int PushType=Push_ALL,int64_t PushKey=-1);

   void clearMsg();    //清空消息队列里面的消息


   //是否有必要加锁，一般add remove在同一线程运行，add一般初始时刻
   //待完善，如何维护子队列，可以从父类新增，子类删除，还不影响效率？  待续
   void addChildren(MessageQueue* mq);
   void removeChild(MessageQueue* mq);
   void removeFromParent()  //是否必要？
   {
       MessageQueue* pParent=getParent();
       if (pParent!=NULL)
       {
           pParent->removeChild(this);
           pParent=NULL;     //移除完成没有父队列了 复位
       }
   }

   //void setNoUse(bool b){FlagNoUse=b;}
   void setMqType(int n){MqType=n;}
   int getMqType()const {return MqType;}
   void setMqID(int n){MqID=n;}
   int getMqID()const {return MqID;}
private:
    RingBuffer<Message> pRingBuf;
    Mutex m_mutexListChild;
    std::vector<MessageQueue*> ListChildren;      //old std::list大多是遍历操作，极少有删除操作,改用vector

    //bool FlagNoUse;                                             //不再使用标记，如果被设置为true,将被从父类的ListChildren删除
    int MqID;
    int MqType;
    //MessageQueuePrivate* pimpl;                       //为了不在头文件引入,其他系统相关头文件，mutex用pimpl实现
private:
   void  setParent(MessageQueue* p)
   {
       //父亲队列指针不能为自身
       if(p==NULL||p==this) 
       {
           return;
       }
       pParent=p;
   }
   MessageQueue*getParent() const{return pParent;}
private:
    MessageQueue* pParent;


public:
    //判断是否同一队列,仅凭指针是不可靠的.因为指针可能会被重复使用,有可能已经被析构过了
    //注意理解此设计意图
    bool isSame(MessageQueue* mq)
    {
        if (mq!=this)
        {
            return false;
        }
        return this->mqUUID==mq->mqUUID;
    }
private:
    unsigned long long mqUUID;   //用来唯一区分不同消息队列
    static std::atomic<unsigned long long> gMessageQueueIDIndex;
};

//BASIC_API MessageQueue* GetMessageQueue(unsigned int nInstance);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//AB双方双工通信,不区分主从,仅仅是个名称而已
//实用于各种线程功能类的前后台交互通信
class BASIC_API MessageQueueDuplex
{
public:
    MessageQueueDuplex():pRingBufMaster(256),pRingBufSlaver(256)
    {

    }

    //直接指定延时去等待 timeout=0XFFFFFFFF会无限延时阻塞   
    bool pushMsgToSlaver(const Message& msg,unsigned int timeout=Wait_Dafault)
    {
        return pRingBufSlaver.pushOnlySingle(msg,timeout);
    }
    bool getMsgFromSlaver(Message& msg,unsigned int timeout=Wait_Dafault)
    {
        return pRingBufSlaver.getOnlySingle(msg,timeout);
    }
    bool pushMsgToMaster(const Message& msg,unsigned int timeout=Wait_Dafault)
    {
        return pRingBufMaster.pushOnlySingle(msg,timeout);
    }
    bool getMsgFromMaster(Message& msg,unsigned int timeout=Wait_Dafault)
    {
        return pRingBufMaster.getOnlySingle(msg,timeout);
    }
private:
    RingBuffer<Message> pRingBufMaster;
    RingBuffer<Message> pRingBufSlaver;
};





NAME_SPACE_MAIN_END

#endif //MESSAGEQUEUE_H_

