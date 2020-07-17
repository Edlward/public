#ifndef _Heap_Header_
#define _Heap_Header_

#include "../Basic/global.h"

//creator by lizulin 

//堆 heap
//堆也叫优先队列，堆是一种特殊的完全二叉树数据结
//堆分为两种，最大堆，最小堆。
//最大堆：根节点大于左右两个子节点的完全二叉树
//最小堆：根节点小于左右两个子节点的完全二叉树
//堆可以用数组来存储，

//数组下标i从0开始,i=0是根节点。
//a[i]处存根节点，a[2 * i+1]存左子树的根节点 ; a[2 * i + 2]存右子树的根节点。

//最大堆的插入：时间复杂度O(logn)
//肯定在数组末尾加入该数字，然后和父节点进行比较,也就是节点上浮操作。
//
//最大堆的删除：时间复杂度O(logn)
//删除根节点元素，用数组的最后一个元素来顶替，然后进行节点下沉。


//TODO:
//对于结构体，可以自定义比较函数 ，待续
//最大堆/最小堆补全。容量设定, 递归/异常。


//比较函数
template<class T>
struct Less
{
    bool operator()(const T& Lv, const T& Rv)
    {
        return Lv<Rv;
    }
};

template<class T>
struct Greater
{
    bool operator()(const T& Lv, const T& Rv)
    {
        return Lv>Rv;
    }
};

template<class T>
struct LessEqual
{
    bool operator()(const T& Lv, const T& Rv)
    {
        return Lv=<Rv;
    }
};

template<class T>
struct GreaterEqual
{
    bool operator()(const T& Lv, const T& Rv)
    {
        return Lv>=Rv;
    }
};


//用了仿函数的方式去复用最大堆和最小堆的代码。在这里默认把堆调整为最小堆。
//template<class T, template<class> class Continer = Less>   //声明方式1

//声明方式1
//template<class T,template<class> class Continer = Greater>//默认为大堆 
//Continer<T> _con;  //内部成员

//声明方式2
//template<class T,class Cmp>

#include<vector>  //不使用vector
#include<type_traits>

template<class T,template<class T> class CmpFun=Less>
class Heap
{
    //to be confirm
    //是否是底层叶子节点   节点0，1情况 size=1，pos=0？
    static inline bool isLeaf(size_t  pos,size_t sizeHeap)
    {
        return (pos>=sizeHeap/2)&&(pos<sizeHeap);
        // return (pos!=0)&&(pos>=m_size/2)&&(pos<m_size);
    }

    //左节点下标
    static inline size_t leftchild(size_t pos)
    { 
        return 2*pos+1;
    }
    //右边节点下标
    static inline size_t  rightchild(size_t pos)
    { 
        return 2*pos+2;
    }
    //父节点下标  pos=0的特殊情况溢出，暂时
    static inline size_t parent(size_t pos)
    { 
        return (pos-1)/2;
    }
public:
    Heap(){}
    Heap(const T* arr,size_t size)
    {
        Listdata.reserve(size);
        for (size_t n = 0; n <size; ++n)
        {
            Listdata.push_back(arr[n]);
        }
        //int root=(Listdata.size()-2)/2;//找到第一个非叶子结点
        //while (root>=0)
        //{
        //     shift_down(root--);  //向下调整
        //}

        //for(int i= (m_size-2)/2; i>=0;i--)
        for(size_t n=(Size()-2)/2+1;n>0;n--)
        {
            shift_down(n-1);
        }
    }
    ~Heap()
    {
    }
    bool push(const T & t)
    {
        Listdata.push_back(t);
        shift_up(Listdata.size()-1);
        return true;
    }
    bool push(const T & t,size_t maxSize)  //=UINT_MAX 限制容量的推送版本
    {
        if (Size()>=maxSize)
        {
            return false;
        }
        Listdata.push_back(t);
        shift_up(Listdata.size()-1);
        return true;
    }
    bool push(const T* arr,size_t size)  //同时推送多个元素版本
    {
        for (size_t n=0;n<size;n++)
        {
            bool retP=push(arr[n]);
            if (!retP)
            {
                return false;
            }
        }
        return true;
    }
    void pop()
    {
        T tmp;
        bool ret=pop(tmp);
    }
    bool pop(T& val)
    {
        //if (Listdata.size() == 0)
        //{
        //    return;
        //}
        //if (Listdata.size() <= 2)
        //{
        //    swap(Listdata[0], Listdata[Listdata.size() - 1]);
        //    Listdata.pop_back();
        //}
        //else
        //{
        //    swap(Listdata[0], Listdata[Listdata.size() - 1]);
        //    Listdata.pop_back();
        //    shift_down(0);
        //}

        if(this->Size()==0)
        {
            return false;
        }

        T* pData=Listdata.data();
        val = pData[0];                               //取出头,
        pData[0] = pData[this->Size()-1];   //尾部放到头部
        Listdata.pop_back();   //
        shift_down(0);
        return true;
    }

    void pushCover(const T& val,size_t maxSize)
    {
        if(this->Size()>=maxSize)
        {
            T tmp;
            pop(tmp);
        }
        push(val);
    }

    bool IsEmpty() const{ return this->Size()==0;}
    bool IsFull() const{ return this->Size()==this->Capacity();}
    size_t Size() const{ return Listdata.size();}
    size_t Capacity() const{ return Listdata.capacity();}
    const T& top() const
    {
        if (Listdata.size()!=0)
        {
            return Listdata[0];
        }
        else
        {
            throw exception("this is a bug");
        }
    }

    //移除等于某个值的版本  找到并删除返回true,否则为false
    bool delValue(const T& key)  
    {
        if(this->Size()==0)
        {
            return false;
        }
        T* pData=Listdata.data();
        for (size_t i = 0; i <Listdata.size(); ++i)
        {
            if (key==pData[i])
            {
                    pData[i] = pData[this->Size()-1];   //尾部放到此处
                    Listdata.pop_back();   //删去尾部元素

                    //删除元素对于最大堆和最小堆操作不同
                    if (std::is_same<Less<T>,CmpFun<T>>::value)   //最小堆
                    {
                         shift_up(i);
                    }
                    else if(std::is_same<Greater<T>,CmpFun<T>>::value)  //最大堆
                    {
                        shift_down(i);
                    }
                    else
                    {
                             printf("error cmpFun\n");   //throw exception("error cmpFun");
                    }
                    continue;
            }
        }
        return true;
    }

    void print()const
    {
        T* pData=Listdata.data();
        for(size_t i=0; i<Size(); ++i)
        {
            std::cout<<pData[i]<<" ";
        }
        std::cout << std::endl;
    }
    void popPrint()
    {
        while (!IsEmpty())
        {
            std::cout <<top()<< " ";
            this->pop();
        }
        std::cout<<std::endl;
    }
protected:
    //void shift_down(int root)
    //{
    //    while (root <= (Listdata.size() - 2) / 2)
    //    {
    //        int child = root * 2 + 2 >= Listdata.size() ? root * 2 + 1 : Cmp()(Listdata[root * 2 + 1], Listdata[root * 2 + 2]) ? root * 2 + 1 : root * 2 + 2;
    //        if (!Cmp()(Listdata[root], Listdata[child]))
    //        {
    //            swap(Listdata[root], Listdata[child]);
    //        }
    //        else
    //        {
    //            break;
    //        }
    //        root = child;
    //    }
    //}
    //void shift_up(int child)
    //{
    //    while (child >= 1)
    //    {
    //        int root = (child - 1) / 2;
    //        if (!Cmp()(Listdata[root],Listdata[child]))
    //        {
    //            swap(Listdata[root], Listdata[child]);
    //        }
    //        child = root;
    //    }
    //}

    void shift_up(size_t  curr)
    {
        T* pData=Listdata.data();
        //只要不是根节点 curr一开始=0则不运算
        while(curr!=0)
        {
            size_t posParent=parent(curr);
            if(Cmp(pData[posParent],pData[curr]))   //根据比较函数不同,决定最大堆,最小堆。
            {
                break;
            }
            Swap(pData[curr],pData[posParent]);   //大于等于则退出
            curr=posParent;
        }
    }

    void shift_down(size_t pos)
    {
        if (this->Size()==0)
        {
            return;
        }

        T* pData=Listdata.data();
        while(!isLeaf(pos,this->Size()))
        {
            size_t posL=leftchild(pos);
            size_t posR=rightchild(pos);
            size_t posSeclect=posL;    //选中的最小 最大节点序号。

            //Means pos has right child 有右子节点
            if(posR<this->Size())
            {
                if(Cmp(pData[posR],pData[posL]))   //取子树中小大节点的下标
                {
                    posSeclect=posR;
                }
            }
            if(Cmp(pData[pos],pData[posSeclect]))
            {
                return;
            }
            Swap(pData[pos],pData[posSeclect]);
            pos=posSeclect;
        }
    }

    //合并待续
    void AdjustUpV2(size_t child)
    {
        T* pData=Listdata.data();
        size_t parent = (child - 1) / 2;
        while (child > 0)
        {
            if (Cmp(pData[child],pData[parent]))
            {
                Swap(pData[child],pData[parent]);
                child = parent;
                parent = (child - 1) / 2;
            }
            else
            {
                break;
            }
        }
    }
    void AdjustDownV2(size_t parent)
    {
        T* pData=Listdata.data();

        size_t child = parent * 2 + 1;
        size_t size =Listdata.size();
        while (child < size)
        {
            if (child + 1 < size&&_con(pData[child+1],pData[child]))
            {
                //注意这必须是child+1更大或更小，所以把child+1放在前面
                    ++child;
            }
            if (Cmp(pData[child],pData[parent]))
            {
                Swap(pData[parent],pData[child]);
                parent = child;
                child = parent * 2 + 1;
            }
            else
            {
                break;
            }
        }
    }
protected:
    //std::vector<T> Listdata;  //去掉vector依赖？暂留，可以自动扩容
    SArray<T> Listdata;         //用自定义容器,设置设置内置空间
    CmpFun<T> Cmp;
};


#ifdef UNIT_TEST
int Test_Heap();
#endif

#endif

