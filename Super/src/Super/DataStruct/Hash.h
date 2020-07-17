#ifndef HASH_H
#define HASH_H

//hash表容器实现，链地址法(拉链法/哈希桶)、开地址法--有空继续完善，包括迭代器等

/*
在散列技术中，冲突是指对于关键字集合中的两个关键字值Ki和Kj，当Ki != Kj时，
有h(Ki)==h(Kj)，h是散列函数。 
目前比较常用的散列函数有：除留余数法、平方取中法、折叠法、数字分析法。 
一个散列表中冲突是难以避免的，所以要寻求解决冲突的方法。拉链法和开地址法是
两种常用的方法。拉链的方法也称开散列法，开地址法又称闭散列法。 

拉链法就是为每个散列地址建立一个单链表，表中存储所有具有该散列值的元素。 
开地址法是在冲突发生时，通过一种策略来探查下一个空位置，比如有线性探查法、伪随机探查法、二次探查法和双散列法。 
*/


#ifdef __cplusplus

#include<list>  
#include<vector>  

//链地址法hashTable
//本文参考了《数据结构与算法分析C++描述（第3版）》[美] Mark Weiss 著       张怀勇 等译     刘田   审校
//using namespace std;  
//refactor by lizulin 容器地址只可能是大于0的，改unsigned hash函数最好也返回unsigned的

template <typename T>  
class HashTableByList  
{  
    friend unsigned int hash(const T& x);    //不同类实现其hash 函数
private:  
    int currentSize;                                      //散列表中当前存放的元素个数  by lizulin
    std::vector<std::list<T> > theLists;  
    unsigned int myhash(const T& x) const  //对不同类型的数据可以用不同的hash函数重载函数
    {  
        //return hash(x);  
        return hash(x)%theLists.size();  
    }
    //unsigned int hash(int x)//对int类型的hash函数，在这里hash函数为简单的取余函数  
    //{  
    //    //return x%theLists.size();  
    //    return x; 
    //}
public:  
    HashTableByList(size_t size = 11)    //构造函数，默认初始链表数为11  
        :theLists(size),currentSize(0)
    {
        //currentSize=0;
        //根据哈希表的大小分配vector的空间
        //theLists.reserve(size);  //add by lizuiln
        //theLists.resize(size);
    }
    ~HashTableByList()//析构函数 
    { 
        makeEmpty();  
    }  
    bool isEmpty() const
    {
        return currentSize == 0;
    }
    void makeEmpty()//清空所有链表
    {  
        for(size_t i = 0;i < theLists.size();i++)  
        {
            theLists.at(i).clear();  
        }
        currentSize=0;
    }  

    bool contain(const T& x) const //查找元素
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
        //写法2
        //const list<T> & whichList = theLists[myhash(x)];
        //return find(whichList.begin(), whichList.end(), x) != whichList.end();
    }

    //为什么这么设计？
    //功能描述: 查找元素x 返回结果：如果找到则返回该元素，否则返回一个默认构造的元素值
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
            T obj;//返回一个成员值为0的对象
            return obj;
        }
    }

    //void insert(T x)//插入元素
    //{  
    //    theLists.at(myhash(x)).push_back(x);  
    //}  

    //函数功能：向hash表中插入元素，如果元素已经存在则返回false，不存在则在链表的最前面插入
    bool insert(const T& x)
    {
        int iHashValue = myhash(x);
        std::list<T>& tempList = theLists[iHashValue];
        if (contain(x))
        {
            return false;   //已经存在返回false
        }

        //rehash
        if(++currentSize > theLists.size())
        {
            rehash();//扩充表的大小
        }

        tempList.push_back(x);
        return true;
    }

    //add by lizulin--待完善 key 与数据分离
    //函数功能：向hash表中插入元素，如果元素已经存在则返回false，不存在则在链表的最前面插入
    template<typename Tkey>
    bool insert(const Tkey& key,const T& x)
    {
        int iHashValue = myhash(key);
        std::list<T>& tempList = theLists[iHashValue];
        if (contain(x))
        {
            return false;   //已经存在返回false
        }

        //rehash
        if(++currentSize > theLists.size())
        {
            rehash();//扩充表的大小
        }

        tempList.push_back(x);
        return true;
    }

    bool remove(const T& x)//删除元素  
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

        whichList.erase(it);//删除元素x

        --currentSize;
        return true;
    }  


    //功能描述: 获得距离n最近的一个素数 
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

    //扩充容量
    void rehash()//再散列  
    {  
        std::vector<std::list<T> > oldLists=theLists;  
        //创建一个新的大小为原来两倍大小的散列表
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
    void print() const //显示哈希表的元素结构
    {  
        for(size_t i=0;i<theLists.size();i++)
        {  
            //std::cout<<"第"<<i<<"条链表：";  
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








//开地址法/开放定址法实现

template<typename T>
class HashTableByOpenAaddressing
{
public:
    explicit HashTableByOpenAaddressing(size_t size = 11):array(nextPrime(size)), currentSize(0)
    {
        makeEmpty();
    }
    ~HashTableByOpenAaddressing();
    enum EntryType {ACTIVE,EMPTY,DELETED};//每个数据单元都有一个info变量，表明该位置是否被占用、空或已删除
private:
    //散列表的数据单元结构
    struct HashEntry
    {
        T element;            //该散列表存放的数据
        EntryType info;    //表明该位置的状态 
        HashEntry(const T & e = T(), EntryType i = EMPTY):element(e), info(i){}
    };
    std::vector<HashEntry> array;     //散列表
    int currentSize;                            //散列表中当前存放的元素个数
public:
    //friend std::ostream & operator<<(const std::ostream & os, const HashEntry & e)
    friend std::ostream& operator<<(std::ostream & os, const HashEntry & e)
    {
       // std::cout << "element: " << e.element << ", info = " << e.info;
        return os;
    }
    //要查找元素x 如果找到则返回该元素的引用
    T findElement(const T & x)
    {
        int currentPos = findPos(x);
        if(isActive(currentPos))
        {
            return array[currentPos].element;
        }
        else
        {
            //没有找到,返回一个空值?  意义
            T obj;
            return obj;
        }
    }

    /****************************************************************
    *   函数名称：findPos(const HashedObj & x) const
    *   功能描述: 查找x应该插入的位置
    *   参数列表: x是要插入的元素
    *   返回结果：如果找到空的位置则返回要插入的位置标号
    *****************************************************************/
    size_t findPos(const T & x)
    {
        //线性探测f(i) = i; f(i) = f(i-1) + 1;相隔为1
        //平方探测f(i) = i*i; f(i) = f(i-1) + 2*i - 1; 相隔为2*i-1
        //双散列，f(i) = i*hash2(x); f(i) = f(i-1)+hash2(x);相隔为hash2(x);
        //hash2(x) = R-(x%R); R=prePrime(array.size()),R为小于TableSize()的素数
        int offset = 1;
        unsigned int currentPos = myhash(x);

        //如果找到了空的位置则返回位置标号
        //如果找到了该元素x，则返回该元素的位置标号
        while(array[currentPos].info!=EMPTY&& array[currentPos].element !=x)
        {
            //currentPos += offset;//线性探测
            currentPos += 2 * offset -1;//平方探测
            //currentPos += prePrime(array.size()) - myhash(x)%prePrime(array.size());//双散列
            offset++;
            if(currentPos >= array.size())
            {
                currentPos -= array.size();
            }
        }
        return currentPos;
    }

   //功能描述: 输出散列表中的内容
    void print()
    {
        std::cout << "输出散列表中的内容: " << std::endl;
        for(unsigned i = 0; i < array.size(); ++i)
        {
            if(isActive(i))
                std::cout << i << ": " << std::endl << array[i] << std::endl;
        }
    }

    //功能描述: 判断散列表是否为空
    bool isEmpty()
    {
        return currentSize == 0;
    }

    //功能描述: 清空散列表 
    void makeEmpty()
    {
        for(int i = 0; i < array.size(); ++i)
        {
            array[i].info = EMPTY;
        }
        currentSize = 0;//当前元素个数设为0
    }

    //函数名称：containes(const HashedObj & x) const
    //功能描述: 判断散列表是否包含值为x的元素 
    bool contain(const T & x)
    {
        //findPos(x)返回的位置是ACTIVE的说明存在该元素x
        return isActive(findPos(x));
    }

   //功能描述: 删除散列表中的值为x的元素 成功删除返回true,否则返回false
    bool remove(const T & x)
    {
        int currentPos = findPos(x);
        if(!isActive(currentPos))
        {
            return false;
        }
        array[currentPos].info = DELETED;//懒惰删除，仅仅将标识位info设置为Deleted
        --currentSize;
        return true;
    }

    //功能: 在散列表中插入元素x,如果插入项已经存在，则什么都不做。
    //否则将其放在表的前端 返回结果：插入成功返回true, 否则返回false
    bool insert(const T & x)
    {
        int currentPos = findPos(x);//先找到位置
        if(isActive(currentPos))//如果该位置处已经存放了该元素，则之间返回false
        {
            return false;
        }
        array[currentPos] = HashEntry(x, ACTIVE);
        //如果当前散列表中元素的个数大于散列表长度的一半，则扩大散列表为原来的2倍
        if(++currentSize > array.size()/2)
        {
            rehash();//扩充表的大小
        }
        return true;
    }

private:

   //功能描述: 获得距离n最近的一个小于n的素数 
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

    //功能描述: 获得距离n最近的一个大于n的素数 
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

    //功能描述: 判断位置currentPos处的是否有元素 有元素则返回true,否则返回false
    bool isActive(int currentPos) const
    {
        return array[currentPos].info == ACTIVE;
    }
    unsigned int myhash(const T & key) const
    {
        return hash(key)%array.size();
    }
    //功能描述: 扩大散列表的大小
    void rehash()
    {
        std::vector<HashEntry> oldArray = array;
        //创建一个新的大小为原来两倍大小的散列表
        array.resize(nextPrime(2 * oldArray.size()));
        for(int i = 0; i < array.size(); ++i)
        {
            array[i].info = EMPTY;
        }
        currentSize = 0;
        //复制散列表
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
