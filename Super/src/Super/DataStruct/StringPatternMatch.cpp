#include "StringPatternMatch.h"


/*
Brute-Force算法简称BF算法，即暴风(Brute Force)算法，是普通的模式匹配算法，BF算法的思想就是将目标串S的第一个字符与模式串T的第一个字符进行匹配，若相等，则继续比较S的第二个字符和 T的第二个字符；若不相等，则比较S的第二个字符和T的第一个字符，依次比较下去，直到得出最后的匹配结果。
*/

#include<string>
int StringPatternMatchBy_BruteForce(std::string sSrc, std::string sPattern) 
{
    unsigned int i = 0;
    unsigned int j = 0;
    int pos = -1;  
    while (i < sSrc.length()&&j < sPattern.length())   // 循环比较的条件
    {
        if (sSrc[i] == sPattern[j])  // 如果相等 则继续比较
        {
            i++;
            j++;
        }
        else 
        { // 不相等则让指向主串的“移动指针”i回溯，让指向模式串的“移动指针”j指向初始位置0
            i = i - (j - 1); // 主串的“移动指针”i应该回溯（i-1）个单位
            j = 0;
        }
    }
    // 如果j>=T.length() 则说明匹配成功  那么修改pos
    if (j >= sPattern.length()) 
    {
        pos = i - sPattern.length();
        return pos;
    }
    else   // 否则直接-1
    {
        return -1;
    }
}

