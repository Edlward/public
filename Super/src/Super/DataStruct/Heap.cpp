#include "Heap.h"

#include <iostream>
using namespace std;
 

#if 0
//creator by lizulin

//�� heap
//��Ҳ�����ȶ��У�����һ���������ȫ���������ݽ�
//�ѷ�Ϊ���֣����ѣ���С�ѡ�
//���ѣ����ڵ�������������ӽڵ����ȫ������
//��С�ѣ����ڵ�С�����������ӽڵ����ȫ������
//�ѿ������������洢��

//�����±�i��0��ʼ,i=0�Ǹ��ڵ㡣
//a[i]������ڵ㣬a[2 * i+1]���������ĸ��ڵ� ; a[2 * i + 2]���������ĸ��ڵ㡣

//    ���ѵĲ��룺ʱ�临�Ӷ�O(logn)
//    �϶�������ĩβ��������֣�Ȼ��͸��ڵ���бȽ�,Ҳ���ǽڵ��ϸ�������
//
//    ���ѵ�ɾ����ʱ�临�Ӷ�O(logn)
//    ɾ�����ڵ�Ԫ�أ�����������һ��Ԫ�������棬Ȼ����нڵ��³���


//TODO:
//���ڽṹ�壬�����Զ���ȽϺ��� ������
//����/��С�Ѳ�ȫ�������趨, �ݹ�/�쳣��


#if 0
//�ݹ�д�� �ο�
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
//�����ԣ��ݹ�д����Ԫ�ؽ��������ȷǵݹ�д���ࡣ
#endif


//new add 
//�Ƿ��ǵײ�Ҷ�ӽڵ�   �ڵ�0��1��� size=1��pos=0��
static inline bool isLeaf(size_t  pos,size_t sizeHeap)
{
    return (pos>=sizeHeap/2)&&(pos<sizeHeap);
    // return (pos!=0)&&(pos>=m_size/2)&&(pos<m_size);
}

//��ڵ��±�
static inline size_t leftchild(size_t pos)
{ 
    return 2*pos+1;
}

//�ұ߽ڵ��±�
static inline size_t  rightchild(size_t pos)
{ 
    return 2*pos+2;
}
//���ڵ��±�  pos=0����������������ʱ
static inline size_t parent(size_t pos)
{ 
    return (pos-1)/2;
}


//����
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
        //int curPos=m_size/2-1;     //m_size����0���ǣ�
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
            return T();  //ʧ�����쳣�򷵻ؿ�ֵ--�������� ������
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
    //���϶��¹淶���еĸ�ʽ���������Ѷ���
    //void shift_down(size_t  start,size_t  end)  //�ϲ�����
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
        //ֻҪ���Ǹ��ڵ� currһ��ʼ=0������
        while(curr!=0)
        {
            size_t posParent=parent(curr);
            if(pData[curr]<=pData[posParent])     //����,С�ڵ����򲻽�����
            {
                break;
            }
            Swap(pData[curr],pData[posParent]);   //���ڵ������˳�
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

            //Means pos has right child �����ӽڵ�
            if(posR<m_size)
            {
                if(pData[posR]>pData[posL])   //ȡ�����д���±�
                {
                    posMaxLR=posR;//posL=posR;
                }
            }
            if(pData[pos]>=pData[posMaxLR])	//�����д����pos�����бȽϣ��������˳�
            {
                return;
            }
            Swap(pData[pos],pData[posMaxLR]);     //������ǰ�ڵ���������ӽڵ��ֵ
            pos=posMaxLR;
        }
    }
};






//��С��
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
            return T();  //ʧ�����쳣�򷵻ؿ�ֵ
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
    //        if(j+1 <= end && pData[j] > pData[j+1])   //��С�ѣ����Ѳ��
    //        {
    //            ++j;
    //        }
    //        if(tmp >pData[j])    //��С�ѣ����Ѳ��
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
    //        if(tmp > pData[i])    //��С�ѣ����Ѳ��
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
        //ֻҪ���Ǹ��ڵ� currһ��ʼ=0������
        while(curr!=0)
        {
            size_t posParent=parent(curr);
            if(pData[curr]>=pData[posParent])     //����,С�ڵ����򲻽�����
            {
                break;
            }
            Swap(pData[curr],pData[posParent]);   //���ڵ������˳�
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

            //Means pos has right child �����ӽڵ�
            if(posR<m_size)
            {
                if(pData[posR]<pData[posL])   //ȡ������С���±�
                {
                    posMaxLR=posR;//posL=posR;
                }
            }
            if(pData[pos]<=pData[posMaxLR])	//�����д����pos�����бȽϣ��������˳�
            {
                return;
            }
            Swap(pData[pos],pData[posMaxLR]);     //������ǰ�ڵ���������ӽڵ��ֵ
            pos=posMaxLR;
        }
    }
};





//���ڷº���������/��С�ѹ��ô���

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



//���˷º����ķ�ʽȥ�������Ѻ���С�ѵĴ��롣������Ĭ�ϰѶѵ���Ϊ���ѡ�
//template<class T, template<class> class Continer = Less>   //������ʽ1


//��ʵ�ֿ��������⣬����ֹ��������
//�ڸĽ������Ը������õ�capticyֵ���ݡ��пմ�����


//������ʽ1
//template<class T,template<class> class Continer = Greater>//Ĭ��Ϊ��� 
//Continer<T> _con;  //�ڲ���Ա

//������ʽ2
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
        //int root=(Listdata.size()-2)/2;//�ҵ���һ����Ҷ�ӽ��
        //while (root>=0)
        //{
        //     shift_down(root--);  //���µ���
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
        val = pData[0];                               //ȡ��ͷ,
        pData[0] = pData[this->Size()-1];   //β���ŵ�ͷ��


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
        //ֻҪ���Ǹ��ڵ� currһ��ʼ=0������
        while(curr!=0)
        {
            size_t posParent=parent(curr);
            if(Cmp(pData[posParent],pData[curr]))   //���ݱȽϺ�����ͬ,��������,��С�ѡ�
            {
                break;
            }
            Swap(pData[curr],pData[posParent]);   //���ڵ������˳�
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
            size_t posSeclect=posL;    //ѡ�е���С ���ڵ���š�

            //Means pos has right child �����ӽڵ�
            if(posR<this->Size())
            {
                if(Cmp(pData[posR],pData[posL]))   //ȡ������С��ڵ���±�
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
    //�ϲ�����
    void AdjustDownV2(size_t parent)
    {
        T* pData=Listdata.data();

        size_t child = parent * 2 + 1;
        size_t size =Listdata.size();
        while (child < size)
        {
            if (child + 1 < size&&_con(pData[child+1],pData[child]))
            {
                //ע���������child+1������С�����԰�child+1����ǰ��
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
    std::vector<T> Listdata;    //ȥ��vector�����������������Զ�����
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
        heap.push(99);                 //��ʧ��
        heap.pushCover(999);      //ǿ�Ʋ���
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

    getchar(); //system("pause");����ƽ̨

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
        heap2.delValue(5);   //ɾ��
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

    getchar(); //system("pause");����ƽ̨
    return 0;
}


#endif

