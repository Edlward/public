#ifndef SORT_H_
#define SORT_H_
//继续添加所有排序算法 Continue to add all the sorting algorithm


#if defined(_WIN32)
    #if defined(BASIC_API_EXPORT)
        #define BASIC_API __declspec(dllexport)
	#elif defined(BASIC_API_IMPORT)
        #define BASIC_API __declspec(dllimport)
    #else
        #define BASIC_API 
#endif
#else
        #define BASIC_API 
#endif

//所有排序算法及其测试  all sort algorithm  and it's test 

void BubbleSort(int* Array,unsigned int len);       //冒泡排序
void ExchangeSort( int* Array,unsigned int len);  //交换排序
void SelectSort( int* Array,unsigned int len);       //选择排序
void InsertSort( int* Array,unsigned int len);       //插入排序
void ShellSort( int* Array,unsigned  int len);       //希尔排序--属于插入排序改进版本
void QuickSort(int* Array,unsigned int len);        //快速排序
void Merge_Sort(int* Array,unsigned int len);     //并归排序
void Heap_Sort(int* Array,unsigned int len);       //堆排序
void Count_Sort(int* ArrayA,int* ArrayB,int* ArrayCurr,unsigned int len,unsigned int k);  //计数排序
void Count_Sort2(int* Array,int* ArrayCurr,unsigned int len,unsigned int k);
void Basic_Sort(int* Array,int* ArrayB,int *w,int *crr,unsigned int len,unsigned int k,unsigned int d);   //基数排序
void QuickSort_Tradition(int* Array,unsigned  int low, unsigned int high);    //传统快速排序
void QuickSort_Random(int* Array, unsigned int low,unsigned  int high);     //随机快速排序



//封装时考虑是否逆序

//
//函数功能：向有序数组中插如元素，并使插入新元素后仍然有序。
//    函数原形：void InsertSort(DataType s[], int &Count, DataType x, bool Up=true);
//参数：
//    DataType s[]：欲插入元素的有序序列；
//    int &Count：有序序列中现有元素个数；
//    DataType x：欲插入的元素；
//    bool Up：true按升序排列，false按降序排列。
//    返回值：无。
//    备注：无。
//    */
//    template <typename DataType>
//void InsertSort(DataType s[], int &Count, DataType x, bool Up=true)
//




//模板实现，稳定后放到此处
template <typename T> 
inline void Swap(T& x,T& y)
{
    T tmp;
    tmp=x;
    x=y;
    y=tmp;
}

template <typename T>
void BubbleSort(T *Array,size_t Count) 
{
    if (Count<2)  //小于两个数没有必要排序
    {
        return;
    }
    size_t nPosEnd=Count-1;

    for (size_t i = 0; i!=nPosEnd; i++) 
    {
        for (size_t j = 0; (j+i)!=nPosEnd; j++) 
        {
            if (Array[j] > Array[j+1]) 
            {
                Swap(Array[j],Array[j+1]);
            }
        }
    }
}

template <typename T>
void ExchangeSort(T *Array,size_t Count) 
{
    if (Count<2)  //小于两个数没有必要排序
    {
        return;
    }

    //int temp;
    for(size_t  i=0; i<Count-1; i++)
    {
        for(size_t  j=i+1; j<Count; j++)
        {
            if( Array[j] < Array[i] )
            {
                Swap(Array[i],Array[j]);
            }
        }
    }
}

template <typename T>
void SelectSort(T *Array,size_t Count)
{
    if (Count<2)  //小于两个数没有必要排序
    {
        return;
    }

    T temp;                    //一个存储值
    size_t pos;      //一个存储下标
    for(size_t i=0; i<Count; i++)
    {
        temp = Array[i];
        pos  = i;
        for(size_t  j=i+1; j<Count; j++)
        {
            if( Array[j] < temp ) //选择排序法就是用第一个元素与最小的元素交换
            {
                temp = Array[j];
                pos  = j;  //下标的交换赋值，记录当前最小元素的下标位置
            }
        }
        Array[pos] = Array[i];
        Array[i] = temp;
    }
}

template <typename T>
void InsertSort(T *Array,size_t Count)
{
    if (Count<2)  //小于两个数没有必要排序
    {
        return;
    }

    T temp; //一个存储值
    size_t pos;  //一个存储下标
    for(size_t i=1; i<Count; i++) 	//最多做n-1趟插入
    {
        temp = Array[i];	//当前要插入的元素
        pos  = i-1;
        while(temp<Array[pos] )  //while( pos>=0 && temp<Array[pos] )
        {
            Array[pos+1] = Array[pos]; 	//将前一个元素后移一位
            pos--;
        }
        Array[pos+1] = temp;
    }
}

template <typename T>
void ShellSort(T *Array,size_t Count)
{
    if (Count<2)  //小于两个数没有必要排序
    {
        return;
    }

    T temp;
    size_t pos;
    size_t d = Count;		//增量初值
    do{
        d = d/3 + 1 ;
        for(size_t i= d; i<Count; i++ )
        {
            temp = Array[i];
            pos = i-d;
            while(temp < Array[pos] )   //pos>=0 &&  //实现增量为d的插入排序
            {	
                Array[ pos + d ] = Array[pos];
                pos -= d;
            }
            Array[ pos + d ] = temp;
        }
    } while( d > 1 );
}

template <typename T>
void QuickRun(T* Array,size_t left,size_t right)
{
    T middle;
    T temp;
    size_t i=left;
    size_t j=right;
    middle=Array[(left+right)/2];   //get the middle element
    do{
        while((Array[i]<middle)&&(i<right))
        {
            i++;
        }
        while((Array[j]>middle)&&(j>left))
        {
            j--;
        }
        if(i<=j)
        {            
            //exchange the elements
            //temp=Array[i];
            //Array[i]=Array[j];
            //Array[j]=temp;
            Swap(Array[i],Array[j]);
            i++;
            j--;
        }
    }while(i<=j);
    if(left<j)
    {
        QuickRun(Array,left,j);
    }
    if(right>i)
    {
        QuickRun(Array,i,right);
    }
}

template <typename T>
void QuickSort(T *Array,size_t Count)
{
    if (Count<2)  //小于两个数没有必要排序
    {
        return;
    }

    QuickRun(Array,0,Count-1);
}


#ifdef UNIT_TEST
BASIC_API int Test_sort();
#endif


#endif



/*
2. 插入排序

	（1 ） 直接插入排序(straight insertion sort)的作法是：每次从无序表中取出第一个元素，把它插入到有序表的合适位置，使有序表仍然有序。
	第一趟比较前两个数,然后把第二个数按大小插入到有序表中; 第二趟把第三个数据与前两个数从前向后扫描，把第三个数按大小插入到有序表中；依次进行下去，进行了(n-1)趟扫描以后就完成了整个排序过程。

	（2）折半插入排序（binary insertion sort）是对插入排序算法的一种改进，由于排序算法过程中，就是不断的依次将元素插入前面已排好序的序列中。由于前半部分为已排好序的数列，这样我们不用按顺序依次寻找插入点，可以采用折半查找的方法来加快寻找插入点的速度。
	折半插入排序算法的具体操作为：在将一个新元素插入已排好序的数组的过程中，寻找插入点时，将待插入区域的首元素设置为a[low],末元素设置为a[high]，则轮比较时将待插入元素与a[m],其中m=(low+high)/2相比较,如果比参考元素小，则选择a[low]到a[m-1]为新的插入区域(即high=m-1)，否则选择a[m+1]到a[high]为新的插入区域（即low=m+1），如此直至low<=high不成立，即将此位置之后所有元素后移一位，并将新元素插入a[high+1]。
	折半插入排序算法是一种稳定的排序算法，比直接插入算法明显减少了关键字之间比较的次数，因此速度比直接插入排序算法快，但记录移动的次数没有变，所以折半插入排序算法的时间复杂度仍然为O(n^2)，与直接插入排序算法相同。
	（3） 2-路插入排序是在折半插入排序的基础上再改进之，其目的是减少排序过程中移动记录的次数，但为此需要n个记录的辅助空间。时间复杂度为O(n^2)。理解：所谓的2-路，是指优先插入在序列前面或后面，然后再考虑插入到中间。

*/
