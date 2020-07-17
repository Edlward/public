#ifndef STLHelper_H
#define STLHelper_H

//STL À©Õ¹ ÔÝÊ±


#include <vector>
#include <list>
#include <deque>
#include <map>

template <typename T>
std::vector<T>ListToVector(std::list<T> list)
{
    std::vector<T> Vec;
    Vec.reserve(list.size());

    for (auto it=list.begin();it!=list.end();++it)
    {
        Vec.push_back(*it);
    }
    return Vec;
}

template <typename T>
std::list<T> VectorToList(std::vector<T> Vec)
{
    std::list<T> List;
    for (size_t n=0;n<Vec.size();++n)
    {
        List.push_back(Vec[n]);
    }
    return List;
}


template <typename T>
std::vector<T>DequeToVector(std::deque<T> list)
{
    std::vector<T> Vec;
    Vec.reserve(list.size());
    for (auto it=list.begin();it!=list.end();++it)
    {
        Vec.push_back(*it);
    }
    return Vec;
}

template <typename T>
std::deque<T> VectorToDeque(std::vector<T> Vec)
{
    std::deque<T> Listdeque;
    for (size_t n=0;n<Vec.size();++n)
    {
        Listdeque.push_back(Vec[n]);
    }
    return Listdeque;
}



//TODO£º
template <typename T>
std::deque<T> isExistMapKey()
{
    std::deque<T> Listdeque;
    for (size_t n=0;n<Vec.size();++n)
    {
        Listdeque.push_back(Vec[n]);
    }
    return Listdeque;
}




#endif

