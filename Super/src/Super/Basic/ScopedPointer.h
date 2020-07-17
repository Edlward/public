#ifndef SCOPEDPOINTER_H
#define SCOPEDPOINTER_H

//#include <QtCore/qglobal.h>
//from qt remove the depdends   仅供学习使用


#include "../Basic/global.h"
#include <stdlib.h>


template <typename T>
struct ScopedPointerDeleter
{
    static inline void cleanup(T *pointer)
    {
        // Enforce a complete type.
        // If you get a compile error here, read the section on forward declared
        // classes in the QScopedPointer documentation.
        typedef char IsIncompleteType[ sizeof(T) ? 1 : -1 ];
        (void) sizeof(IsIncompleteType);

        delete pointer;
    }
};

template <typename T>
struct ScopedPointerArrayDeleter
{
    static inline void cleanup(T *pointer)
    {
        // Enforce a complete type.
        // If you get a compile error here, read the section on forward declared
        // classes in the QScopedPointer documentation.
        typedef char IsIncompleteType[ sizeof(T) ? 1 : -1 ];
        (void) sizeof(IsIncompleteType);

        delete [] pointer;
    }
};

struct ScopedPointerPodDeleter
{
    static inline void cleanup(void *pointer) { if (pointer) free(pointer); }
};

#ifndef QT_NO_QOBJECT
template <typename T>
struct QScopedPointerObjectDeleteLater
{
    static inline void cleanup(T *pointer) { if (pointer) pointer->deleteLater(); }
};

class QObject;
typedef QScopedPointerObjectDeleteLater<QObject> QScopedPointerDeleteLater;
#endif

template <typename T, typename Cleanup = ScopedPointerDeleter<T> >
class ScopedPointer
{
    typedef T *ScopedPointer:: *RestrictedBool;
public:
    explicit ScopedPointer(T *p = nullptr):d(p)
    {
    }

    inline ~ScopedPointer()
    {
        T *oldD = this->d;
        Cleanup::cleanup(oldD);
    }

    inline T &operator*() const
    {
        S_ASSERT(d);
        return *d;
    }

    T *operator->() const
    {
        return d;
    }

    bool operator!() const
    {
        return !d;
    }

#if defined(Q_QDOC)
    inline operator bool() const
    {
        return isNull() ? nullptr : &ScopedPointer::d;
    }
#else
    operator RestrictedBool() const 
    {
        return isNull() ? nullptr : &ScopedPointer::d;
    }
#endif

    T *data() const 
    {
        return d;
    }

    T *get() const 
    {
        return d;
    }

    bool isNull() const 
    {
        return !d;
    }

    void reset(T *other = nullptr) //Q_DECL_NOEXCEPT_EXPR(noexcept(Cleanup::cleanup(std::declval<T *>())))
    {
        if (d == other)
            return;
        T *oldD = d;
        d = other;
        Cleanup::cleanup(oldD);
    }

    T *take() 
    {
        T *oldD = d;
        d = nullptr;
        return oldD;
    }

    void swap(ScopedPointer<T, Cleanup> &other) 
    {
        qSwap(d, other.d);
    }

    typedef T *pointer;

protected:
    T *d;

private:
    S_DISABLE_COPY(ScopedPointer)
};

template <class T, class Cleanup>
inline bool operator==(const ScopedPointer<T, Cleanup> &lhs, const ScopedPointer<T, Cleanup> &rhs) 
{
    return lhs.data() == rhs.data();
}

template <class T, class Cleanup>
inline bool operator!=(const ScopedPointer<T, Cleanup> &lhs, const ScopedPointer<T, Cleanup> &rhs) 
{
    return lhs.data() != rhs.data();
}

template <class T, class Cleanup>
inline bool operator==(const ScopedPointer<T, Cleanup> &lhs, std::nullptr_t) 
{
    return lhs.isNull();
}

template <class T, class Cleanup>
inline bool operator==(std::nullptr_t, const ScopedPointer<T, Cleanup> &rhs) 
{
    return rhs.isNull();
}

template <class T, class Cleanup>
inline bool operator!=(const ScopedPointer<T, Cleanup> &lhs, std::nullptr_t) 
{
    return !lhs.isNull();
}

template <class T, class Cleanup>
inline bool operator!=(std::nullptr_t, const ScopedPointer<T, Cleanup> &rhs) 
{
    return !rhs.isNull();
}

template <class T, class Cleanup>
inline void swap(ScopedPointer<T, Cleanup> &p1, ScopedPointer<T, Cleanup> &p2) 
{ p1.swap(p2); }

template <typename T, typename Cleanup = ScopedPointerArrayDeleter<T> >
class ScopedArrayPointer : public ScopedPointer<T, Cleanup>
{

    //VS2012编译通不过，修改写法待续
    //template <typename Ptr>
    // if_same_type = typename std::enable_if<std::is_same<typename std::remove_cv<T>::type,Ptr>::value,bool>::type;

public:
    inline ScopedArrayPointer() : ScopedPointer<T, Cleanup>(nullptr) {}

    //template <typename D, if_same_type<D> = true>
    //explicit QScopedArrayPointer(D *p)
    //    : QScopedPointer<T, Cleanup>(p)
    //{
    //}

    inline T &operator[](int i)
    {
        return this->d[i];
    }

    inline const T &operator[](int i) const
    {
        return this->d[i];
    }

    void swap(ScopedArrayPointer &other)  // prevent QScopedPointer <->QScopedArrayPointer swaps
    { ScopedPointer<T, Cleanup>::swap(other); }

private:
    explicit inline ScopedArrayPointer(void *) {
        // Enforce the same type.

        // If you get a compile error here, make sure you declare
        // QScopedArrayPointer with the same template type as you pass to the
        // constructor. See also the QScopedPointer documentation.

        // Storing a scalar array as a pointer to a different type is not
        // allowed and results in undefined behavior.
    }

    S_DISABLE_COPY(ScopedArrayPointer)
};

template <typename T, typename Cleanup>
inline void swap(ScopedArrayPointer<T, Cleanup> &lhs, ScopedArrayPointer<T, Cleanup> &rhs) 
{ lhs.swap(rhs); }


















/* Internal helper class - exposes the data through data_ptr (legacy from QShared).
   Required for some internal Qt classes, do not use otherwise. */
template <typename T, typename Cleanup = ScopedPointerDeleter<T> >
class CustomScopedPointer : public ScopedPointer<T, Cleanup>
{
public:
    explicit inline CustomScopedPointer(T *p = 0)
        : ScopedPointer<T, Cleanup>(p)
    {
    }

    inline T *&data_ptr()
    {
        return this->d;
    }

    inline bool operator==(const CustomScopedPointer<T, Cleanup> &other) const
    {
        return this->d == other.d;
    }

    inline bool operator!=(const CustomScopedPointer<T, Cleanup> &other) const
    {
        return this->d != other.d;
    }

private:
    S_DISABLE_COPY(CustomScopedPointer)
};

/* Internal helper class - a handler for QShared* classes, to be used in QCustomScopedPointer */
template <typename T>
class ScopedPointerSharedDeleter
{
public:
    static inline void cleanup(T *d)
    {
        if (d && !d->ref.deref())
            delete d;
    }
};

/* Internal.
   This class is basically a scoped pointer pointing to a ref-counted object
 */
template <typename T>
class ScopedSharedPointer : public CustomScopedPointer<T, ScopedPointerSharedDeleter<T> >
{
public:
    explicit inline ScopedSharedPointer(T *p = 0)
        : CustomScopedPointer<T, ScopedPointerSharedDeleter<T> >(p)
    {
    }

    inline void detach()
    {
        qAtomicDetach(this->d);
    }

    inline void assign(T *other)
    {
        if (this->d == other)
            return;
        if (other)
            other->ref.ref();
        T *oldD = this->d;
        this->d = other;
        ScopedPointerSharedDeleter<T>::cleanup(oldD);
    }

    inline bool operator==(const ScopedSharedPointer<T> &other) const
    {
        return this->d == other.d;
    }

    inline bool operator!=(const ScopedSharedPointer<T> &other) const
    {
        return this->d != other.d;
    }

private:
    S_DISABLE_COPY(ScopedSharedPointer)
};








#endif //

