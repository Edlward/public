#ifndef SIZE_Header
#define SIZE_Header

#include "../Basic/global.h"


class  Size
{
public:
    Size()  : wd(-1), ht(-1) {}
    Size(int w, int h)  : wd(w), ht(h) {}

    bool isNull() const { return wd==0 && ht==0; }
    bool isEmpty() const { return wd<1 || ht<1; }

    bool isValid() const { return wd>=0 && ht>=0; }
    int width() const { return wd; }
    int height() const { return ht; }

    void setWidth(int w) { wd = w; }
    void setHeight(int h) { ht = h; }
    Size transposed() const { return Size(ht, wd); }
    void transpose(){ Swap(wd, ht);}

    void scale(int w, int h, Sp::AspectRatioMode mode) { scale(Size(w, h), mode); }
    void scale(const Size &s,Sp::AspectRatioMode mode) { *this = scaled(s, mode); }

    Size scaled(int w, int h, Sp::AspectRatioMode mode) const { return scaled(Size(w, h), mode); }
    Size scaled(const Size &s, Sp::AspectRatioMode mode) const
    {
        if (mode == Sp::IgnoreAspectRatio || wd == 0 || ht == 0) {
            return s;
        } else {
            bool useHeight;
            int64_t rw = int64_t(s.ht) * int64_t(wd) /int64_t(ht);

            if (mode == Sp::KeepAspectRatio) {
                useHeight = (rw <= s.wd);
            } else { // mode == Sp::KeepAspectRatioByExpanding
                useHeight = (rw >= s.wd);
            }

            if (useHeight) {
                return Size(rw, s.ht);
            } else {
                return Size(s.wd,
                    int32_t(int64_t(s.wd) * int64_t(ht) / int64_t(wd)));
            }
        }
    }

    Size expandedTo(const Size & otherSize) const 
    {
        return Size(Max(wd,otherSize.wd),Max(ht,otherSize.ht));
    }

    Size boundedTo(const Size & otherSize) const 
    {
        return Size(Min(wd,otherSize.wd),Min(ht,otherSize.ht));
    }

    int &rwidth() { return wd; }
    int &rheight() { return ht; }

    Size &operator+=(const Size &s) { wd+=s.wd; ht+=s.ht; return *this; }
    Size &operator-=(const Size &s) { wd-=s.wd; ht-=s.ht; return *this; }
    Size &operator*=(qreal c) { wd =Round(wd*c);ht=Round(ht*c); return *this; }
    Size &operator/=(qreal c)
    {
        S_ASSERT(!FuzzyIsNull(c));
        wd =Round(wd/c); ht=Round(ht/c);
        return *this;
    }

    friend inline  bool operator==(const Size &, const Size &) ;
    friend inline  bool operator!=(const Size &, const Size &) ;
    friend inline  const Size operator+(const Size &, const Size &) ;
    friend inline  const Size operator-(const Size &, const Size &) ;
    friend inline  const Size operator*(const Size &, qreal) ;
    friend inline  const Size operator*(qreal, const Size &) ;
    friend inline const Size operator/(const Size &, qreal);
private:
    int wd;
    int ht;
};

 inline bool operator==(const Size &s1, const Size &s2) 
{ return s1.wd == s2.wd && s1.ht == s2.ht; }

 inline bool operator!=(const Size &s1, const Size &s2) 
{ return s1.wd != s2.wd || s1.ht != s2.ht; }

 inline const Size operator+(const Size & s1, const Size & s2) 
{ return Size(s1.wd+s2.wd, s1.ht+s2.ht); }

 inline const Size operator-(const Size &s1, const Size &s2) 
{ return Size(s1.wd-s2.wd, s1.ht-s2.ht); }

 inline const Size operator*(const Size &s, qreal c) 
{ return Size(Round(s.wd*c),Round(s.ht*c)); }

 inline const Size operator*(qreal c, const Size &s) 
{ return Size(Round(s.wd*c),Round(s.ht*c)); }

inline const Size operator/(const Size &s, qreal c)
{
    S_ASSERT(!FuzzyIsNull(c));
    return Size(Round(s.wd/c),Round(s.ht/c));
}



class  SizeF
{
public:
    SizeF()  : wd(-1.), ht(-1.) {}
    SizeF(const Size &sz)  : wd(sz.width()), ht(sz.height()) {}
    SizeF(qreal w, qreal h)  : wd(w), ht(h) {}
    bool isNull() const { return IsNull(wd) && IsNull(ht); }

    bool isEmpty() const { return wd <= 0. || ht <= 0.; }

    bool isValid() const { return wd >= 0. && ht >= 0.; }

    qreal width() const { return wd; }
    qreal height() const { return ht; }
    void setWidth(qreal w) { wd = w; }
    void setHeight(qreal h) { ht = h; }

    SizeF transposed() const { return SizeF(ht, wd); }
    void scale(qreal w, qreal h, Sp::AspectRatioMode mode) { scale(SizeF(w, h), mode); }
    void scale(const SizeF &s, Sp::AspectRatioMode mode) { *this = scaled(s, mode); }

    SizeF scaled(qreal w, qreal h, Sp::AspectRatioMode mode) const { return scaled(SizeF(w, h), mode); }
    SizeF scaled(const SizeF &s, Sp::AspectRatioMode mode) const
    {
        if (mode == Sp::IgnoreAspectRatio ||IsNull(wd) ||IsNull(ht)) {
            return s;
        } else {
            bool useHeight;
            qreal rw = s.ht * wd / ht;

            if (mode == Sp::KeepAspectRatio) {
                useHeight = (rw <= s.wd);
            } else { // mode == Sp::KeepAspectRatioByExpanding
                useHeight = (rw >= s.wd);
            }

            if (useHeight) {
                return SizeF(rw, s.ht);
            } else {
                return SizeF(s.wd, s.wd * ht / wd);
            }
        }
    }

    SizeF expandedTo(const SizeF & otherSize) const 
    {
        return SizeF(Max(wd,otherSize.wd),Max(ht,otherSize.ht));
    }
    SizeF boundedTo(const SizeF & otherSize) const 
    {
        return SizeF(Min(wd,otherSize.wd),Min(ht,otherSize.ht));
    }

    qreal &rwidth() { return wd; }
    qreal &rheight() { return ht; }

    SizeF &operator+=(const SizeF &s) { wd += s.wd; ht += s.ht; return *this; }
    SizeF &operator-=(const SizeF &s) { wd -= s.wd; ht -= s.ht; return *this; }
    SizeF &operator*=(qreal c) { wd *= c; ht *= c; return *this; }
    SizeF &operator/=(qreal c)
    {
        S_ASSERT(!FuzzyIsNull(c));
        wd = wd/c; ht = ht/c;
        return *this;
    }

     void transpose()
     {
         Swap(wd, ht);
     }

    friend  inline bool operator==(const SizeF &, const SizeF &) ;
    friend  inline bool operator!=(const SizeF &, const SizeF &) ;
    friend  inline const SizeF operator+(const SizeF &, const SizeF &) ;
    friend  inline const SizeF operator-(const SizeF &, const SizeF &) ;
    friend  inline const SizeF operator*(const SizeF &, qreal) ;
    friend  inline const SizeF operator*(qreal, const SizeF &) ;
    friend inline const SizeF operator/(const SizeF &, qreal);

    Size toSize() const 
    {
        return Size(Round(wd),Round(ht));
    }
private:
    qreal wd;
    qreal ht;
};


 inline bool operator==(const SizeF &s1, const SizeF &s2) 
{ return FuzzyCompare(s1.wd, s2.wd) && FuzzyCompare(s1.ht, s2.ht); }

 inline bool operator!=(const SizeF &s1, const SizeF &s2) 
{ return !FuzzyCompare(s1.wd, s2.wd) || !FuzzyCompare(s1.ht, s2.ht); }

 inline const SizeF operator+(const SizeF & s1, const SizeF & s2) 
{ return SizeF(s1.wd+s2.wd, s1.ht+s2.ht); }

 inline const SizeF operator-(const SizeF &s1, const SizeF &s2) 
{ return SizeF(s1.wd-s2.wd, s1.ht-s2.ht); }

 inline const SizeF operator*(const SizeF &s, qreal c) 
{ return SizeF(s.wd*c, s.ht*c); }

 inline const SizeF operator*(qreal c, const SizeF &s) 
{ return SizeF(s.wd*c, s.ht*c); }

inline const SizeF operator/(const SizeF &s, qreal c)
{
    S_ASSERT(!FuzzyIsNull(c));
    return SizeF(s.wd/c, s.ht/c);
}

#endif


