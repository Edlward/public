#include "MessageQueue.h"    //消息队列


 NAME_SPACE_MAIN_BEGIN

std::atomic<unsigned long long> MessageQueue::gMessageQueueIDIndex(0);

void MessageQueue::clear()
{
    pRingBuf.clear();
    ListChildren.clear();
}

bool MessageQueue::pushMsg(const Message& msg,bool bBlock,int PushType,int64_t PushKey)
{
    //默认5ms  0XFFFFFFFF=INFINITE;表示无限等待
    unsigned int timeout=(bBlock)?(0XFFFFFFFF):(5); 
    return pushMsg(msg,timeout,PushType,PushKey);
}
bool MessageQueue::pushMsgForce(const Message& msg,int PushType,int64_t PushKey)
{
    //unsigned int timeout=5;   //默认5ms 
    //if(flag == true)
    //{
    //    timeout=0XFFFFFFFF;//INFINITE;      //无限等待
    //}
    //return pushMsg(msg,timeout,PushType,PushKey);

    return true;
}
bool MessageQueue::getMsg(Message& msg,bool bBlock)
{
    return pRingBuf.getOnlySingle(msg,bBlock);
}


//注意:一般发送消息可能存在多个线程同时push给某个模块，内部RingBuffer要用 ByMultiple版
//取消息，一般是线程内部处理调用，只有一个消费者不用ByMultiple版
bool MessageQueue::pushMsg(const Message& msg,unsigned int timeout,int PushType,int64_t PushKey)
{
   // return true;
    if(PushType==Push_Self)
    {
        bool ret=pRingBuf.pushMayMultiple(msg,timeout);   
        return ret;
    }

    LockerGuard<Mutex> lock(m_mutexListChild);  //互斥ListChildren链表操作 //但是否影响用ByMultiple显得多余？ 待续
    
    //推送给子队列
    std::vector<MessageQueue*>::iterator it;
    if(PushType==Push_ALL)
    {  
        //PreDprintf("PushType==Push_ALL begin\n");

        //PreDprintf("parentMQ:%p selfMQ:%p\n",pParent,this);

        bool ret=pRingBuf.pushMayMultiple(msg,timeout);       //推送给自身

        //PreDprintf("PushType==Push_ALL self end\n");


        for (it=ListChildren.begin();it!=ListChildren.end();++it)   //推送给子队列
        {
            //*it!=NULL 不判断空指针 不判断循环循环依赖导致递归
            //if (*it!=NULL&&*it!=this)
            //if ((*it)->FlagNoUse)
            //{
            //    (*it)->FlagNoUse=false;
            //    it=ListChildren.erase(it);  //返回下一个有效迭代器，无需加一
            //}
            //if (*it!=NULL)  //可能eraser之后为空
            //{
                 ret=(*it)->pushMsg(msg,timeout);
            //}

              //PreDprintf("PushType==Push_ALL child end\n");

        }
        return ret;
    }
    else if(PushType==Push_AllChild)
    {
        bool ret=true;
        for (it=ListChildren.begin();it!=ListChildren.end();++it)    //仅推送给子队列
        {
            //*it!=NULL 不判断空指针 不判断循环循环依赖导致递归
            //if (*it!=NULL&&*it!=this)
            //if ((*it)->FlagNoUse)
            //{
            //    (*it)->FlagNoUse=false;
            //    it=ListChildren.erase(it);  //返回下一个有效迭代器，无需加一
            //}
            //if (*it!=NULL)  //可能eraser之后为空
            //{
                  ret=(*it)->pushMsg(msg,timeout);
            //}
        }
        return ret;
    }
    else if(PushType==Push_OneType)
    {
        bool ret=true;
        for (it=ListChildren.begin();it!=ListChildren.end();++it)
        {
            //if ((*it)->FlagNoUse)
            //{
            //    (*it)->FlagNoUse=false;
            //    it=ListChildren.erase(it);  //返回下一个有效迭代器，无需加一
            //}
            //if (*it!=NULL)  //可能eraser之后为空
            //{
                if ((*it)->getMqType()==PushKey)   //推送到ID等于特定的队列
                {
                    ret=(*it)->pushMsg(msg,timeout);
                    return ret;
                }
            //}
        }
        return ret;
    }
    else if(PushType==Push_OneChild)
    {
        bool ret=true;
        for (it=ListChildren.begin();it!=ListChildren.end();++it)
        {
            //if ((*it)->FlagNoUse)
            //{
            //    (*it)->FlagNoUse=false;
            //    it=ListChildren.erase(it);  //返回下一个有效迭代器，无需加一
            //}
            //if (*it!=NULL)  //可能eraser之后为空
            //{
                if ((*it)->getMqID()==PushKey)   //推送到ID等于特定的队列
                {
                    ret=(*it)->pushMsg(msg,timeout);
                    return ret;
                }
            //}
        }
    }
    else if (PushType==Push_ExceptOne)
    {
        bool ret=true;
        for (it=ListChildren.begin();it!=ListChildren.end();++it)
        {
            //if ((*it)->FlagNoUse)
            //{
            //    (*it)->FlagNoUse=false;
            //    it=ListChildren.erase(it);  //返回下一个有效迭代器，无需加一
            //}
            //if (*it!=NULL)  //可能eraser之后为空
            //{
                if (this!=(void*)PushKey)   //除了某个之外，全部推送，一般用在推送给兄弟队列
                {
                    ret=(*it)->pushMsg(msg,timeout);
                    //return ret;
                }
            //}
        }
        return ret;
    }
    return true;
}

bool MessageQueue::pushToAllBrother(const Message& msg,unsigned int timeout,int PushType,int64_t PushKey)
{
    MessageQueue* parent=getParent();
    if (parent!=NULL)
    {
        return parent->pushMsg(msg,timeout,Push_ExceptOne,(int64_t)this);
    }
    return false;
}

bool MessageQueue::pushToAllBrother(const Message& msg,bool flag,int PushType,int64_t PushKey)
{
    MessageQueue* parent=getParent();
    if (parent!=NULL)
    {
        return parent->pushMsg(msg,flag,Push_ExceptOne,(int64_t)this);
    }
    return false;
}

bool MessageQueue::getMsg(Message& msg,unsigned int timeout)
{
    return pRingBuf.getOnlySingle(msg,timeout);
}

void MessageQueue::addChildren(MessageQueue* mq)
{
    //if (mq!=this)      //避免循环,自己不能作为自己的子队列
    if(!isSame(mq))
    {
        mq->setParent(this);
        LockerGuard<Mutex> lock(m_mutexListChild);  //互斥ListChildren链表操作
        ListChildren.push_back(mq);
    }
}

void MessageQueue::clearMsg()
{
    pRingBuf.clear();
}

void MessageQueue::removeChild(MessageQueue* mq)
{
    if (mq==NULL)
    {
        return;
    }

    //std::cout<<"MessageQueue::removeChild() mq="<<(void*)mq<<std::endl;
    LockerGuard<Mutex> lock(m_mutexListChild);  //互斥ListChildren链表操作

    std::vector<MessageQueue*>::iterator it;
    for (it=ListChildren.begin();it!=ListChildren.end();) //++it
    {
        //if (*it==mq)
        if((*it)->isSame(mq))
        {
            it=ListChildren.erase(it);  //返回下一个有效迭代器，无需加一
            break;
        }
        else
        {
            it++;
        }
    }
}


NAME_SPACE_MAIN_END
