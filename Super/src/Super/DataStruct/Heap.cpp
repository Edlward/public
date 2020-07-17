#include "Heap.h"

#include <iostream>
using namespace std;
 

#if 0
//creator by lizulin

//堆 heap
//堆也叫优先队列，堆是一种特殊的完全二叉树数据结
//堆分为两种，最大堆，最小堆。
//最大堆：根节点大于左右两个子节点的完全二叉树
//最小堆：根节点小于左右两个子节点的完全二叉树
//堆可以用数组来存储，

//数组下标i从0开始,i=0是根节点。
//a[i]处存根节点，a[2 * i+1]存左子树的根节点 ; a[2 * i + 2]存右子树的根节点。

//    最大堆的插入：时间复杂度O(logn)
//    肯定在数组末尾加入该数字，然后和父节点进行比较,也就是节点上浮操作。
//
//    最大堆的删除：时间复杂度O(logn)
//    删除根节点元素，用数组的最后一个元素来顶替，然后进行节点下沉。


//TODO:
//对于结构体，可以自定义比较函数 ，待续
//最大堆/最小堆补全。容量设定, 递归/异常。


#if 0
//递归写法 参考
void down(int index)
{
    assert(index >= 0);
    int temp = index;
    index = index * 2 + 1;
    if (index >= size) return;
    if (index+1<size&&heap[index]<heap[index+1]) index++;
    if (heap[index] < heap[temp]) return;
    else
    { 
        swap(heap[index], heap[temp]);
        down(index);
    }
}
void up(int index)
{
    assert(index < size);
    if (index == 0) return; 
    if (heap[index] < heap[(index - 1) / 2]) return;
    else 
    {
        swap(heap[index], heap[(index - 1) / 2]);
        up((index - 1) / 2);
    }
}
//很明显，递归写法两元素交换次数比非递归写法多。
#endif


//new add 
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


//最大堆
template <typename T>
class MaxHeap 
{
private:
    T *pData;
    size_t m_size;
    size_t m_capacity;
public:
    MaxHeap(size_t maxSize)
    {
        m_capacity=maxSize;
        pData = new T[m_capacity];
        m_size=0;
    }
    MaxHeap(const T arr[],size_t arrSize)
    {
        m_capacity=arrSize;
        pData = new T[m_capacity];
        m_size = arrSize;
        for(size_t i=0; i<m_size; ++i)
        {
            pData[i]=arr[i];
        }
        //for(int i= (m_size-2)/2; i>=0;i--)
        for(size_t i= (m_size-2)/2+1; i>0;i--)
        {
            shift_down(i-1);
        }
        //int curPos=m_size/2-1;     //m_size等于0考虑？
        //while(curPos >= 0)
        //{
        //    shift_down(curPos,m_size-1);
        //    --curPos;
        //}
    }
    ~MaxHeap()
    {
        delete[] pData;
        pData=NULL;
        m_capacity=m_size=0;
    }

    bool push(const T& val)
    {
        if(m_size>=m_capacity)
        {
            return false;
        }

        pData[m_size] = val;
        shift_up(m_size);
        ++m_size;
        return true;
    }
    void pushCover(const T& val)
    {
        if(m_size>=m_capacity)
        {
            T tmp;
            pop(tmp);
        }
        push(val);
    }
    bool pop(T &val)
    {
        if(m_size <= 0)
        {
            return false;
        }
        val = pData[0];
        pData[0]=pData[m_size-1];
        --m_size;
        //shift_down(0, m_size-1);
        shift_down(0);
        return true;
    }
    const T & top() const
    {
        if (m_size!=0)
        {
            return pData[0];
        }
        else
        {
            return T();  //失败抛异常或返回空值--返回引用 有问题
        }
    }

    bool IsEmpty() const{ return m_size==0;}
    bool IsFull() const{ return m_size==m_capacity;}
    size_t Size() const{ return m_size;}
    size_t Capacity() const{ return m_capacity;}

    void print()const
    {
        for(size_t i=0; i<m_size; ++i)
        {
            std::cout<<pData[i]<<" ";
        }
        std::cout << std::endl;
    }
protected:


    //Make sure pos is bigger than sub-tree
    //自上而下规范所有的格式以满足最大堆定义
    //void shift_down(size_t  start,size_t  end)  //合并待续
    //{
    //    size_t  i = start;
    //    size_t  j = i*2 + 1;
    //    T tmp = pData[i];
    //    while(j <= end)
    //    {
    //        if(j+1 <= end && pData[j] < pData[j+1])
    //        {
    //            ++j;
    //        }
    //        if(tmp < pData[j])
    //        {
    //            pData[i] = pData[j];
    //        }
    //        else
    //        {
    //            break;
    //        }
    //        i = j;
    //        j = j*2 + 1;
    //    }
    //    pData[i] = tmp;
    //}

    //void shift_up(size_t  start) //old
    //{
    //   size_t  j = start;
    //   size_t  i = (start-1) / 2;
    //    T tmp = pData[j];
    //    while(j > 0)
    //    {
    //        if(tmp < pData[i])
    //        {
    //            break;
    //        }
    //        else
    //        {
    //            pData[j] = pData[i];
    //        }
    //        j = i;
    //        i = (i-1) / 2;
    //    }
    //    pData[j] = tmp;
    //}

    void shift_up(size_t  curr)
    {
        //只要不是根节点 curr一开始=0则不运算
        while(curr!=0)
        {
            size_t posParent=parent(curr);
            if(pData[curr]<=pData[posParent])     //最大堆,小于等于则不交换。
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

        while(!isLeaf(pos,this->Size()))
        {
            size_t posL=leftchild(pos);
            size_t posR=rightchild(pos);
            size_t posMaxLR=posL;

            //Means pos has right child 有右子节点
            if(posR<m_size)
            {
                if(pData[posR]>pData[posL])   //取子树中大的下标
                {
                    posMaxLR=posR;//posL=posR;
                }
            }
            if(pData[pos]>=pData[posMaxLR])	//子树中大的与pos处进行比较，大于则退出
            {
                return;
            }
            Swap(pData[pos],pData[posMaxLR]);     //交互当前节点与其最大子节点的值
            pos=posMaxLR;
        }
    }
};






//最小堆
template <typename T>
class MinHeap 
{
private:
    T *pData;
    size_t m_size;
    size_t m_capacity;
public:
    MinHeap(size_t maxSize)
    {
        m_capacity=maxSize;
        pData = new T[m_capacity];
        m_size = 0;
    }
    MinHeap(const T arr[],const size_t arrSize)
    {
        m_capacity=arrSize;
        pData=new T[m_capacity];
        m_size = arrSize;
        for(int i=0; i<m_size; ++i)
        {
            pData[i] = arr[i];
        }

        //for(int i= (m_size-2)/2; i>=0;i--)
        for(size_t i=(m_size-2)/2+1;i>0;i--)
        {
            shift_down(i-1);
        }

        //int curPos = m_size/2 - 1;
        //while(curPos >= 0)
        //{
        //    shift_down(curPos,m_size-1);
        //    --curPos;
        //}
    }
    ~MinHeap()
    {
        delete[] pData;
        pData=NULL;
        m_capacity=m_size=0;
    }
    bool push(const T& val)
    {
        if(m_size >= m_capacity)
        {
            return false;
        }
        pData[m_size] = val;
        shift_up(m_size);
        ++m_size;
        return true;
    }
    bool pop(T &val)
    {
        if(m_size <= 0)
        {
            return false;
        }
        val = pData[0];
        pData[0] = pData[m_size-1];
        --m_size;
        //shift_down(0, m_size-1);
        shift_down(0);
        return true;
    }

    const T & top() const
    {
        if (m_size!=0)
        {
            return pData[0];
        }
        else
        {
            return T();  //失败抛异常或返回空值
        }
    }
    bool IsEmpty() const{ return m_size==0;}
    bool IsFull() const{ return m_size==m_capacity;}
    size_t Size() const{ return m_size;}
    size_t Capacity() const{ return m_capacity;}
    void print() const
    {
        for(size_t i=0; i<m_size; ++i)
        {
            std::cout << pData[i] << " ";
        }
        std::cout << std::endl;
    }
protected:

    //void shift_down(size_t  start,size_t  end)   //
    //{
    //    size_t  i = start;
    //    size_t  j = i*2 + 1;
    //    T tmp = pData[i];
    //    while(j <=end)
    //    {
    //        if(j+1 <= end && pData[j] > pData[j+1])   //最小堆，最大堆差别
    //        {
    //            ++j;
    //        }
    //        if(tmp >pData[j])    //最小堆，最大堆差别
    //        {
    //            pData[i] = pData[j];
    //        }
    //        else
    //        {
    //            break;
    //        }
    //        i = j;
    //        j = j*2 + 1;
    //    }
    //    pData[i] = tmp;
    //}
    //void shift_up(size_t  start)
    //{
    //    size_t  j = start;
    //    size_t  i = (start-1) / 2;
    //    T tmp = pData[j];
    //    while(j > 0)
    //    {
    //        if(tmp > pData[i])    //最小堆，最大堆差别
    //        {
    //            break;
    //        }
    //        else
    //        {
    //            pData[j] = pData[i];
    //        }
    //        j = i;
    //        i = (i-1) / 2;
    //    }
    //    pData[j] = tmp;
    //}

    void shift_up(size_t  curr)
    {
        //只要不是根节点 curr一开始=0则不运算
        while(curr!=0)
        {
            size_t posParent=parent(curr);
            if(pData[curr]>=pData[posParent])     //最大堆,小于等于则不交换。
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

        while(!isLeaf(pos,this->Size()))
        {
            size_t posL=leftchild(pos);
            size_t posR=rightchild(pos);
            size_t posMaxLR=posL;

            //Means pos has right child 有右子节点
            if(posR<m_size)
            {
                if(pData[posR]<pData[posL])   //取子树中小的下标
                {
                    posMaxLR=posR;//posL=posR;
                }
            }
            if(pData[pos]<=pData[posMaxLR])	//子树中大的与pos处进行比较，大于则退出
            {
                return;
            }
            Swap(pData[pos],pData[posMaxLR]);     //交互当前节点与其最大子节点的值
            pos=posMaxLR;
        }
    }
};





//基于仿函数，最大堆/最小堆共用代码

#include<vector>

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



//用了仿函数的方式去复用最大堆和最小堆的代码。在这里默认把堆调整为最大堆。
//template<class T, template<class> class Continer = Less>   //声明方式1


//此实现可能有问题，会无止尽的扩容
//在改进，可以根据设置的capticy值扩容。有空待续。


//声明方式1
//template<class T,template<class> class Continer = Greater>//默认为大堆 
//Continer<T> _con;  //内部成员

//声明方式2
//template<class T,class Cmp>

template<class T,template<class T> class CmpFun=Less>
class Heap
{
public:
    Heap(){}
    Heap(const T* arr,size_t size)
    {
        Listdata.reserve(size);
        for (size_t i = 0; i < size; ++i)
        {
            Listdata.push_back(arr[i]);
        }
        //int root=(Listdata.size()-2)/2;//找到第一个非叶子结点
        //while (root>=0)
        //{
        //     shift_down(root--);  //向下调整
        //}

        //for(int i= (m_size-2)/2; i>=0;i--)
        for(size_t i=(Size()-2)/2+1;i>0;i--)
        {
            shift_down(i-1);
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
        //--m_size;
        //shift_down(0, m_size-1);

        shift_down(0);
        return true;
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
    void AdjustUpV2(size_t child)
    {
        T* pData=Listdata.data();
        size_t parent = (child - 1) / 2;
        while (child > 0)
        {
            if (Cmp(pData[child],pData[parent]))
            {
                swap(pData[child],pData[parent]);
                child = parent;
                parent = (child - 1) / 2;
            }
            else
            {
                break;
            }
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
                swap(pData[parent],pData[child]);
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
    std::vector<T> Listdata;    //去掉vector依赖？暂留，可以自动扩容
    CmpFun<T> Cmp;
};

int Test_Heap()
{
    int array[] = {23, 9, 34, 2, 56, 89, 35, 54,201,307,7,405,18,563,627,24,711,1,822,911};
    const int arraySize=sizeof(array)/sizeof(array[0]);

    {
        MaxHeap<int> heap(array,arraySize);
        cout << "***************************MaxHeap**************************"<<endl; 
        heap.print();
        heap.push(99);                 //满失败
        heap.pushCover(999);      //强制插入
        heap.print();
        for(size_t i=0; i<arraySize; ++i)
        {
            int val;
            heap.pop(val);
            std::cout <<val<< " ";
        }
        std::cout <<std::endl;
    }


    {
        std::cout <<"***************************MinHeap***************************"<<std::endl;
        MinHeap<int> heap(array,arraySize);

        heap.print();
        heap.push(1);
        heap.print();
        for(size_t i=0; i<arraySize; ++i)
        {
            int val;
            heap.pop(val);
            std::cout << val << " ";
        }
        std::cout<<std::endl;
    }


    {
        std::cout<<"***************************MaxHeap2***************************"<<std::endl;
        //Heap<int,Greater<int> > heap(array,arraySize);
        Heap<int,Greater> heap(array,arraySize);
        heap.push(777);
        heap.push(3);
        heap.push(1100);
        while (!heap.IsEmpty())
        {
            std::cout <<heap.top()<< " ";
            heap.pop();
        }
        std::cout<<std::endl;
    }

    {
        std::cout<<"***************************MinHeap2***************************"<<std::endl;
        //Heap<int,Less<int> > heap2(array,arraySize);
        Heap<int,Less> heap2(array,arraySize);
        heap2.push(777);
        heap2.push(3);
        heap2.push(1100);
        while(!heap2.IsEmpty())
        {
            std::cout <<heap2.top()<< " ";
            heap2.pop();
        }
        std::cout<<std::endl;
    }

    getchar(); //system("pause");不跨平台

    return 0;
}


#endif













#ifdef UNIT_TEST

int Test_Heap()
{
    int array[] = {23, 9, 34, 2, 56, 89, 35, 9,9,9,9,9,54,201,307,7,405,18,563,627,5,5,5,5,24,711,1,822,911};
    const int arraySize=sizeof(array)/sizeof(array[0]);
    {
        std::cout<<"***************************MinHeap***************************"<<std::endl;
        //Heap<int,Less<int> > heap2(array,arraySize);
        Heap<int,Less> heap2(array,arraySize);
        heap2.push(777);
        heap2.push(3);
        heap2.push(1100);
        while(!heap2.IsEmpty())
        {
            std::cout <<heap2.top()<< " ";
            heap2.pop();
        }
        std::cout<<std::endl;

        heap2.push(array,arraySize);
        heap2.popPrint();

        heap2.push(array,arraySize);
        heap2.delValue(5);   //删除
        heap2.popPrint();
    }

    {
        std::cout<<"***************************MaxHeap***************************"<<std::endl;
        //Heap<int,Greater<int> > heap(array,arraySize);
        Heap<int,Greater> heap(array,arraySize);
        heap.push(777);
        heap.push(3);
        heap.push(1100);
        while (!heap.IsEmpty())
        {
            std::cout <<heap.top()<< " ";
            heap.pop();
        }
        std::cout<<std::endl;

        heap.push(array,arraySize);
        heap.popPrint();
        heap.push(array,arraySize);
        heap.delValue(5);
        heap.popPrint();
    }

    getchar(); //system("pause");不跨平台
    return 0;
}


#endif

