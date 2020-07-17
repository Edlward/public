#ifndef _Heap_Header_
#define _Heap_Header_

#include "../Basic/global.h"

//creator by lizulin 

//�� heap
//��Ҳ�����ȶ��У�����һ���������ȫ���������ݽ�
//�ѷ�Ϊ���֣����ѣ���С�ѡ�
//���ѣ����ڵ�������������ӽڵ����ȫ������
//��С�ѣ����ڵ�С�����������ӽڵ����ȫ������
//�ѿ������������洢��

//�����±�i��0��ʼ,i=0�Ǹ��ڵ㡣
//a[i]������ڵ㣬a[2 * i+1]���������ĸ��ڵ� ; a[2 * i + 2]���������ĸ��ڵ㡣

//���ѵĲ��룺ʱ�临�Ӷ�O(logn)
//�϶�������ĩβ��������֣�Ȼ��͸��ڵ���бȽ�,Ҳ���ǽڵ��ϸ�������
//
//���ѵ�ɾ����ʱ�临�Ӷ�O(logn)
//ɾ�����ڵ�Ԫ�أ�����������һ��Ԫ�������棬Ȼ����нڵ��³���


//TODO:
//���ڽṹ�壬�����Զ���ȽϺ��� ������
//����/��С�Ѳ�ȫ�������趨, �ݹ�/�쳣��


//�ȽϺ���
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


//���˷º����ķ�ʽȥ�������Ѻ���С�ѵĴ��롣������Ĭ�ϰѶѵ���Ϊ��С�ѡ�
//template<class T, template<class> class Continer = Less>   //������ʽ1

//������ʽ1
//template<class T,template<class> class Continer = Greater>//Ĭ��Ϊ��� 
//Continer<T> _con;  //�ڲ���Ա

//������ʽ2
//template<class T,class Cmp>

#include<vector>  //��ʹ��vector
#include<type_traits>

template<class T,template<class T> class CmpFun=Less>
class Heap
{
    //to be confirm
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
public:
    Heap(){}
    Heap(const T* arr,size_t size)
    {
        Listdata.reserve(size);
        for (size_t n = 0; n <size; ++n)
        {
            Listdata.push_back(arr[n]);
        }
        //int root=(Listdata.size()-2)/2;//�ҵ���һ����Ҷ�ӽ��
        //while (root>=0)
        //{
        //     shift_down(root--);  //���µ���
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
    bool push(const T & t,size_t maxSize)  //=UINT_MAX �������������Ͱ汾
    {
        if (Size()>=maxSize)
        {
            return false;
        }
        Listdata.push_back(t);
        shift_up(Listdata.size()-1);
        return true;
    }
    bool push(const T* arr,size_t size)  //ͬʱ���Ͷ��Ԫ�ذ汾
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
        val = pData[0];                               //ȡ��ͷ,
        pData[0] = pData[this->Size()-1];   //β���ŵ�ͷ��
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

    //�Ƴ�����ĳ��ֵ�İ汾  �ҵ���ɾ������true,����Ϊfalse
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
                    pData[i] = pData[this->Size()-1];   //β���ŵ��˴�
                    Listdata.pop_back();   //ɾȥβ��Ԫ��

                    //ɾ��Ԫ�ض������Ѻ���С�Ѳ�����ͬ
                    if (std::is_same<Less<T>,CmpFun<T>>::value)   //��С��
                    {
                         shift_up(i);
                    }
                    else if(std::is_same<Greater<T>,CmpFun<T>>::value)  //����
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
                //ע���������child+1������С�����԰�child+1����ǰ��
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
    //std::vector<T> Listdata;  //ȥ��vector�����������������Զ�����
    SArray<T> Listdata;         //���Զ�������,�����������ÿռ�
    CmpFun<T> Cmp;
};


#ifdef UNIT_TEST
int Test_Heap();
#endif

#endif

