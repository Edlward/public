#include "BigInt.h"

#include <vector>
#include <math.h>   //for pow

//————————————————
//    版权声明：本文为CSDN博主「MaplePlane」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/MaplePlane/article/details/98258785


//简单函数实现


std::vector<int> mul(std::vector<int>a, int b)
{
    std::vector<int>c;
    int t = 0;
    for (int i = a.size() - 1; i >= 0; i--)
    {
        t += a[i] * b;
        c.insert(c.begin(), t % 10);
        t = t / 10;
    }
    if (t > 0)
        c.insert(c.begin(), t);
    return c;
}

std::vector<int> div(std::vector<int>a, int b)
{
    std::vector<int>c;
    bool is_first = true;
    int t = 0;
    int x = 0;
    for (int i = 0; i < a.size(); i++)
    {
        t = t * 10 + a[i];
        x = t / b;
        if (!is_first || x)
        {
            c.push_back(x);
            is_first = false;
        }
        t = t % b;
    }
    return c;
}

std::vector<int>Plus(std::vector<int>a, int b)
{
    std::vector<int>c;
    int t = b;
    if (t == 0) return a;
    int i = a.size() - 1;
    for (i; i >= 0; i--)
    {
        t = t + a[i];
        c.insert(c.begin(), t % 10);
        t = t / 10;
        if (t == 0)
            break;
    }
    if (i != -1)
        c.insert(c.begin(), a.begin(), a.begin() + i);
    else
    {
        while (t)
        {
            c.insert(c.begin(), t % 10);
            t = t / 10;
        }
    }
    return c;
}

std::vector<int>reduce(std::vector<int>a, int b)
{
    std::vector<int>c;
    int t = 0;
    if (b == 0)
        return a;
    bool is_true = true;
    int i;
    int p = 0;
    for (i = a.size() - 1; i >= 0; i--)
    {
        if (is_true == false)break;
        t = t + a[i] * pow(10, p);
        p++;
        if (t > b)
        {
            c.push_back(t - b);
            is_true = false;
        }
    }
    if (i != -1)
        c.insert(c.begin(), a.begin(), a.begin() + i + 1);
    return c;
}







//vector对vector完善实现待续  std::vector<int>改char? 虽然不超过10但是int更高效 by lizulin？

std::vector<int> add(const std::vector<int> &A,const std::vector<int> &B);
std::vector<int> sub(const std::vector<int> &A,const std::vector<int> &B);
std::vector<int> mul(const std::vector<int> &A,const std::vector<int> &B);
std::vector<int> div(const std::vector<int> &A,const std::vector<int> &B);


std::vector<int> add(const std::vector<int> &A,const std::vector<int> &B)
{
    std::vector<int> C;
    int t=0;
    for(int i=0;i<A.size()||i<B.size();i++)
    {
        if(i<A.size()) t+=A[i];
        if(i<B.size()) t+=B[i];
        C.push_back(t%10);
        t/=10;
    }
    if(t) C.push_back(1);
    return C;
}


#ifdef UNIT_TEST

int Test_BigInt()
{
    std::vector<int> A,B;
    std::string a,b;
    std::cin>>a>>b;
    for(int i=a.size()-1;i>=0;i--) A.push_back(a[i]-'0');
    for(int i=b.size()-1;i>=0;i--) B.push_back(b[i]-'0');

    auto C=add(A,B);//接受C数组

    for(int i=C.size()-1;i>=0;i--) printf("%d",C[i]);//从后往前输出
    return 0; 
}

#endif

