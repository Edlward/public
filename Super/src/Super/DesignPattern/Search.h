#ifndef _Search_H_
#define _Search_H_



//顺序查找
//基本思想：顺序查找也称为线性查找，属于无序查找算法。从数据结构线性表的一端开始，顺序扫描，依次将扫描到的结点关键字与给定值k相比较，若相等则表示查找成功；若扫描结束仍没有找到关键字等于k的结点，表示查找失败。
int SequentialSearch(int array[], int value, int len)
{
    int i;
    for(i=0; i<len; i++)
    {
        if(array[i]==value)
        {
            return i;
        }
    }
    return -1;
}


//二分查找（折半查找），版本1
int BinarySearch(int array[], int value, int len)
{
    int low, high, mid;
    low = 0;
    high = len-1;
    while(low<=high)
    {
        mid = (low+high)/2;
        if(array[mid]==value)
            return mid;
        if(array[mid]>value)
            high = mid-1;
        if(array[mid]<value)
            low = mid+1;
    }
    return -1;
}


//二分查找，递归版本
int BinarySearch_useRecursive(int array[], int value, int low, int high)
{
    int mid = low+(high-low)/2;
    if(array[mid]==value)
        return mid;
    if(array[mid]>value)
        return BinarySearch_useRecursive(array, value, low, mid-1);
    if(array[mid]<value)
        return BinarySearch_useRecursive(array, value, mid+1, high);
}


//插值查找
int InsertionSearch(int array[], int value, int low, int high)
{
    int mid = low+(value-array[low])/(array[high]-array[low])*(high-low);
    if(array[mid]==value)
        return mid;
    if(array[mid]>value)
        return InsertionSearch(array, value, low, mid-1);
    if(array[mid]<value)
        return InsertionSearch(array, value, mid+1, high);
}

//斐波那契查找 待封装

//树形查找：二叉树，红黑树

//分块查找 

//哈希查找--见hash


int Test_Search();

#endif




