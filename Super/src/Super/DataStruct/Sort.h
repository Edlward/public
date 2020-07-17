#ifndef SORT_H_
#define SORT_H_
//����������������㷨 Continue to add all the sorting algorithm


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

//���������㷨�������  all sort algorithm  and it's test 

void BubbleSort(int* Array,unsigned int len);       //ð������
void ExchangeSort( int* Array,unsigned int len);  //��������
void SelectSort( int* Array,unsigned int len);       //ѡ������
void InsertSort( int* Array,unsigned int len);       //��������
void ShellSort( int* Array,unsigned  int len);       //ϣ������--���ڲ�������Ľ��汾
void QuickSort(int* Array,unsigned int len);        //��������
void Merge_Sort(int* Array,unsigned int len);     //��������
void Heap_Sort(int* Array,unsigned int len);       //������
void Count_Sort(int* ArrayA,int* ArrayB,int* ArrayCurr,unsigned int len,unsigned int k);  //��������
void Count_Sort2(int* Array,int* ArrayCurr,unsigned int len,unsigned int k);
void Basic_Sort(int* Array,int* ArrayB,int *w,int *crr,unsigned int len,unsigned int k,unsigned int d);   //��������
void QuickSort_Tradition(int* Array,unsigned  int low, unsigned int high);    //��ͳ��������
void QuickSort_Random(int* Array, unsigned int low,unsigned  int high);     //�����������



//��װʱ�����Ƿ�����

//
//�������ܣ������������в���Ԫ�أ���ʹ������Ԫ�غ���Ȼ����
//    ����ԭ�Σ�void InsertSort(DataType s[], int &Count, DataType x, bool Up=true);
//������
//    DataType s[]��������Ԫ�ص��������У�
//    int &Count����������������Ԫ�ظ�����
//    DataType x���������Ԫ�أ�
//    bool Up��true���������У�false���������С�
//    ����ֵ���ޡ�
//    ��ע���ޡ�
//    */
//    template <typename DataType>
//void InsertSort(DataType s[], int &Count, DataType x, bool Up=true)
//




//ģ��ʵ�֣��ȶ���ŵ��˴�
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
    if (Count<2)  //С��������û�б�Ҫ����
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
    if (Count<2)  //С��������û�б�Ҫ����
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
    if (Count<2)  //С��������û�б�Ҫ����
    {
        return;
    }

    T temp;                    //һ���洢ֵ
    size_t pos;      //һ���洢�±�
    for(size_t i=0; i<Count; i++)
    {
        temp = Array[i];
        pos  = i;
        for(size_t  j=i+1; j<Count; j++)
        {
            if( Array[j] < temp ) //ѡ�����򷨾����õ�һ��Ԫ������С��Ԫ�ؽ���
            {
                temp = Array[j];
                pos  = j;  //�±�Ľ�����ֵ����¼��ǰ��СԪ�ص��±�λ��
            }
        }
        Array[pos] = Array[i];
        Array[i] = temp;
    }
}

template <typename T>
void InsertSort(T *Array,size_t Count)
{
    if (Count<2)  //С��������û�б�Ҫ����
    {
        return;
    }

    T temp; //һ���洢ֵ
    size_t pos;  //һ���洢�±�
    for(size_t i=1; i<Count; i++) 	//�����n-1�˲���
    {
        temp = Array[i];	//��ǰҪ�����Ԫ��
        pos  = i-1;
        while(temp<Array[pos] )  //while( pos>=0 && temp<Array[pos] )
        {
            Array[pos+1] = Array[pos]; 	//��ǰһ��Ԫ�غ���һλ
            pos--;
        }
        Array[pos+1] = temp;
    }
}

template <typename T>
void ShellSort(T *Array,size_t Count)
{
    if (Count<2)  //С��������û�б�Ҫ����
    {
        return;
    }

    T temp;
    size_t pos;
    size_t d = Count;		//������ֵ
    do{
        d = d/3 + 1 ;
        for(size_t i= d; i<Count; i++ )
        {
            temp = Array[i];
            pos = i-d;
            while(temp < Array[pos] )   //pos>=0 &&  //ʵ������Ϊd�Ĳ�������
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
    if (Count<2)  //С��������û�б�Ҫ����
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
2. ��������

	��1 �� ֱ�Ӳ�������(straight insertion sort)�������ǣ�ÿ�δ��������ȡ����һ��Ԫ�أ��������뵽�����ĺ���λ�ã�ʹ�������Ȼ����
	��һ�˱Ƚ�ǰ������,Ȼ��ѵڶ���������С���뵽�������; �ڶ��˰ѵ�����������ǰ��������ǰ���ɨ�裬�ѵ�����������С���뵽������У����ν�����ȥ��������(n-1)��ɨ���Ժ�����������������̡�

	��2���۰��������binary insertion sort���ǶԲ��������㷨��һ�ָĽ������������㷨�����У����ǲ��ϵ����ν�Ԫ�ز���ǰ�����ź���������С�����ǰ�벿��Ϊ���ź�������У��������ǲ��ð�˳������Ѱ�Ҳ���㣬���Բ����۰���ҵķ������ӿ�Ѱ�Ҳ������ٶȡ�
	�۰���������㷨�ľ������Ϊ���ڽ�һ����Ԫ�ز������ź��������Ĺ����У�Ѱ�Ҳ����ʱ�����������������Ԫ������Ϊa[low],ĩԪ������Ϊa[high]�����ֱȽ�ʱ��������Ԫ����a[m],����m=(low+high)/2��Ƚ�,����Ȳο�Ԫ��С����ѡ��a[low]��a[m-1]Ϊ�µĲ�������(��high=m-1)������ѡ��a[m+1]��a[high]Ϊ�µĲ������򣨼�low=m+1�������ֱ��low<=high��������������λ��֮������Ԫ�غ���һλ��������Ԫ�ز���a[high+1]��
	�۰���������㷨��һ���ȶ��������㷨����ֱ�Ӳ����㷨���Լ����˹ؼ���֮��ȽϵĴ���������ٶȱ�ֱ�Ӳ��������㷨�죬����¼�ƶ��Ĵ���û�б䣬�����۰���������㷨��ʱ�临�Ӷ���ȻΪO(n^2)����ֱ�Ӳ��������㷨��ͬ��
	��3�� 2-·�������������۰��������Ļ������ٸĽ�֮����Ŀ���Ǽ�������������ƶ���¼�Ĵ�������Ϊ����Ҫn����¼�ĸ����ռ䡣ʱ�临�Ӷ�ΪO(n^2)����⣺��ν��2-·����ָ���Ȳ���������ǰ�����棬Ȼ���ٿ��ǲ��뵽�м䡣

*/
