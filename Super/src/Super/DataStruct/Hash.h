#ifndef HASH_H
#define HASH_H

//hash������ʵ�֣�����ַ��(������/��ϣͰ)������ַ��--�пռ������ƣ�������������

/*
��ɢ�м����У���ͻ��ָ���ڹؼ��ּ����е������ؼ���ֵKi��Kj����Ki != Kjʱ��
��h(Ki)==h(Kj)��h��ɢ�к����� 
Ŀǰ�Ƚϳ��õ�ɢ�к����У�������������ƽ��ȡ�з����۵��������ַ������� 
һ��ɢ�б��г�ͻ�����Ա���ģ�����ҪѰ������ͻ�ķ������������Ϳ���ַ����
���ֳ��õķ����������ķ���Ҳ�ƿ�ɢ�з�������ַ���ֳƱ�ɢ�з��� 

����������Ϊÿ��ɢ�е�ַ����һ�����������д洢���о��и�ɢ��ֵ��Ԫ�ء� 
����ַ�����ڳ�ͻ����ʱ��ͨ��һ�ֲ�����̽����һ����λ�ã�����������̽�鷨��α���̽�鷨������̽�鷨��˫ɢ�з��� 
*/


#ifdef __cplusplus

#include<list>  
#include<vector>  

//����ַ��hashTable
//���Ĳο��ˡ����ݽṹ���㷨����C++��������3�棩��[��] Mark Weiss ��       �Ż��� ����     ����   ��У
//using namespace std;  
//refactor by lizulin ������ַֻ�����Ǵ���0�ģ���unsigned hash�������Ҳ����unsigned��

template <typename T>  
class HashTableByList  
{  
    friend unsigned int hash(const T& x);    //��ͬ��ʵ����hash ����
private:  
    int currentSize;                                      //ɢ�б��е�ǰ��ŵ�Ԫ�ظ���  by lizulin
    std::vector<std::list<T> > theLists;  
    unsigned int myhash(const T& x) const  //�Բ�ͬ���͵����ݿ����ò�ͬ��hash�������غ���
    {  
        //return hash(x);  
        return hash(x)%theLists.size();  
    }
    //unsigned int hash(int x)//��int���͵�hash������������hash����Ϊ�򵥵�ȡ�ຯ��  
    //{  
    //    //return x%theLists.size();  
    //    return x; 
    //}
public:  
    HashTableByList(size_t size = 11)    //���캯����Ĭ�ϳ�ʼ������Ϊ11  
        :theLists(size),currentSize(0)
    {
        //currentSize=0;
        //���ݹ�ϣ��Ĵ�С����vector�Ŀռ�
        //theLists.reserve(size);  //add by lizuiln
        //theLists.resize(size);
    }
    ~HashTableByList()//�������� 
    { 
        makeEmpty();  
    }  
    bool isEmpty() const
    {
        return currentSize == 0;
    }
    void makeEmpty()//�����������
    {  
        for(size_t i = 0;i < theLists.size();i++)  
        {
            theLists.at(i).clear();  
        }
        currentSize=0;
    }  

    bool contain(const T& x) const //����Ԫ��
    {  
        unsigned int iHashValue = myhash(x);
        std::list<T> ali=theLists.at(iHashValue);  
        std::list<T>::iterator it=ali.begin();  
        for(;it != ali.end();it++)
        {  
            if (*it == x)  
                return true;  
        }  
        return false;  
        //д��2
        //const list<T> & whichList = theLists[myhash(x)];
        //return find(whichList.begin(), whichList.end(), x) != whichList.end();
    }

    //Ϊʲô��ô��ƣ�
    //��������: ����Ԫ��x ���ؽ��������ҵ��򷵻ظ�Ԫ�أ����򷵻�һ��Ĭ�Ϲ����Ԫ��ֵ
    T findElement(const T & x) const
    {
        const std::list<T>& whichList = theLists[myhash(x)];
        typename std::list<T>::const_iterator it = find(whichList.begin(), whichList.end(), x);
        if(it != whichList.end())
        {
            return *it;
        }
        else
        {
            T obj;//����һ����ԱֵΪ0�Ķ���
            return obj;
        }
    }

    //void insert(T x)//����Ԫ��
    //{  
    //    theLists.at(myhash(x)).push_back(x);  
    //}  

    //�������ܣ���hash���в���Ԫ�أ����Ԫ���Ѿ������򷵻�false�������������������ǰ�����
    bool insert(const T& x)
    {
        int iHashValue = myhash(x);
        std::list<T>& tempList = theLists[iHashValue];
        if (contain(x))
        {
            return false;   //�Ѿ����ڷ���false
        }

        //rehash
        if(++currentSize > theLists.size())
        {
            rehash();//�����Ĵ�С
        }

        tempList.push_back(x);
        return true;
    }

    //add by lizulin--������ key �����ݷ���
    //�������ܣ���hash���в���Ԫ�أ����Ԫ���Ѿ������򷵻�false�������������������ǰ�����
    template<typename Tkey>
    bool insert(const Tkey& key,const T& x)
    {
        int iHashValue = myhash(key);
        std::list<T>& tempList = theLists[iHashValue];
        if (contain(x))
        {
            return false;   //�Ѿ����ڷ���false
        }

        //rehash
        if(++currentSize > theLists.size())
        {
            rehash();//�����Ĵ�С
        }

        tempList.push_back(x);
        return true;
    }

    bool remove(const T& x)//ɾ��Ԫ��  
    {
       /*     
       std::list<int>*ali=&theLists.at(myhash(x));  
        std::list<int>::iterator ir=ali->begin();  
        for(;ir!=ali->end();ir++)
        {  
            if(*ir == x)
            {  
                ir=ali->erase(ir);  
            }  
        }
        */  

        //new 
        std::list<T> & whichList = theLists[myhash(x)];
        typename std::list<T>::iterator it = find(whichList.begin(), whichList.end(), x);
        if(it == whichList.end())
            return false;

        whichList.erase(it);//ɾ��Ԫ��x

        --currentSize;
        return true;
    }  


    //��������: ��þ���n�����һ������ 
    unsigned int nextPrime(unsigned int n) const
    {
        unsigned int i;
        if(n % 2 == 0)
            n++;

        for(; ; n += 2)
        {
            for(i = 3; i*i <= n; i += 2)
            {
                if(n % i == 0)
                {
                    goto ContOuter;
                }
            }
            return n;
           ContOuter: ;
        }
    }

    //��������
    void rehash()//��ɢ��  
    {  
        std::vector<std::list<T> > oldLists=theLists;  
        //����һ���µĴ�СΪԭ��������С��ɢ�б�
        theLists.resize(nextPrime(2 * theLists.size()));   //old theLists.resize(2* theLists.size() + 1);    new bylizulin
         
        for(int j =0;j<theLists.size();j++)  
        {
            theLists[j].clear();  
        }
        for(int i =0; i<oldLists.size( );i++)  
        {  
            std::list<T>::iterator it=oldLists[i].begin();  
            while(it!=oldLists[i].end())  
            {  
                insert(*it);  
                it++;  
            }  
        }  
    }
    void print() const //��ʾ��ϣ���Ԫ�ؽṹ
    {  
        for(size_t i=0;i<theLists.size();i++)
        {  
            //std::cout<<"��"<<i<<"������";  
            if(theLists.at(i).empty())  
            {
                //cout<<"(empty)"<<endl;  
            }
            else
            {  
                std::list<T>::const_iterator it=theLists.at(i).begin();  
                //std::cout << "[ " << *it++;  
                while( it != theLists.at(i).end())  
                {
                    //std::cout << ", " << *it++;  
                }
                //std::cout << " ]" << endl;  
            }  
        }  
    } 
};  








//����ַ��/���Ŷ�ַ��ʵ��

template<typename T>
class HashTableByOpenAaddressing
{
public:
    explicit HashTableByOpenAaddressing(size_t size = 11):array(nextPrime(size)), currentSize(0)
    {
        makeEmpty();
    }
    ~HashTableByOpenAaddressing();
    enum EntryType {ACTIVE,EMPTY,DELETED};//ÿ�����ݵ�Ԫ����һ��info������������λ���Ƿ�ռ�á��ջ���ɾ��
private:
    //ɢ�б�����ݵ�Ԫ�ṹ
    struct HashEntry
    {
        T element;            //��ɢ�б��ŵ�����
        EntryType info;    //������λ�õ�״̬ 
        HashEntry(const T & e = T(), EntryType i = EMPTY):element(e), info(i){}
    };
    std::vector<HashEntry> array;     //ɢ�б�
    int currentSize;                            //ɢ�б��е�ǰ��ŵ�Ԫ�ظ���
public:
    //friend std::ostream & operator<<(const std::ostream & os, const HashEntry & e)
    friend std::ostream& operator<<(std::ostream & os, const HashEntry & e)
    {
       // std::cout << "element: " << e.element << ", info = " << e.info;
        return os;
    }
    //Ҫ����Ԫ��x ����ҵ��򷵻ظ�Ԫ�ص�����
    T findElement(const T & x)
    {
        int currentPos = findPos(x);
        if(isActive(currentPos))
        {
            return array[currentPos].element;
        }
        else
        {
            //û���ҵ�,����һ����ֵ?  ����
            T obj;
            return obj;
        }
    }

    /****************************************************************
    *   �������ƣ�findPos(const HashedObj & x) const
    *   ��������: ����xӦ�ò����λ��
    *   �����б�: x��Ҫ�����Ԫ��
    *   ���ؽ��������ҵ��յ�λ���򷵻�Ҫ�����λ�ñ��
    *****************************************************************/
    size_t findPos(const T & x)
    {
        //����̽��f(i) = i; f(i) = f(i-1) + 1;���Ϊ1
        //ƽ��̽��f(i) = i*i; f(i) = f(i-1) + 2*i - 1; ���Ϊ2*i-1
        //˫ɢ�У�f(i) = i*hash2(x); f(i) = f(i-1)+hash2(x);���Ϊhash2(x);
        //hash2(x) = R-(x%R); R=prePrime(array.size()),RΪС��TableSize()������
        int offset = 1;
        unsigned int currentPos = myhash(x);

        //����ҵ��˿յ�λ���򷵻�λ�ñ��
        //����ҵ��˸�Ԫ��x���򷵻ظ�Ԫ�ص�λ�ñ��
        while(array[currentPos].info!=EMPTY&& array[currentPos].element !=x)
        {
            //currentPos += offset;//����̽��
            currentPos += 2 * offset -1;//ƽ��̽��
            //currentPos += prePrime(array.size()) - myhash(x)%prePrime(array.size());//˫ɢ��
            offset++;
            if(currentPos >= array.size())
            {
                currentPos -= array.size();
            }
        }
        return currentPos;
    }

   //��������: ���ɢ�б��е�����
    void print()
    {
        std::cout << "���ɢ�б��е�����: " << std::endl;
        for(unsigned i = 0; i < array.size(); ++i)
        {
            if(isActive(i))
                std::cout << i << ": " << std::endl << array[i] << std::endl;
        }
    }

    //��������: �ж�ɢ�б��Ƿ�Ϊ��
    bool isEmpty()
    {
        return currentSize == 0;
    }

    //��������: ���ɢ�б� 
    void makeEmpty()
    {
        for(int i = 0; i < array.size(); ++i)
        {
            array[i].info = EMPTY;
        }
        currentSize = 0;//��ǰԪ�ظ�����Ϊ0
    }

    //�������ƣ�containes(const HashedObj & x) const
    //��������: �ж�ɢ�б��Ƿ����ֵΪx��Ԫ�� 
    bool contain(const T & x)
    {
        //findPos(x)���ص�λ����ACTIVE��˵�����ڸ�Ԫ��x
        return isActive(findPos(x));
    }

   //��������: ɾ��ɢ�б��е�ֵΪx��Ԫ�� �ɹ�ɾ������true,���򷵻�false
    bool remove(const T & x)
    {
        int currentPos = findPos(x);
        if(!isActive(currentPos))
        {
            return false;
        }
        array[currentPos].info = DELETED;//����ɾ������������ʶλinfo����ΪDeleted
        --currentSize;
        return true;
    }

    //����: ��ɢ�б��в���Ԫ��x,����������Ѿ����ڣ���ʲô��������
    //��������ڱ��ǰ�� ���ؽ��������ɹ�����true, ���򷵻�false
    bool insert(const T & x)
    {
        int currentPos = findPos(x);//���ҵ�λ��
        if(isActive(currentPos))//�����λ�ô��Ѿ�����˸�Ԫ�أ���֮�䷵��false
        {
            return false;
        }
        array[currentPos] = HashEntry(x, ACTIVE);
        //�����ǰɢ�б���Ԫ�صĸ�������ɢ�б��ȵ�һ�룬������ɢ�б�Ϊԭ����2��
        if(++currentSize > array.size()/2)
        {
            rehash();//�����Ĵ�С
        }
        return true;
    }

private:

   //��������: ��þ���n�����һ��С��n������ 
    unsigned int prePrime(unsigned int n)
    {
        unsigned int i;
        if(n % 2 == 0)
            n--;

        for(; ; n -= 2)
        {
            for(i = 3; i*i <= n; i += 2)
            {
                if(n % i == 0)
                {
                    goto ContOuter;
                }
            }
            return n;
            ContOuter: ;
        }
    }

    //��������: ��þ���n�����һ������n������ 
    unsigned int nextPrime(unsigned int n)
    {
        unsigned  int i;
        if(n % 2 == 0)
            n++;

        for(; ; n += 2)
        {
            for(i = 3; i*i <= n; i += 2)
            {
                if(n % i == 0)
                {
                    goto ContOuter;
                }
            }
            return n;
            ContOuter: ;
        }
    }

    //��������: �ж�λ��currentPos�����Ƿ���Ԫ�� ��Ԫ���򷵻�true,���򷵻�false
    bool isActive(int currentPos) const
    {
        return array[currentPos].info == ACTIVE;
    }
    unsigned int myhash(const T & key) const
    {
        return hash(key)%array.size();
    }
    //��������: ����ɢ�б�Ĵ�С
    void rehash()
    {
        std::vector<HashEntry> oldArray = array;
        //����һ���µĴ�СΪԭ��������С��ɢ�б�
        array.resize(nextPrime(2 * oldArray.size()));
        for(int i = 0; i < array.size(); ++i)
        {
            array[i].info = EMPTY;
        }
        currentSize = 0;
        //����ɢ�б�
        for(int i = 0; i < oldArray.size(); ++i)
        {
            if(oldArray[i].info == ACTIVE)
            {
                insert(oldArray[i].element);
            }
        }
    }
};












#endif



 

 














#ifdef UNIT_TEST
int Test_Hash();
#endif


#endif // HASH_H
