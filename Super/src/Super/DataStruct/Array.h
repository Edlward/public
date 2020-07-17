#pragma once

//#include <array>//l


#include <algorithm>
#include <iterator>

using namespace std;

template<class T,size_t m_Size>
class array
{	// fixed size array of values
public:
    enum {ARRAY_SIZE = m_Size};	// helper for expression evaluator
    typedef array<T, m_Size> _Myt;
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T& reference;
    typedef const T& const_reference;

    typedef _Array_iterator<T, m_Size> iterator;
    typedef _Array_const_iterator<T, m_Size> const_iterator;

    typedef _STD reverse_iterator<iterator> reverse_iterator;
    typedef _STD reverse_iterator<const_iterator> const_reverse_iterator;

    void assign(const T& _Value)
    {	// assign value to all elements
        _Fill_n(m_Elems, m_Size, _Value);
    }

    void fill(const T& _Value)
    {	// assign value to all elements
        _Fill_n(m_Elems, m_Size, _Value);
    }

    void swap(_Myt& _Other)
    {	// swap contents with _Other
        _Swap_ranges(_STD addressof(m_Elems[0]),
            _STD addressof(m_Elems[0]) + m_Size,
            _STD addressof(_Other._Elems[0]));
    }

    iterator begin() _NOEXCEPT
    {	// return iterator for beginning of mutable sequence
        return (iterator(_STD addressof(m_Elems[0]), 0));
    }

    const_iterator begin() const _NOEXCEPT
    {	// return iterator for beginning of nonmutable sequence
        return (const_iterator(_STD addressof(m_Elems[0]), 0));
    }

    iterator end() _NOEXCEPT
    {	// return iterator for end of mutable sequence
        return (iterator(_STD addressof(m_Elems[0]), m_Size));
    }

    const_iterator end() const _NOEXCEPT
    {	// return iterator for beginning of nonmutable sequence
        return (const_iterator(_STD addressof(m_Elems[0]), m_Size));
    }

    reverse_iterator rbegin() _NOEXCEPT
    {	// return iterator for beginning of reversed mutable sequence
        return (reverse_iterator(end()));
    }

    const_reverse_iterator rbegin() const _NOEXCEPT
    {	// return iterator for beginning of reversed nonmutable sequence
        return (const_reverse_iterator(end()));
    }

    reverse_iterator rend() _NOEXCEPT
    {	// return iterator for end of reversed mutable sequence
        return (reverse_iterator(begin()));
    }

    const_reverse_iterator rend() const _NOEXCEPT
    {	// return iterator for end of reversed nonmutable sequence
        return (const_reverse_iterator(begin()));
    }

#if _HAS_CPP0X
    const_iterator cbegin() const _NOEXCEPT
    {	// return iterator for beginning of nonmutable sequence
        return (((const _Myt *)this)->begin());
    }

    const_iterator cend() const _NOEXCEPT
    {	// return iterator for end of nonmutable sequence
        return (((const _Myt *)this)->end());
    }

    const_reverse_iterator crbegin() const _NOEXCEPT
    {	// return iterator for beginning of reversed nonmutable sequence
        return (((const _Myt *)this)->rbegin());
    }

    const_reverse_iterator crend() const _NOEXCEPT
    {	// return iterator for end of reversed nonmutable sequence
        return (((const _Myt *)this)->rend());
    }
#endif /* _HAS_CPP0X */

    size_type size() const _NOEXCEPT
    {	// return length of sequence
        return (m_Size);
    }

    size_type max_size() const _NOEXCEPT
    {	// return maximum possible length of sequence
        return (m_Size);
    }

    bool empty() const _NOEXCEPT
    {	// test if sequence is empty
        return (m_Size == 0);
    }

    reference at(size_type _Pos)
    {	// subscript mutable sequence with checking
        if (m_Size <= _Pos)
            _Xran();
        return (m_Elems[_Pos]);
    }

    const_reference at(size_type _Pos) const
    {	// subscript nonmutable sequence with checking
        if (m_Size <= _Pos)
            _Xran();
        return (m_Elems[_Pos]);
    }

    reference operator[](size_type _Pos)
    {	// subscript mutable sequence
#if _ITERATOR_DEBUG_LEVEL == 2
        if (m_Size <= _Pos)
            _DEBUG_ERROR("array subscript out of range");

#elif _ITERATOR_DEBUG_LEVEL == 1
        _SCL_SECURE_VALIDATE_RANGE(_Pos < m_Size);
#endif /* _ITERATOR_DEBUG_LEVEL */

        _Analysis_assume_(_Pos < m_Size);

        return (m_Elems[_Pos]);
    }

    const_reference operator[](size_type _Pos) const
    {	// subscript nonmutable sequence
#if _ITERATOR_DEBUG_LEVEL == 2
        if (m_Size <= _Pos)
            _DEBUG_ERROR("array subscript out of range");

#elif _ITERATOR_DEBUG_LEVEL == 1
        _SCL_SECURE_VALIDATE_RANGE(_Pos < m_Size);
#endif /* _ITERATOR_DEBUG_LEVEL */

        _Analysis_assume_(_Pos < m_Size);

        return (m_Elems[_Pos]);
    }

    reference front()
    {	// return first element of mutable sequence
        return (m_Elems[0]);
    }

    const_reference front() const
    {	// return first element of nonmutable sequence
        return (m_Elems[0]);
    }

    reference back()
    {	// return last element of mutable sequence
        return (m_Elems[m_Size - 1]);
    }

    const_reference back() const
    {	// return last element of nonmutable sequence
        return (m_Elems[m_Size - 1]);
    }

    T *data() _NOEXCEPT
    {	// return pointer to mutable data array
        return (m_Elems);
    }

    const T *data() const _NOEXCEPT
    {	// return pointer to nonmutable data array
        return (m_Elems);
    }

    __declspec(noreturn) void _Xran() const
    {	// report an out_of_range error
        _Xout_of_range("invalid array<T, N> subscript");
    }

    T m_Elems[m_Size == 0 ? 1 : m_Size];
};
