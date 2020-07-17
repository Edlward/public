#ifndef _Search_H_
#define _Search_H_



//˳�����
//����˼�룺˳�����Ҳ��Ϊ���Բ��ң�������������㷨�������ݽṹ���Ա��һ�˿�ʼ��˳��ɨ�裬���ν�ɨ�赽�Ľ��ؼ��������ֵk��Ƚϣ���������ʾ���ҳɹ�����ɨ�������û���ҵ��ؼ��ֵ���k�Ľ�㣬��ʾ����ʧ�ܡ�
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


//���ֲ��ң��۰���ң����汾1
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


//���ֲ��ң��ݹ�汾
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


//��ֵ����
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

//쳲��������� ����װ

//���β��ң��������������

//�ֿ���� 

//��ϣ����--��hash


int Test_Search();

#endif




