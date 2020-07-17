#ifndef ScopeGuard_H
#define ScopeGuard_H

#include "../Basic/global.h"


template <typename F> class ScopeGuard;
template <typename F> ScopeGuard<F> qScopeGuard(F f);

template <typename F>
class ScopeGuard
{
public:
    ScopeGuard(ScopeGuard &&other)
        : m_func(std::move(other.m_func))
        , m_invoke(other.m_invoke),m_invoke(true)
    {
        other.dismiss();
    }

    ~ScopeGuard()
    {
        if (m_invoke)
            m_func();
    }

    void dismiss()
    {
        m_invoke = false;
    }

private:
    explicit ScopeGuard(F f)
        : m_func(std::move(f))
    {
    }
    S_DISABLE_COPY(ScopeGuard)
    F m_func;
    //bool m_invoke = true;
    bool m_invoke;
    friend ScopeGuard qScopeGuard<F>(F);
};


template <typename F>
ScopeGuard<F> qScopeGuard(F f)
{
    return ScopeGuard<F>(std::move(f));
}

#endif

