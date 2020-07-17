/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef SharedData_H
#define SharedData_H

//#include <QtCore/qglobal.h>
//#include <QtCore/qatomic.h>
//#if QT_DEPRECATED_SINCE(5, 6)
//#include <QtCore/qhash.h>
//#endif
//#include <QtCore/qhashfunctions.h>
//
//QT_BEGIN_NAMESPACE


#include "../Basic/global.h"

#define Q_UNUSED(x) (void)x;


//新增用std::atomic 模拟QAtomicInt
#include <atomic>
class SAtomicInt
{
public:
    SAtomicInt(int value):count(value)
    {

    }
    int load()
    {
        return count;
    }
    void ref() // 增加要使用的共享数据的引用计数器的值  
    {
        count++;
    }
    int deref()  // 减少当前共享数据的引用计数器的值 
    {
        return count--;
    }
    std::atomic<int> count;
};




template <class T> class SharedDataPointer;

class  SharedData
{
public:
    //mutable QAtomicInt ref;
    mutable SAtomicInt ref;

    inline SharedData() : ref(0) { }
    inline SharedData(const SharedData &) : ref(0) { }

private:
    // using the assignment operator would lead to corruption in the ref-counting
    SharedData &operator=(const SharedData &);
};

template <class T> class SharedDataPointer
{
public:
    typedef T Type;
    typedef T *pointer;

    inline void detach() { if (d && d->ref.load() != 1) detach_helper(); }
    inline T &operator*() { detach(); return *d; }
    inline const T &operator*() const { return *d; }
    inline T *operator->() { detach(); return d; }
    inline const T *operator->() const { return d; }
    inline operator T *() { detach(); return d; }
    inline operator const T *() const { return d; }
    inline T *data() { detach(); return d; }
    inline const T *data() const { return d; }
    inline const T *constData() const { return d; }

    inline bool operator==(const SharedDataPointer<T> &other) const { return d == other.d; }
    inline bool operator!=(const SharedDataPointer<T> &other) const { return d != other.d; }

    inline SharedDataPointer() { d = nullptr; }
    inline ~SharedDataPointer() { if (d && !d->ref.deref()) delete d; }

    explicit SharedDataPointer(T *data);
    inline SharedDataPointer(const SharedDataPointer<T> &o) : d(o.d) { if (d) d->ref.ref(); }
    inline SharedDataPointer<T> & operator=(const SharedDataPointer<T> &o) {
        if (o.d != d) {
            if (o.d)
                o.d->ref.ref();
            T *old = d;
            d = o.d;
            if (old && !old->ref.deref())
                delete old;
        }
        return *this;
    }
    inline SharedDataPointer &operator=(T *o) {
        if (o != d) {
            if (o)
                o->ref.ref();
            T *old = d;
            d = o;
            if (old && !old->ref.deref())
                delete old;
        }
        return *this;
    }
#ifdef Q_COMPILER_RVALUE_REFS
    SharedDataPointer(SharedDataPointer &&o) : d(o.d) { o.d = nullptr; }
    inline SharedDataPointer<T> &operator=(SharedDataPointer<T> &&other)
    {
        SharedDataPointer moved(std::move(other));
        swap(moved);
        return *this;
    }
#endif

    inline bool operator!() const { return !d; }

    inline void swap(SharedDataPointer &other)
    { qSwap(d, other.d); }

protected:
    T *clone();

private:
    void detach_helper();

    T *d;
};

template <class T> inline bool operator==(std::nullptr_t p1, const SharedDataPointer<T> &p2)
{
    Q_UNUSED(p1);
    return !p2;
}

template <class T> inline bool operator==(const SharedDataPointer<T> &p1, std::nullptr_t p2)
{
    Q_UNUSED(p2);
    return !p1;
}

template <class T> class QExplicitlySharedDataPointer
{
public:
    typedef T Type;
    typedef T *pointer;

    inline T &operator*() const { return *d; }
    inline T *operator->() { return d; }
    inline T *operator->() const { return d; }
    inline T *data() const { return d; }
    inline const T *constData() const { return d; }
    inline T *take() { T *x = d; d = nullptr; return x; }

    inline void detach() { if (d && d->ref.load() != 1) detach_helper(); }

    inline void reset()
    {
        if(d && !d->ref.deref())
            delete d;

        d = nullptr;
    }

    inline operator bool () const { return d != nullptr; }

    inline bool operator==(const QExplicitlySharedDataPointer<T> &other) const { return d == other.d; }
    inline bool operator!=(const QExplicitlySharedDataPointer<T> &other) const { return d != other.d; }
    inline bool operator==(const T *ptr) const { return d == ptr; }
    inline bool operator!=(const T *ptr) const { return d != ptr; }

    inline QExplicitlySharedDataPointer() { d = nullptr; }
    inline ~QExplicitlySharedDataPointer() { if (d && !d->ref.deref()) delete d; }

    explicit QExplicitlySharedDataPointer(T *data);
    inline QExplicitlySharedDataPointer(const QExplicitlySharedDataPointer<T> &o) : d(o.d) { if (d) d->ref.ref(); }

    template<class X>
    inline QExplicitlySharedDataPointer(const QExplicitlySharedDataPointer<X> &o)
#ifdef QT_ENABLE_QEXPLICITLYSHAREDDATAPOINTER_STATICCAST
        : d(static_cast<T *>(o.data()))
#else
        : d(o.data())
#endif
    {
        if(d)
            d->ref.ref();
    }

    inline QExplicitlySharedDataPointer<T> & operator=(const QExplicitlySharedDataPointer<T> &o) {
        if (o.d != d) {
            if (o.d)
                o.d->ref.ref();
            T *old = d;
            d = o.d;
            if (old && !old->ref.deref())
                delete old;
        }
        return *this;
    }
    inline QExplicitlySharedDataPointer &operator=(T *o) {
        if (o != d) {
            if (o)
                o->ref.ref();
            T *old = d;
            d = o;
            if (old && !old->ref.deref())
                delete old;
        }
        return *this;
    }
#ifdef Q_COMPILER_RVALUE_REFS
    inline QExplicitlySharedDataPointer(QExplicitlySharedDataPointer &&o) : d(o.d) { o.d = nullptr; }
    inline QExplicitlySharedDataPointer<T> &operator=(QExplicitlySharedDataPointer<T> &&other)
    {
        QExplicitlySharedDataPointer moved(std::move(other));
        swap(moved);
        return *this;
    }
#endif

    inline bool operator!() const { return !d; }

    inline void swap(QExplicitlySharedDataPointer &other)
    { qSwap(d, other.d); }

protected:
    T *clone();

private:
    void detach_helper();

    T *d;
};

template <class T>
inline  SharedDataPointer<T>::SharedDataPointer(T *adata)
    : d(adata)
{ if (d) d->ref.ref(); }

template <class T>
inline  T *SharedDataPointer<T>::clone()
{
    return new T(*d);
}

template <class T>
inline void SharedDataPointer<T>::detach_helper()
{
    T *x = clone();
    x->ref.ref();
    if (!d->ref.deref())
        delete d;
    d = x;
}

template <class T>
inline  T *QExplicitlySharedDataPointer<T>::clone()
{
    return new T(*d);
}

template <class T>
inline  void QExplicitlySharedDataPointer<T>::detach_helper()
{
    T *x = clone();
    x->ref.ref();
    if (!d->ref.deref())
        delete d;
    d = x;
}

template <class T>
inline  QExplicitlySharedDataPointer<T>::QExplicitlySharedDataPointer(T *adata)
    : d(adata)
{ if (d) d->ref.ref(); }

template <class T> inline bool operator==(std::nullptr_t p1, const QExplicitlySharedDataPointer<T> &p2)
{
    Q_UNUSED(p1);
    return !p2;
}

template <class T> inline bool operator==(const QExplicitlySharedDataPointer<T> &p1, std::nullptr_t p2)
{
    Q_UNUSED(p2);
    return !p1;
}

template <class T>
inline  void qSwap(SharedDataPointer<T> &p1, SharedDataPointer<T> &p2)
{ p1.swap(p2); }

template <class T>
inline  void qSwap(QExplicitlySharedDataPointer<T> &p1, QExplicitlySharedDataPointer<T> &p2)
{ p1.swap(p2); }

//QT_END_NAMESPACE
namespace std {
    template <class T>
    inline  void swap(SharedDataPointer<T> &p1,SharedDataPointer<T> &p2)
    { p1.swap(p2); }

    template <class T>
    inline  void swap(QExplicitlySharedDataPointer<T> &p1,QExplicitlySharedDataPointer<T> &p2)
    { p1.swap(p2); }
}
//QT_BEGIN_NAMESPACE

template <class T>
inline  uint qHash(const SharedDataPointer<T> &ptr, uint seed = 0)
{
    return qHash(ptr.data(), seed);
}
template <class T>
inline  uint qHash(const QExplicitlySharedDataPointer<T> &ptr, uint seed = 0)
{
    return qHash(ptr.data(), seed);
}

//template<typename T> Q_DECLARE_TYPEINFO_BODY(QSharedDataPointer<T>, Q_MOVABLE_TYPE);
//template<typename T> Q_DECLARE_TYPEINFO_BODY(QExplicitlySharedDataPointer<T>, Q_MOVABLE_TYPE);

//QT_END_NAMESPACE

#endif // QSHAREDDATA_H
