#ifndef ScopedValueRollback_H
#define ScopedValueRollback_H

#include "../Basic/global.h"


template <typename T>
class ScopedValueRollback
{
public:
    explicit ScopedValueRollback(T &var) :
        varRef(var), oldValue(var)
    {
    }

    explicit ScopedValueRollback(T &var, T value) :
        varRef(var), oldValue(var)
    {
        varRef = std::move(value);
    }

    ~ScopedValueRollback()
    {
        varRef = std::move(oldValue);
    }

    void commit()
    {
        oldValue = varRef;
    }

private:
    T& varRef;
    T oldValue;

    S_DISABLE_COPY(ScopedValueRollback)
};


#endif

