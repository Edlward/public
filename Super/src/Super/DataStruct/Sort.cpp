#include "Sort.h"
#include <time.h>

//算法分类
//稳定的排序算法：冒泡排序、插入排序、归并排序和基数排序
//不是稳定的排序算法：选择排序、快速排序、希尔排序、堆排序

inline void Swap(int& x,int& y)
{
    int tmp;
    tmp=x;
    x=y;
    y=tmp;
}

/*
冒泡排序是一种简单的排序算法。它重复地走访过要排序的数列，一次比较两个元素，
如果它们的顺序错误就把它们交换过来。走访数列的工作是重复地进行直到没有再需要交换，
也就是说该数列已经排序完成。这个算法的名字由来是因为越小的元素会经由交换慢慢“浮”到数列的顶端。 
算法描述:
    比较相邻的元素。如果第一个比第二个大，就交换它们两个；
    对每一对相邻元素作同样的工作，从开始第一对到结尾的最后一对，这样在最后的元素应该会是最大的数；
    针对所有的元素重复以上的步骤，除了最后一个；
    重复步骤1~3，直到排序完成。
时间复杂度:
    最好情况下：正序有序，则只需要比较n次。故，为O(n)  
    最坏情况下:  逆序有序，则需要比较(n-1)+(n-2)+……+1，故，为O(N*N)
稳定性:
    排序过程中只交换相邻两个元素的位置。因此，当两个数相等时，是没必要交换两个数的位置的。
    所以，它们的相对位置并没有改变，冒泡排序算法是稳定的！
*/


//冒泡排序  【稳定的】
void BubbleSort(int* Array,unsigned int Count )
{
    int temp;
    for(unsigned int i=0; i<Count-1; i++)  //外层每循环一次，将最小的一个移动到最前面
    {
        for(unsigned int j=0;j<Count-i-1; j++)
        {
            if( Array[j+1]<Array[j] )
            {
                Swap(Array[j],Array[j+1]);
            }
        }
    }
}
////冒泡排序
//void bubble_sort(int a[],int len)
//{
//    int i,goon;
//    goon = 1;
//    while(goon && len--){
//        goon = 0;
//        for(i=0;i<len;i++){
//            if(a[i]>a[i+1]){
//                swap(&a[i],&a[i+1]);
//                goon =1;
//            }
//        }
//    }
//}

/*
现在注意，我们给出O方法的定义：
若存在一常量K和起点n0，使当n>=n0时，有f(n)<=K*g(n),则f(n) = O(g(n))。
现在我们来看1/2*(n-1)*n，当K=1/2，n0=1，g(n)=n*n时，1/2*(n-1)*n<=1/2*n*n=K*g(n)。
所以f(n) =O(g(n))=O(n*n)。所以我们程序循环的复杂度为O(n*n)。
*/

/*
冒泡排序的一种改进:
鸡尾酒排序，也叫定向冒泡排序，是冒泡排序的一种改进。此算法与
冒泡排序的不同处在于从低到高然后从高到低，而冒泡排序则仅从低
到高去比较序列里的每个元素。他可以得到比冒泡排序稍微好一点的效能。

以序列(2,3,4,5,1)为例，鸡尾酒排序只需要访问一次序列就可以完成排序，但如果使用冒泡排序则需要四次。
但是在乱数序列的状态下，鸡尾酒排序与冒泡排序的效率都很差劲。
*/
//鸡尾酒排序
void CocktailSort(int A[], int n)
{
    int left = 0;                            // 初始化边界
    int right = n - 1;
    while (left < right)
    {
        for (int i = left; i < right; i++)   // 前半轮,将最大元素放到后面
        {
            if (A[i] > A[i + 1])
            {
                Swap(A[i],A[i+1]);
            }
        }
        right--;
        for (int i = right; i > left; i--)   // 后半轮,将最小元素放到前面
        {
            if (A[i-1] > A[i])
            {
                 Swap(A[i-1],A[i]);
            }
        }
        left++;
    }
}




//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//交换排序    【稳定的】
void ExchangeSort(int *Array,unsigned int Count)
{
    int temp;
    for(unsigned int i=0; i<Count-1; i++)
    {
        for(unsigned int j=i+1; j<Count; j++)
        {
            if( Array[j] < Array[i] )
            {
                Swap(Array[i],Array[j]);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//选择排序(直接选择排序)
/*
思想：
    首先在未排序序列中找到最小元素，存放到排序序列的起始位置，然后，再从剩余未排序元素中继续寻找最小元素，然后放到排序序列末尾。以此类推，直到所有元素均排序完毕。具体做法是：选择最小的元素与未排序部分的首部交换，使得序列的前面为有序。  
时间复杂度: 
    最好情况下：交换0次，但是每次都要找到最小的元素，因此大约必须遍历N*N次，因此为O(N*N)。减少了交换次数！ 
    最坏情况下，平均情况下：O(N*N)
稳定性:
    由于每次都是选取未排序序列A中的最小元素x与A中的第一个元素交换，因此跨距离了，很可能破坏了元素间的相对位置，因此选择排序是不稳定的！
*/

//选择排序【不稳定的】
void SelectSort( int *Array,unsigned int Count)
{
    int temp;                    //一个存储值
    unsigned  int pos;      //一个存储下标
    for(unsigned int i=0; i<Count; i++)
    {
        temp = Array[i];
        pos  = i;
        for(unsigned int j=i+1; j<Count; j++)
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

//遗憾的是算法需要的循环次数依然是1/2*(n-1)*n。所以算法复杂度为O(n*n)。
//我们来看他的交换。由于每次外层循环只产生一次交换（只有一个最小值）。所以f(n)<=n
//所以我们有f(n)=O(n)。所以，在数据较乱的时候，可以减少一定的交换次数。


/*
改进选择排序
    但是我们上篇文章中提到过，在排序中应该尽量避免较多的和元素互换操作，而这里每比较一次，如果遇到更小的，就要互换一次元素。为了减少元素互换操作，我们可以在每次比较后不直接进行交换，将较小的元素的位置序号记录下来，这样一趟比较之后，就会得到最小元素的位置，如果最小值的位置发生了改变，再将该位置的元素与第一个元素互换，依次类推。。。这样每一趟比较完成后最多只需执行一次元素互换的操作。
*/

//改进选择排序
void Select_Sort2(int *Array,unsigned int len)  
{  
    unsigned int i,j,min;  
    for(i=0;i<len;i++)  
    {  
        min = i;    //用来记录每一趟比较的最小值的位置  
        for(j=i+1;j<len;j++)  
        {
            if(Array[min] > Array[j])  
            {
                min = j;     //仅记录最小值的位置  
            }
        }
        //如果最小值的位置发生了变化，  
        //则最后执行一次元素互换的操作  
        if(min != i)  
        {  
            Swap(Array[i],Array[min]);
        }  
    }  
}  


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//插入排序:
/*
思想：
    如下图所示，每次选择一个元素K插入到之前已排好序的部分A[1…i]中，插入过程中K依次由后向前与A[1…i]中的元素进行比较。若发现发现A[x]>=K,则将K插入到A[x]的后面，插入前需要移动元素。
算法时间复杂度:  
    最好的情况下：正序有序(从小到大)，这样只需要比较n次，不需要移动。因此时间复杂度为O(n)  
    最坏的情况下：逆序有序,这样每一个元素就需要比较n次，共有n个元素，因此实际复杂度为O(n¬2)  
    平均情况下：O(n¬2)
稳定性:
    理解性记忆比死记硬背要好。因此，我们来分析下。稳定性，就是有两个相同的元素，排序先后的相对位置是否变化，主要用在排序时有多个排序规则的情况下。在插入排序中，K1是已排序部分中的元素，当K2和K1比较时，直接插到K1的后面(没有必要插到K1的前面，这样做还需要移动！！)，因此，插入排序是稳定的。
*/


//插入排序   (直接插入排序)【稳定的】
void InsertSort( int *Array,unsigned int Count)
{
    int temp; //一个存储值
    unsigned int pos;  //一个存储下标
    for(unsigned int i=1; i<Count; i++) 	//最多做n-1趟插入
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

//其复杂度仍为O(n*n)。
//最终，我个人认为，在简单排序算法中，直接插入排序是最好的。


//折半插入排序
/*
引出
    在直接插入排序过程中，我们是把一个记录插入到有序序列中，至于要插入到有序序列中的哪个位置呢？采用的是顺序查找确定插入的位置。显然对于有序序列，折半查找的效率要高，所以在寻找插入位置时可以用折半查找。折半查找主要分为三步：1、查找要插入的位置  2、移位  3、把记录插入相应的位置。
代码*/

//折半查找
int binarySearch(int *a,int low,int high,int key)
{
    int mid=(low+high)/2;
    if(low>high)
        return low;
    if(a[mid]==key)
        return mid;
    else if(key<a[mid])
        return binarySearch(a,low,mid-1,key);
    else 
        return binarySearch(a,mid+1,high,key);
}

//折半插入排序
void binaryInsertSort(int *a,int n)
{
    int i,j,site,temp;
    for(i=2;i<=n;i++){
        //1.折半查找要插入的位置
        site=binarySearch(a,1,i,a[i]);
        temp=a[i];
        //2.移位
        for(j=i;j>site;j--)
            a[j]=a[j-1];
        //3.插入a[i]
        a[site]=temp;
    }
}
/*
//效率分析
    折半插入排序是对直接插入排序的一种改进，这种改进只考虑了关键字比较次数，并没有减少移位次数，所以平均时间和最坏情况下（待排序序列逆序）时间复杂度o（n^2）,如果记录数量很大的话，这两种情况下是优于直接插入排序。再来看一下最佳情况（待排序序列有序），此时关键字比较次数并不为o（1），时间复杂度为o（n*log2n）。（其中折半查找时间复杂度o（log2n），这个在以后写查找的时候再分析，这里不做详细讲解。）。所以在记录数较小、待排序序列基本有序情况下直接插入排序优于折半插入排序。此外，折半插入排序是不稳定的原地排序，实现起来也较复杂。
*/

//表插入排序
/*
//引出
    折半插入排序相对于直接插入排序来说减少了比较次数。那么我们可不可以减少移动次数呢，答案是可以的。于是就有了表插入排序，用一个静态链表来存储待排序序列，其他操作和直接插入排序很像。主要步骤：1、初始化链表  2、取出要插入的记录 3、遍历链表寻找插入位置  4、记录插入链表中。
*/

//代码

//静态链表
typedef struct
{
    int key;//关键字
    int next;//指向下一个关键字的下标
}Node,*PNode;

//表插入排序
void tableInsertSort(PNode list,int n)
{
    int p,head;
    int i;
    //初始化
    list[0].next=1;
    list[1].next=0;

    //逐个插入
    for(i=2;i<=n;i++)
    {
        head=0;
        p=list[0].next;
        //遍历链表，寻找插入位置
        while(p!=0 && list[p].key<=list[i].key)
        {
            head=p;
            p=list[p].next;
        }
        if(p==0)
        {//插入的值是最大值
            list[i].next=p;
            list[head].next=i;
        }else
        {
            list[i].next=p;
            list[head].next=i;
        }

    }
}
/*
效率分析
    表插入排序也是对直接插入排序的一种改进，这种改进只减少了移动次数，并没有减少关键字比较次数，所以平均时间和最坏情况下（待排序序列逆序）时间复杂度o（n^2）,如果记录数量很大的话，这两种情况下是优于直接插入排序。再来看一下最佳情况（待排序序列有序），关键字比较次数并为o（1），时间复杂度为o（n）。此时和直接插入排序时间复杂度一样。此外，表插入排序改变了记录的存储结构，无法顺序访问，是一种稳定的排序算法，实现起来也较复杂。
*/

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/*
思想：希尔排序也是一种插入排序方法,实际上是一种分组插入方法。
先取定一个小于n的整数d1作为第一个增量,把表的全部记录分成d1个组,
所有距离为d1的倍数的记录放在同一个组中,在各组内进行直接插入排序；
然后,取第二个增量d2(＜d1),重复上述的分组和排序,直至所取的增量
dt=1(dt<dt-1<…<d2<d1),即所有记录放在同一组中进行直接插入排序为止。    
例如：将 n 个记录分成 d 个子序列： 
{ R[0]，   R[d]，     R[2d]，…，     R[kd] } 
{ R[1]，   R[1+d]， R[1+2d]，…，R[1+kd] } 
… 
{ R[d-1]，R[2d-1]，R[3d-1]，…，R[(k+1)d-1] }
说明：d=5 时，先从A[d]开始向前插入，判断A[d-d]，然后A[d+1]与A[(d+1)-d]比较，如此类推，这一回合后将原序列分为d个组。<由后向前>
时间复杂度:  
    最好情况：由于希尔排序的好坏和步长d的选择有很多关系，因此，目前还没有得出最好的步长如何选择(现在有些比较好的选择了，但不确定是否是最好的)。所以，不知道最好的情况下的算法时间复杂度。  
    最坏情况下：O(N*logN)，最坏的情况下和平均情况下差不多。  
    平均情况下：O(N*logN)
稳定性:
    由于多次插入排序，我们知道一次插入排序是稳定的，不会改变相同元素的相对顺序，但在不同的插入排序过程中，相同的元素可能在各自的插入排序中移动，最后其稳定性就会被打乱，所以shell排序是不稳定的。(有个猜测，方便记忆：一般来说，若存在不相邻元素间交换，则很可能是不稳定的排序。)
*/

//希尔排序法   【不稳定的】
void ShellSort(int* Array,unsigned int Count )
{
    int temp;
    unsigned int pos;
    unsigned int d = Count;		//增量初值
    do{
        d = d/3 + 1 ;
        for(unsigned int i= d; i<Count; i++ )
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

//第一种实现方法
/*
    仔细分析希尔排序的实现思想，会发现，如果要循环对各个子序列依次进行插入排序，我们需要在直接插入排序代码的外面再加一层for循环，用来循环所有的子序列。我们根据插入排序的第一种实现方法写出的代码如下：
*/

void Shell_Insert1(int *arr,int len,int ader)  
{  
    int i,k;  
    //循环对ader个子序列进行插入排序操作  
    for(k=0;k<ader;k++)  
        for(i=ader+k;i<len;i+=ader)      //对一个子序列进行插入排序操作  
        {   //将第i个元素分别与前面的每隔ader个位置的元素比较，插入适当的位置  
            if(arr[i]<arr[i-ader])  
            {   //一直向左进行比较，直到插入到适当的位置  
                int key = arr[i];  
                int count = 0;  //用来记录key在与前面元素比较时向左移动了几个ader长度  
                while(i>k && key<arr[i-ader])  
                {  
                    arr[i] = arr[i-ader];  
                    arr[i-ader] = key;  
                    i -= ader;  
                    count++;  
                }  
                //将待插入的数定位到下一个元素，执行下一次插入排序  
                //因为后面还要执行i+=ader，所以这里回到原位置即可  
                i += count*ader;    
            }   
        }  
}  

//第二种实现方法
/*
    很明显，与上面插入排序的第一种实现方法一样，更加冗杂，现在我们用插入排序的第二种实现方法来实现希尔排序，同样采用添加外层for循环的方式，来循环对每个子序列进行插入排序。代码如下：
*/


//对长为len的数组进行一趟增量为ader的插入排序 
//本算法在插入排序算法的第三种实现形式上进行修改得到 
void Shell_Insert2(int *arr,int len,int ader)  
{  
    int i,j,k;  
    //循环对ader个子序列各自进行插入排序  
    for(k=0;k<ader;k++)  
        for(i=ader+k;i<len;i+=ader)  
            for(j=i-ader;j>=k && arr[j]>arr[j+ader];j-=ader)  
            {  
                //交换元素数值  
                arr[j]^=arr[j+ader];  
                arr[j+ader]^=arr[j];  
                arr[j]^=arr[j+ader];  
            }  
}  
//第二种实现方法的改进
/*
上面的代码中需要三个for循环，因为我们是循环对每个子序列进行插入排序的，实际上我们还可以这样做：对每个子序列交叉进行排序。比如，第1个子序列中的第2个元素A5（A5表示它在总序列A中的位置序号是5，下同）刚进行完插入排序操作，便接着对第2个子序列中的第2个元素A6进行插入排序操作。这样我们就可以少写一个for循环，但实际比较的次数还是相同的，只是代码更加简洁。如下：
*/

//在第二种代码的形式上继续精简代码 
//交叉进行各个子序列的插入排序 
void Shell_Insert2_1(int *arr,int len,int ader)  
{  
    int i,j;  
    //交叉对ader个子序列进行插入排序  
    for(i=ader;i<len;i++)  
        for(j=i-ader;j>=0 && arr[j]>arr[j+ader];j-=ader)  
        {  
            //交换元素数值  
            //由于不会出现自己与自己交换的情况  
            //因此可以安全地用该交换方法  
            arr[j]^=arr[j+ader];  
            arr[j+ader]^=arr[j];  
            arr[j]^=arr[j+ader];  
        }  
}  
//第三种实现方法
//同样，根据插入排序的第三种实现方法，循环逐个对每个子序列进行插入排序操作，我们可以得到希尔排序的实现方法，如下：

//对长为len的数组进行一趟增量为ader的插入排序 
//本算法在插入排序算法的第二种实现形式上进行修改得到 
void Shell_Insert3(int *arr,int len,int ader)  
{  
    int i,j,k;  
    //循环对ader个子序列各自进行插入排序  
    for(k=0;k<ader;k++)  
        for(i=ader+k;i<len;i+=ader)  
            if(arr[i] < arr[i-ader])  
            {  
                int key = arr[i];  
                for(j=i-ader;j>=k && arr[j]>key;j-=ader)  
                    arr[j+ader] = arr[j];  
                arr[j+ader] = key;  
            }  
}  
//第三种实现方法的改进
//在第三种代码的形式上继续精简代码 
//交叉进行各个子序列的插入排序 
void Shell_Insert3_1(int *arr,int len,int ader)  
{  
    int i,j;  
    //对ader子序列交叉进行插入排序  
    for(i=ader;i<len;i++)  
        if(arr[i] < arr[i-ader])  
        {  
            int key = arr[i];  
            for(j=i-ader;j>=0 && arr[j]>key;j-=ader)  
                arr[j+ader] = arr[j];  
            arr[j+ader] = key;  
        }  
}  
//同样，如果在面试中要手写希尔排序的代码，推荐这种方法实现的代码。




//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//6.快速排序法 --不稳定的排序
/*
快速排序虽然平均复杂度为O(N logN)，却可能由于不当的pivot选择，
导致其在最坏情况下复杂度恶化为O(N2)。另外，由于快速排序一般是用递归实现，
我们知道递归是一种函数调用，它会有一些额外的开销，比如返回指针、参数压栈、出栈等，
在分段很小的情况下，过度的递归会带来过大的额外负荷，从而拉缓排序的速度。
*/

/*
思想：
    它是由冒泡排序改进而来的。在待排序的n个记录中任取一个记录(通常取第一个记录),把该记录放入适当位置后,数据序列被此记录划分成两部分。所有关键字比该记录关键字小的记录放置在前一部分,所有比它大的记录放置在后一部分,并把该记录排在这两部分的中间(称为该记录归位),这个过程称作一趟快速排序。
    说明：最核心的思想是将小的部分放在左边，大的部分放到右边，实现分割。         
算法复杂度:
    最好的情况下：因为每次都将序列分为两个部分(一般二分都复杂度都和logN相关)，故为 O(N*logN)  
    最坏的情况下：基本有序时，退化为冒泡排序，几乎要比较N*N次，故为O(N*N)
稳定性:
    由于每次都需要和中轴元素交换，因此原来的顺序就可能被打乱。如序列为 5 3 3 4 3 8 9 10 11会将3的顺序打乱。所以说，快速排序是不稳定的！
*/

void QuickRun(int* Array,unsigned int left,unsigned int right)
{
    int middle;
    int temp;
    unsigned int i,j;
    i=left;
    j=right;
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
            Swap(Array[i],Array[j]);
            //exchange the elements
            //temp=Array[i];
            //Array[i]=Array[j];
            //Array[j]=temp;
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

void QuickSort(int *Array,unsigned int Count)
{
    QuickRun(Array,0,Count-1);
}


//////////////////////////////////////////////////////////////////
//归并排序
/*
思想：多次将两个或两个以上的有序表合并成一个新的有序表。 
算法时间复杂度:
最好的情况下：一趟归并需要n次，总共需要logN次，因此为O(N*logN) 
最坏的情况下，接近于平均情况下，为O(N*logN) 
说明：对长度为n的文件，需进行logN 趟二路归并，每趟归并的时间为O(n)，故其时间复杂度无论是在最好情况下还是在最坏情况下均是O(nlgn)。
稳定性:
归并排序最大的特色就是它是一种稳定的排序算法。归并过程中是不会改变元素的相对位置的。 
缺点:它需要O(n)的额外空间。但是很适合于多链表排序。

*/


#include<stdio.h>
#include<stdlib.h>

/*
将有序的arr[start...mid]和有序的arr[mid+1...end]归并为有序的brr[0...end-start+1],
而后再将brr[0...end-start+1]复制到arr[start...end]，使arr[start...end]有序
*/

void Merge(int *ArrayA,int *ArrayB,unsigned int begin,unsigned int mid,unsigned int end)
{
    unsigned int i = begin;
    unsigned int j = mid+1;
    unsigned int k = 0;
    //比较两个有序序列中的元素，将较小的元素插入到brr中
    while(i<=mid && j<=end)
    {
        if(ArrayA[i]<=ArrayA[j])
        {
            ArrayB[k++] = ArrayA[i++];
        }
        else
        {
            ArrayB[k++] = ArrayA[j++];
        }
    }

    //将arr序列中剩余的元素复制到brr中
    //这两个语句只可能执行其中一个
    while(i<=mid)
    {
        ArrayB[k++] = ArrayA[i++];
    }
    while(j<=end)
    {
        ArrayB[k++] = ArrayA[j++];
    }

    //将brr中的元素复制到arr中，使arr[start...end]有序
    for(i=0;i<k;i++)
    {
        ArrayA[i+begin] = ArrayB[i];
    }
}

/*
借助brr数组对arr[start...end]内的元素进行归并排序
归并排序后的顺序为从小到大
*/
void MSort(int *ArrayA,int *ArrayB,unsigned int begin,unsigned int end)
{
    if(begin < end)
    {
        unsigned int mid = (begin+end)/2;
        MSort(ArrayA,ArrayB,begin,mid);       //左边递归排序
        MSort(ArrayA,ArrayB,mid+1,end);       //右边递归排序
        Merge(ArrayA,ArrayB,begin,mid,end);   //左右序列归并
    }
}
/*
将该排序算法封装起来
*/
void Merge_Sort(int *Array,unsigned int Count)
{
    int *brr = (int *)malloc(Count*sizeof(int));
    MSort(Array,brr,0,Count-1);
    free(brr);
    brr = 0;
}

/*
小总结
归并排序的最好最坏和平均时间复杂度都是O（n*logn），但是需要额外的长度
为n的辅助数组（每次递归调用前都会释放上次递归中传入到Merge函数的brr数组），
因此空间复杂度为O（n）,而不会因为栈的最大深度为O（logn）而积累至O（n*logn）。
占用额外空间是归并排序不足的地方，但是它是几个高效排序算法（快速排序、堆排序、
希尔排序）中唯一稳定的排序方法。
*/
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////
//堆排序
/*
arr[start+1...end]满足最大堆的定义，
将arr[start]加入到最大堆arr[start+1...end]中，
调整arr[start]的位置，使arr[start...end]也成为最大堆
注：由于数组从0开始计算序号，也就是二叉堆的根节点序号为0，
因此序号为i的左右子节点的序号分别为2i+1和2i+2
*/
/*
思想：
    利用完全二叉树中双亲节点和孩子节点之间的内在关系，在当前无序区中选择关键字最大(或者最小)的记录。也就是说，以最小堆为例，根节点为最小元素，较大的节点偏向于分布在堆底附近。 
算法复杂度:
    最坏情况下，接近于最差情况下：O(N*logN)，因此它是一种效果不错的排序算法。
稳定性:
    堆排序需要不断地调整堆，因此它是一种不稳定的排序！

*/
void HeapAdjustDown(int *arr,unsigned int start,unsigned int end)
{
    int temp = arr[start];  //保存当前节点
    unsigned int i = 2*start+1;      //该节点的左孩子在数组中的位置序号
    while(i<=end)
    {
        //找出左右孩子中最大的那个
        if(i+1<=end && arr[i+1]>arr[i])
            i++;
        //如果符合堆的定义，则不用调整位置
        if(arr[i]<=temp)
            break;
        //最大的子节点向上移动，替换掉其父节点
        arr[start] = arr[i];
        start = i;
        i = 2*start+1;
    }
    arr[start] = temp;
}

/*
堆排序后的顺序为从小到大
因此需要建立最大堆
*/
void Heap_Sort(int *array,unsigned int len)
{
    unsigned int i;
    //把数组建成为最大堆
    //第一个非叶子节点的位置序号为len/2-1
    for(i=len/2-1;i>=0;i--)
    {
        HeapAdjustDown(array,i,len-1);
    }
    //进行堆排序
    for(i=len-1;i>0;i--)
    {
        //堆顶元素和最后一个元素交换位置，
        //这样最后的一个位置保存的是最大的数，
        //每次循环依次将次大的数值在放进其前面一个位置，
        //这样得到的顺序就是从小到大
        //int temp = array[i];
        //array[i] = array[0];
        //array[0] = temp;
        Swap(array[0],array[i]);
        //将arr[0...i-1]重新调整为最大堆
        HeapAdjustDown(array,0,i-1);
    }
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////














//////////////////////////////////////////////////////////////////
//9.计数排序
/*
第一种形式实现计数排序
计数排序后的顺序为从小到大
arr[0...len-1]为待排数组，每个元素均是0-k中的一个值
brr[0...len-1]为排序后的输出数组
crr[0...k]保存0...k中每个值在数组arr中出现的次数
*/
void Count_Sort(int *arrayA,int *arrayB,int *crr,unsigned int len,unsigned int k)
{
    unsigned int i,j=0;
    //数组crr各元素置0
    for(i=0;i<=k;i++)
        crr[i] = 0;
    //统计数组arr中每个元素重复出现的个数
    for(i=0;i<len;i++)
        crr[arrayA[i]]++;
    //求数组arr中小于等于i的元素个数
    for(i=1;i<=k;i++)
        crr[i] += crr[i-1];
    //把arr中的元素放在brr中对应的位置上
    for(i=len-1;i>=0;i--)
    {
        arrayB[crr[arrayA[i]]-1] = arrayA[i];
        //如果有相同的元素，则放在下一个位置上
        crr[arrayA[i]]--;
    }
}
//很明显上面代码的时间复杂度为O（n+k），但用到了brr来保存排序结果，
//我们可以它做些改进，使排序原地进行，如下
/*
第二种形式实现计数排序
计数排序后的顺序为从小到大
arr[0...len-1]为待排数组，每个元素均是0-k中的一个值
crr[0...k]保存0...k中每个值在数组arr中出现的次数
*/
void Count_Sort2(int *arr,int *crr,unsigned int len,unsigned int k)
{
    unsigned int i,j=0;
    //数组crr各元素置0
    for(i=0;i<=k;i++)
        crr[i] = 0;
    //统计数组arr中每个元素重复出现的个数
    for(i=0;i<len;i++)
        crr[arr[i]]++;
    //根据crr[i]的大小，将元素i放入arr适当的位置
    for(i=0;i<=k;i++)
        while((crr[i]--)>0)
        {
            arr[j++] = i;
        }
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////











//////////////////////////////////////////////////////////////////
//10.基数排序--在计数排序基础上
/*
思想：它是一种非比较排序。它是根据位的高低进行排序的，也就是先按个位排序，然后依据十位排序……以此类推。示例如下： 
时间复杂度:
分配需要O(n),收集为O(r),其中r为分配后链表的个数，以r=10为例，则有0～9这样10个链表来将原来的序列分类。而d，也就是位数(如最大的数是1234，位数是4，则d=4)，即"分配-收集"的趟数。因此时间复杂度为O(d*(n+r))。
稳定性:
基数排序过程中不改变元素的相对位置，因此是稳定的！
适用情况：如果有一个序列，知道数的范围(比如1～1000)，用快速排序或者堆排序，需要O(N*logN)，但是如果采用基数排序，则可以达到O(4*(n+10))=O(n)的时间复杂度。算是这种情况下排序最快的！！

*/
#include<stdio.h>
#include<stdlib.h>

/*
在第一种计数排序的实现形式上做了些修改
计数排序后的顺序为从小到大
arr[0...len-1]为待排数组，我们这里采用三位数
brr[0...len-1]为排序后的有序数组
w[0...len-1]用来保存取出的每一位上的数，其每个元素均是0-k中的一个值
crr[0...k]保存0...k中每个值出现的次数
*/

//直接引用上面的计数排序

void Count_Sort3(int *arr,int *brr,int *w,int *crr,unsigned int len,unsigned int k)
{
    unsigned int i;
    //数组crr各元素置0
    for(i=0;i<=k;i++)
        crr[i] = 0;
    //统计数组w中每个元素重复出现的个数
    for(i=0;i<len;i++)
        crr[w[i]]++;
    //求数组w中小于等于i的元素个数
    for(i=1;i<=k;i++)
        crr[i] += crr[i-1];
    //把arr中的元素放在brr中对应的位置上
    for(i=len-1;i>=0;i--)
    {
        brr[crr[w[i]]-1] = arr[i];
        //如果有相同的元素，则放在下一个位置上
        crr[w[i]]--;
    }
    //再将brr中的元素复制给arr，这样arr就有序了
    for(i=0;i<len;i++)
    {
        arr[i] = brr[i];
    }
}

/*
基数排序后的顺序为从小到大
其中参数d为元素的位数
*/
void Basic_Sort(int *arr,int *brr,int *w,int *crr,unsigned int len,unsigned int k,unsigned int d)
{
    unsigned int i,j,val=1;
    //从低位到高位依次进行计数排序
    for(i=1;i<=d;i++)
    {   //w中保存的是arr中每个元素对应位上的数
        //范围在0-k之间
        for(j=0;j<len;j++)
            w[j] = (arr[j]/val)%10;
        //对当前位进行计数排序
        Count_Sort3(arr,brr,w,crr,len,k);
        val *= 10;
    }
}



//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
//11.桶排序--因为它的实现不具备普遍性，要根据不同的情况来划分不同个数的桶，以及桶所规定的区间。
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//12.改进的快速排序---对于有序的输入，随机化版本的快速排序比传统快速排序快2-3倍。
//QuickSort_Random
//void Swap(int &a, int &b)
//{
//    int tmp = a;
//    a = b;
//    b = tmp;
//}

// 传统划分操作
int Tradition_Partition(int* Array, int low, int high)
{    
    int pivot = Array[low];
    int i = low;
    for(int j=low+1; j<=high; ++j)
    {
        if(Array[j] <= pivot)
        {
            ++i;
            Swap(Array[i], Array[j]);
        }
    }
    Swap(Array[i], Array[low]);
    return i;
}
// 随机化划分操作，随机选择pivot
int Random_Partition(int* Array, int low, int high)
{
    srand(time(NULL));
    int i = rand() % (high+1);
    Swap(Array[low], Array[i]);
    return Tradition_Partition(Array, low, high);
}
// 传统快排
void QuickSort_Tradition(int A[], int low, int high)
{
    if(low < high)
    {
        int pos = Tradition_Partition(A, low, high);
        QuickSort_Tradition(A, low, pos-1);
        QuickSort_Tradition(A, pos+1, high);
    }
}     // 随机化快速排序
void QuickSort_Random(int A[], int low, int high)
{   
    if(low < high)
    {
        int pos = Random_Partition(A, low, high);
        QuickSort_Random(A, low, pos-1);
        QuickSort_Random(A, pos+1, high);
    }
}


//1. Gnome排序 是插入排序的变形，唯一的区别在于它是使用类似冒泡排序的Swap方法将选择出来的元素移动到合适的位置。也因此它是Stable的排序算法。
/*
    其思想如下：
    Gnome Sort is based on the technique used by the standard Dutch Garden Gnome (Du.: tuinkabouter). Here is how a garden 
    gnome sorts a line of flower pots. Basically, he looks at the flower pot next to him and the previous one; if they are 
    in the right order he steps one pot forward, otherwise he swaps them and steps one pot backwards. Boundary conditions: 
if there is no previous pot, he steps forwards; if there is no pot next to him, he is done. 
*/
//其代码仅五行：
void GnomeSort(int* Array,unsigned int Count)
{  
        unsigned int i = 0;  
        while (i < Count) 
        {  
            if (i == 0 || Array[i-1] <= Array[i])
            {
                i++;  
            }
            else 
            {
                int tmp = Array[i]; 
                Array[i] = Array[i-1]; 
                Array[--i] = tmp;
            }  
        }  
}  
//时间复杂度为O(n^2)，空间复杂度为O(1)。

/*
2. Comb Sort，梳排序或者梳子排序，就像梳子那样有间隔地比较两个数，很形象，O(n*logn)时间复杂度，O(1)空间复杂度，属于不稳定的排序算法。算法的思想是使逆序的元素尽可能快地移动到最终的位置，而不是像冒泡排序那样每次交换只移动一个位置。
*/

void CombSort(int *Array, unsigned int Count) 
{  
        float shrink_factor = 1.247330950103979;  
        unsigned  int gap = Count, swapped = 1, swap, i;  

        while ((gap > 1) || swapped) 
        {  
            if (gap > 1) gap = gap / shrink_factor;  
            swapped = 0;   
            i = 0;  
            while ((gap + i) < Count) 
            {  
                if (Array[i] - Array[i + gap] > 0)
                {  
                    swap = Array[i];  
                    Array[i] = Array[i + gap];  
                    Array[i + gap] = swap;  
                    swapped = 1;  
                }  
                ++i;  
            }  
        }  
}  









//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//一个Quicksort究竟可以写到多么短

//然而快排的基本思想却是极其简单的：接收一个数组，挑一个数，然后把比它小的那一摊数
//放在它的左边，把比它大的那一摊数放在它的右边，然后再对这个数左右两摊数递归的执行快
//排过程，直到子数组只剩一个数为止。
//最优结果--利用C的指针算术，去掉多余的参数
#if 0
void quicksort(int *array, int n)
{
    if(n>1){
        int pivot=0,j;
        for(j=1;j<n;j++)
            if(array[j]<array[0])
                swap(array,++pivot,j);
        swap(array,0,pivot);
        quicksort(array,pivot);
        quicksort(array+pivot+1,n-pivot-1);
    }
}
#endif

//这样的话可以把原本20多行的快排缩减到10行，但是这样有什么意义呢，
//程序的可读性大为下降，而且运行效率也没有丝毫的提升。此外，指针算术很可能会导致各种越界错误。


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//漫谈经典排序算法--补缺的--可能重复

//快速排序（两端交替着向中间扫描）
void QuickSort1(int *a,int low,int high)
{
    int pivotkey=a[low];//以a[low]为枢纽值
    int i=low,j=high;
    if(low>=high)
        return;
    //一趟快速排序
    while(i<j)
    {//双向扫描
        while(i < j && a[j] >= pivotkey)
            j--;
        a[i]=a[j];
        while(i < j && a[i] <= pivotkey)
            i++;
        a[j]=a[i];
    }
    a[i]=pivotkey;//放置枢纽值
    //分别对左边、右边排序
    QuickSort1(a,low,i-1); 
    QuickSort1(a,i+1,high); 
}

//快速排序（以最后一个记录的值为枢纽值，单向扫描数组）
void QuickSort2(int *a,int low,int high)
{
    int pivotkey=a[high];//以a[high]为枢纽值
    int i=low-1,temp,j;
    if(low>=high)
        return;
    //一趟快速排序
    for(j=low;j<high;j++)
    {
        if(a[j]<=pivotkey)
        {
            i++;
            temp=a[i];
            a[i]=a[j];
            a[j]=temp;
        }
    }
    i++;
    //放置枢纽值
    temp=a[i];
    a[i]=pivotkey;
    a[high]=temp;
    //分别对左边、右边排序
    QuickSort2(a,low,i-1); 
    QuickSort2(a,i+1,high); 
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


 







/*
C语言标准库函数qsort排序的介绍与使用
qsort函数包含在<stdlib.h>的头文件里。
qsort函数声明如下：
    void qsort(void *base, unsigned intnmemb, unsigned intsize, int(*compar)(const void *, const void *));

参数说明如下：
base: 要排序的数组
nmemb: 数组中的元素数目
size: 每个数组元素占用内存空间，可使用sizeof获得
compar: 比较两个数组元素的比较函数。本比较函数的第一个参数值小于、等于、大于第二参数值时，本比较函数的返回值应分别小于、等于、大于零。

也就说你要实现一个这样的函数（升序）：
int cmp(const void *a, const void *b)
        如果a > b，返回>0
        如果a == b, 返回0
        如果a < b，返回<0
这里的a和b的关系仅仅是逻辑上的，并不是值比较，所以排序的可以不仅仅是数字，还可以是字符。
*/

/*
//以下排序都是采用的从小到大排序

一、对int类型数组排序
int num[100];

int cmp ( const void *a , const void *b)
{
    return *(int *)a - *(int *)b;
}

qsort(num,100,sizeof(num[0]),cmp);
对一个二维数组的进行排序：
    int a[1000][2]; 其中按照a[0]的大小进行一个整体的排序，其中a[1]必须和a[0]一起移动交换。

qsort(a,1000,sizeof(int)*2,comp);

int comp(const void *a,const void *b)
{
    return ((int *)a)[0]-((int *)b)[0];
}

二、对char类型数组排序（同int类型）
char word[100];

int cmp( const void *a , const void *b)
{
    return *(char *)a - *(char *)b;
}
qsort(word,100,sizeof(word[0]),cmp);

三、对double类型数组排序（特别要注意）

double in[100];
int cmp( const void *a , const void *b)
{
    return *(double *)a > *(double *)b ? 1 : -1;
}
qsort(in,100,sizeof(in[0]),cmp)；

四、对结构体一级排序
struct In
{
    double data;
    int other;
}s[100]

//按照data的值从小到大将结构体排序,关于结构体内的排序关键数据data的类型可以很多种，参考上面的例子写
int cmp( const void *a ,const void *b)
{
    return (*(In *)a).data > (*(In *)b).data ? 1 : -1;
}
qsort(s,100,sizeof(s[0]),cmp);

五、对结构体二级排序
struct In
{
    int x;
    int y;
}s[100];

//按照x从小到大排序，当x相等时按照y从大到小排序
int cmp( const void *a , const void *b)
{
    struct In *c = (In *)a;
    struct In *d = (In *)b;
    if(c->x != d->x) return c->x - d->x;
    else return d->y - c->y;
}
qsort(s,100,sizeof(s[0]),cmp);

六、对字符串进行排序

struct In
{
    int data;
    char str[100];
}s[100];

//按照结构体中字符串str的字典顺序排序
int cmp ( const void *a , const void *b)
{
    return strcmp( (*(In *)a).str , (*(In *)b).str);
}

qsort(s,100,sizeof(s[0]),cmp);

七、计算几何中求凸包的cmp
int cmp(const void *a,const void *b) //重点cmp函数，把除了1点外的所有点，旋转角度排序
{
    struct point *c=(point *)a;
    struct point *d=(point *)b;
    if( calc(*c,*d,p[1]) < 0) 
        return 1;
    //如果在一条直线上，则把远的放在前面
    else if( !calc(*c,*d,p[1]) && dis(c->x,c->y,p[1].x,p[1].y) < dis(d->x,d->y,p[1].x,p[1].y)) 
        return 1;
    else 
        return -1;
}
*/



bool isRirhtOrder(int* Array,unsigned int len)
{
    for(unsigned int i=1;i<len;i++)
    {
        if( Array[i] < Array[i-1] )
        {
            return false;
        }
    }
    return true;
}
bool FillRandom(int* Array,unsigned int len)
{
    srand(time(NULL));
    for(unsigned int i=0;i<len;i++)
    {
        Array[i]=rand();
    }
     return true;
}


#include <memory.h>

#ifdef UNIT_TEST

int Test_sort()
{
    const int ArrayNum=10*1024;
    int Array[ArrayNum]={0};
    int ArrayTmp[ArrayNum]={0};

    FillRandom(ArrayTmp,ArrayNum);


    memcpy(Array,ArrayTmp,sizeof(ArrayTmp));
    BubbleSort(Array,ArrayNum);
    if (!isRirhtOrder(Array,ArrayNum))
    {
        printf("Error at BubbleSort()");
    }


    return 0;
}

#endif
