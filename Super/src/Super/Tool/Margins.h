#ifndef MARGINS_Header
#define MARGINS_Header

#include "../Basic/global.h"


class Margins
{
public:
    Margins()  : m_left(0), m_top(0), m_right(0), m_bottom(0) {}
    Margins(int aleft, int atop, int aright, int abottom) 
        : m_left(aleft), m_top(atop), m_right(aright), m_bottom(abottom) {}

    bool isNull() const { return m_left==0 && m_top==0 && m_right==0 && m_bottom==0; }

    int left() const { return m_left; }

    int top() const { return m_top; }

    int right() const { return m_right; }

    int bottom() const { return m_bottom; }


    void setLeft(int aleft) { m_left = aleft; }

    void setTop(int atop) { m_top = atop; }

    void setRight(int aright) { m_right = aright; }

    void setBottom(int abottom) { m_bottom = abottom; }


    inline Margins &operator+=(const Margins &margins);
    inline Margins &operator-=(const Margins &margins);
    inline Margins &operator+=(int margin);

    Margins &operator-=(int margin) 
    {
        m_left -= margin;
        m_top -= margin;
        m_right -= margin;
        m_bottom -= margin;
        return *this;
    }

    inline Margins &operator*=(int factor);
    inline Margins &operator/=(int divisor);
    inline Margins &operator*=(qreal factor);
    inline Margins &operator/=(qreal divisor);

private:
    int m_left;
    int m_top;
    int m_right;
    int m_bottom;

    friend  inline bool operator==(const Margins &, const Margins &) ;
    friend  inline bool operator!=(const Margins &, const Margins &) ;
};


 inline bool operator==(const Margins &m1, const Margins &m2) 
{
    return
            m1.m_left == m2.m_left &&
            m1.m_top == m2.m_top &&
            m1.m_right == m2.m_right &&
            m1.m_bottom == m2.m_bottom;
}

 inline bool operator!=(const Margins &m1, const Margins &m2) 
{
    return
            m1.m_left != m2.m_left ||
            m1.m_top != m2.m_top ||
            m1.m_right != m2.m_right ||
            m1.m_bottom != m2.m_bottom;
}

 inline Margins operator+(const Margins &m1, const Margins &m2) 
{
    return Margins(m1.left() + m2.left(), m1.top() + m2.top(),
                    m1.right() + m2.right(), m1.bottom() + m2.bottom());
}

 inline Margins operator-(const Margins &m1, const Margins &m2) 
{
    return Margins(m1.left() - m2.left(), m1.top() - m2.top(),
                    m1.right() - m2.right(), m1.bottom() - m2.bottom());
}

 inline Margins operator+(const Margins &lhs, int rhs) 
{
    return Margins(lhs.left() + rhs, lhs.top() + rhs,
                    lhs.right() + rhs, lhs.bottom() + rhs);
}

 inline Margins operator+(int lhs, const Margins &rhs) 
{
    return Margins(rhs.left() + lhs, rhs.top() + lhs,
                    rhs.right() + lhs, rhs.bottom() + lhs);
}

 inline Margins operator-(const Margins &lhs, int rhs) 
{
    return Margins(lhs.left() - rhs, lhs.top() - rhs,
                    lhs.right() - rhs, lhs.bottom() - rhs);
}

 inline Margins operator*(const Margins &margins, int factor) 
{
    return Margins(margins.left() * factor, margins.top() * factor,
                    margins.right() * factor, margins.bottom() * factor);
}

 inline Margins operator*(int factor, const Margins &margins) 
{
    return Margins(margins.left() * factor, margins.top() * factor,
                    margins.right() * factor, margins.bottom() * factor);
}

 inline Margins operator*(const Margins &margins, qreal factor) 
{
    return Margins(Round(margins.left() * factor),Round(margins.top() * factor),
                    Round(margins.right() * factor),Round(margins.bottom() * factor));
}

 inline Margins operator*(qreal factor, const Margins &margins) 
{
    return Margins(Round(margins.left() * factor),Round(margins.top() * factor),
                   Round(margins.right() * factor),Round(margins.bottom() * factor));
}

 inline Margins operator/(const Margins &margins, int divisor)
{
    return Margins(margins.left() / divisor, margins.top() / divisor,
                    margins.right() / divisor, margins.bottom() / divisor);
}

 inline Margins operator/(const Margins &margins, qreal divisor)
{
    return Margins(Round(margins.left() / divisor),Round(margins.top() / divisor),
                    Round(margins.right() / divisor),Round(margins.bottom() / divisor));
}


 inline Margins operator+(const Margins &margins) 
{
    return margins;
}

 inline Margins operator-(const Margins &margins) 
{
    return Margins(-margins.left(), -margins.top(), -margins.right(), -margins.bottom());
}


inline Margins & Margins::operator+=(const Margins &margins)
 {
     return *this = *this + margins;
 }

inline Margins & Margins::operator+=(int margin)
 {
     m_left += margin;
     m_top += margin;
     m_right += margin;
     m_bottom += margin;
     return *this;
 }

inline Margins & Margins::operator-=(const Margins &margins)
 {
     return *this = *this - margins;
 }

inline Margins & Margins::operator*=(int factor)
 {
     return *this = *this * factor;
 }

inline Margins & Margins::operator*=(qreal factor)
 {
     return *this = *this * factor;
 }

inline Margins & Margins::operator/=(int divisor)
 {
     return *this = *this / divisor;
 }

inline Margins & Margins::operator/=(qreal divisor)
 {
     return *this = *this / divisor;
 }





class MarginsF
{
public:

    MarginsF() 
        : m_left(0), m_top(0), m_right(0), m_bottom(0) {}

    MarginsF(qreal aleft, qreal atop, qreal aright, qreal abottom) 
        : m_left(aleft), m_top(atop), m_right(aright), m_bottom(abottom) {}

    MarginsF(const Margins &margins) 
        : m_left(margins.left()), m_top(margins.top()), m_right(margins.right()), m_bottom(margins.bottom()) {}

    bool isNull() const 
    { return FuzzyIsNull(m_left) && FuzzyIsNull(m_top) && FuzzyIsNull(m_right) && FuzzyIsNull(m_bottom); }

    qreal left() const { return m_left; }

    qreal top() const { return m_top; }

    qreal right() const { return m_right; }

    qreal bottom() const { return m_bottom; }

    void setLeft(qreal aleft) { m_left = aleft; }

    void setTop(qreal atop) { m_top = atop; }

    void setRight(qreal aright) { m_right = aright; }

    void setBottom(qreal abottom) { m_bottom = abottom; }


    inline MarginsF &operator+=(const MarginsF &margins);

    inline MarginsF &operator-=(const MarginsF &margins);

    inline MarginsF &operator+=(qreal addend);

    inline MarginsF &operator-=(qreal subtrahend);

    inline MarginsF &operator*=(qreal factor);

    inline MarginsF &operator/=(qreal divisor);

    Margins toMargins() const 
    {
        return Margins(Round(m_left),Round(m_top),Round(m_right),Round(m_bottom));
    }

private:
    qreal m_left;
    qreal m_top;
    qreal m_right;
    qreal m_bottom;
};


 inline bool operator==(const MarginsF &lhs, const MarginsF &rhs) 
{
    return FuzzyCompare(lhs.left(), rhs.left())
           && FuzzyCompare(lhs.top(), rhs.top())
           && FuzzyCompare(lhs.right(), rhs.right())
           && FuzzyCompare(lhs.bottom(), rhs.bottom());
}

 inline bool operator!=(const MarginsF &lhs, const MarginsF &rhs) 
{
    return !operator==(lhs, rhs);
}

 inline MarginsF operator+(const MarginsF &lhs, const MarginsF &rhs) 
{
    return MarginsF(lhs.left() + rhs.left(), lhs.top() + rhs.top(),
                     lhs.right() + rhs.right(), lhs.bottom() + rhs.bottom());
}

 inline MarginsF operator-(const MarginsF &lhs, const MarginsF &rhs) 
{
    return MarginsF(lhs.left() - rhs.left(), lhs.top() - rhs.top(),
                     lhs.right() - rhs.right(), lhs.bottom() - rhs.bottom());
}

 inline MarginsF operator+(const MarginsF &lhs, qreal rhs) 
{
    return MarginsF(lhs.left() + rhs, lhs.top() + rhs,
                     lhs.right() + rhs, lhs.bottom() + rhs);
}

 inline MarginsF operator+(qreal lhs, const MarginsF &rhs) 
{
    return MarginsF(rhs.left() + lhs, rhs.top() + lhs,
                     rhs.right() + lhs, rhs.bottom() + lhs);
}

 inline MarginsF operator-(const MarginsF &lhs, qreal rhs) 
{
    return MarginsF(lhs.left() - rhs, lhs.top() - rhs,
                     lhs.right() - rhs, lhs.bottom() - rhs);
}

 inline MarginsF operator*(const MarginsF &lhs, qreal rhs) 
{
    return MarginsF(lhs.left() * rhs, lhs.top() * rhs,
                     lhs.right() * rhs, lhs.bottom() * rhs);
}

 inline MarginsF operator*(qreal lhs, const MarginsF &rhs) 
{
    return MarginsF(rhs.left() * lhs, rhs.top() * lhs,
                     rhs.right() * lhs, rhs.bottom() * lhs);
}

 inline MarginsF operator/(const MarginsF &lhs, qreal divisor)
{
    return MarginsF(lhs.left() / divisor, lhs.top() / divisor,
                     lhs.right() / divisor, lhs.bottom() / divisor);
}

 inline MarginsF operator+(const MarginsF &margins) 
{
    return margins;
}

 inline MarginsF operator-(const MarginsF &margins) 
{
    return MarginsF(-margins.left(), -margins.top(), -margins.right(), -margins.bottom());
}


  inline MarginsF & MarginsF::operator+=(const MarginsF &margins)
 {
     return *this = *this + margins;
 }

  inline MarginsF & MarginsF::operator+=(qreal addend)
 {
     m_left += addend;
     m_top += addend;
     m_right += addend;
     m_bottom += addend;
     return *this;
 }

  inline MarginsF & MarginsF::operator-=(const MarginsF &margins)
 {
     return *this = *this - margins;
 }

  inline MarginsF & MarginsF::operator-=(qreal subtrahend)
 {
     m_left -= subtrahend;
     m_top -= subtrahend;
     m_right -= subtrahend;
     m_bottom -= subtrahend;
     return *this;
 }

 inline MarginsF & MarginsF::operator*=(qreal factor)
 {
     return *this = *this * factor;
 }

inline MarginsF & MarginsF::operator/=(qreal divisor)
 {
     return *this = *this / divisor;
 }


#endif

