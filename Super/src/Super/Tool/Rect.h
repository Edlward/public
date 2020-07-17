#ifndef RECT_Header
#define RECT_Header

#include "Margins.h"
#include "Size.h"
#include "Point.h"
#include "../Basic/Math.h"

class  Rect
{
public:
    Rect()  : x1(0), y1(0), x2(-1), y2(-1) {}
    Rect(int aleft, int atop, int awidth, int aheight) 
        : x1(aleft), y1(atop), x2(aleft + awidth - 1), y2(atop + aheight - 1) {}

    Rect(const Point &atopLeft, const Point &abottomRight) 
        : x1(atopLeft.x()), y1(atopLeft.y()), x2(abottomRight.x()), y2(abottomRight.y()) {}

    Rect(const Point &atopLeft, const Size &asize) 
        : x1(atopLeft.x()), y1(atopLeft.y()), x2(atopLeft.x()+asize.width() - 1), y2(atopLeft.y()+asize.height() - 1) {}

    bool isNull() const { return x2 == x1 - 1 && y2 == y1 - 1; }

    bool isEmpty() const { return x1 > x2 || y1 > y2; }

    bool isValid() const { return x1 <= x2 && y1 <= y2; }

    int left() const { return x1; }

    int top() const { return y1; }

    int right() const { return x2; }

    int bottom() const { return y2; }

    int x() const { return x1; }

    int y() const { return y1; }

    void setLeft(int pos) { x1 = pos; }

    void setTop(int pos) { y1 = pos; }

    void setRight(int pos) { x2 = pos; }

    void setBottom(int pos) { y2 = pos; }

    void setTopLeft(const Point &p) { x1 = p.x(); y1 = p.y(); }

    void setBottomRight(const Point &p) { x2 = p.x(); y2 = p.y(); }

    void setTopRight(const Point &p) { x2 = p.x(); y1 = p.y(); }

    void setBottomLeft(const Point &p) { x1 = p.x(); y2 = p.y(); }

    void setX(int ax) { x1 = ax; }

    void setY(int ay) { y1 = ay; }

    Point topLeft() const { return Point(x1, y1); }

    Point bottomRight() const { return Point(x2, y2); }

    Point topRight() const { return Point(x2, y1); }

    Point bottomLeft() const { return Point(x1, y2); }
    
    // cast avoids overflow on addition
    Point center() const { return Point(int((int64_t(x1)+x2)/2), int((int64_t(y1)+y2)/2)); } 

    int width() const { return  x2 - x1 + 1; }

    int height() const { return  y2 - y1 + 1; }

    Size size() const { return Size(width(), height()); }

    Rect normalized() const
    {
        Rect r;
        if (x2 < x1 - 1) {                                // swap bad x values
            r.x1 = x2;
            r.x2 = x1;
        } else {
            r.x1 = x1;
            r.x2 = x2;
        }
        if (y2 < y1 - 1) {                                // swap bad y values
            r.y1 = y2;
            r.y2 = y1;
        } else {
            r.y1 = y1;
            r.y2 = y2;
        }
        return r;
    }

    void translate(int dx, int dy) 
    {
        x1 += dx;
        y1 += dy;
        x2 += dx;
        y2 += dy;
    }

    void translate(const Point &p) 
    {
        x1 += p.x();
        y1 += p.y();
        x2 += p.x();
        y2 += p.y();
    }

    Rect translated(int dx, int dy) const 
    { return Rect(Point(x1 + dx, y1 + dy), Point(x2 + dx, y2 + dy)); }

    Rect translated(const Point &p) const 
    { return Rect(Point(x1 + p.x(), y1 + p.y()), Point(x2 + p.x(), y2 + p.y())); }

    Rect transposed() const 
    { return Rect(topLeft(), size().transposed()); }

    void moveTo(int ax, int ay) 
    {
        x2 += ax - x1;
        y2 += ay - y1;
        x1 = ax;
        y1 = ay;
    }

    void moveTo(const Point &p) 
    {
        x2 += p.x() - x1;
        y2 += p.y() - y1;
        x1 = p.x();
        y1 = p.y();
    }

    void moveLeft(int pos) 
    { x2 += (pos - x1); x1 = pos; }

    void moveTop(int pos) 
    { y2 += (pos - y1); y1 = pos; }

    void moveRight(int pos) 
    {
        x1 += (pos - x2);
        x2 = pos;
    }

    void moveBottom(int pos) 
    {
        y1 += (pos - y2);
        y2 = pos;
    }

    void moveTopLeft(const Point &p) 
    {
        moveLeft(p.x());
        moveTop(p.y());
    }

    void moveBottomRight(const Point &p) 
    {
        moveRight(p.x());
        moveBottom(p.y());
    }

    void moveTopRight(const Point &p) 
    {
        moveRight(p.x());
        moveTop(p.y());
    }

    void moveBottomLeft(const Point &p) 
    {
        moveLeft(p.x());
        moveBottom(p.y());
    }

    void moveCenter(const Point &p) 
    {
        int w = x2 - x1;
        int h = y2 - y1;
        x1 = p.x() - w/2;
        y1 = p.y() - h/2;
        x2 = x1 + w;
        y2 = y1 + h;
    }

    void getRect(int *ax, int *ay, int *aw, int *ah) const
    {
        *ax = x1;
        *ay = y1;
        *aw = x2 - x1 + 1;
        *ah = y2 - y1 + 1;
    }

    void setRect(int ax, int ay, int aw, int ah) 
    {
        x1 = ax;
        y1 = ay;
        x2 = (ax + aw - 1);
        y2 = (ay + ah - 1);
    }

    void getCoords(int *xp1, int *yp1, int *xp2, int *yp2) const
    {
        *xp1 = x1;
        *yp1 = y1;
        *xp2 = x2;
        *yp2 = y2;
    }

    void setCoords(int xp1, int yp1, int xp2, int yp2) 
    {
        x1 = xp1;
        y1 = yp1;
        x2 = xp2;
        y2 = yp2;
    }

    Rect adjusted(int xp1, int yp1, int xp2, int yp2) const 
    { return Rect(Point(x1 + xp1, y1 + yp1), Point(x2 + xp2, y2 + yp2)); }

    void adjust(int dx1, int dy1, int dx2, int dy2) 
    {
        x1 += dx1;
        y1 += dy1;
        x2 += dx2;
        y2 += dy2;
    }

    void setWidth(int w) 
    { x2 = (x1 + w - 1); }

    void setHeight(int h) 
    { y2 = (y1 + h - 1); }

    void setSize(const Size &s) 
    {
        x2 = (s.width()  + x1 - 1);
        y2 = (s.height() + y1 - 1);
    }

    bool contains(int ax, int ay, bool aproper) const 
    {
        return contains(Point(ax, ay), aproper);
    }

    bool contains(int ax, int ay) const 
    {
        return contains(Point(ax, ay), false);
    }

    Rect& operator|=(const Rect &r) 
    {
        *this = *this | r;
        return *this;
    }

    Rect& operator&=(const Rect &r) 
    {
        *this = *this & r;
        return *this;
    }

    Rect intersected(const Rect &other) const 
    {
        return *this & other;
    }

    Rect united(const Rect &r) const 
    {
        return *this | r;
    }

    Rect operator|(const Rect &r) const
    {
        if (isNull())
            return r;
        if (r.isNull())
            return *this;

        int l1 = x1;
        int r1 = x1;
        if (x2 - x1 + 1 < 0)
            l1 = x2;
        else
            r1 = x2;

        int l2 = r.x1;
        int r2 = r.x1;
        if (r.x2 - r.x1 + 1 < 0)
            l2 = r.x2;
        else
            r2 = r.x2;

        int t1 = y1;
        int b1 = y1;
        if (y2 - y1 + 1 < 0)
            t1 = y2;
        else
            b1 = y2;

        int t2 = r.y1;
        int b2 = r.y1;
        if (r.y2 - r.y1 + 1 < 0)
            t2 = r.y2;
        else
            b2 = r.y2;

        Rect tmp;
        tmp.x1 =Min(l1, l2);
        tmp.x2 =Max(r1, r2);
        tmp.y1 =Min(t1, t2);
        tmp.y2 =Max(b1, b2);
        return tmp;
    }

    Rect operator&(const Rect &r) const
    {
        if (isNull() || r.isNull())
            return Rect();

        int l1 = x1;
        int r1 = x1;
        if (x2 - x1 + 1 < 0)
            l1 = x2;
        else
            r1 = x2;

        int l2 = r.x1;
        int r2 = r.x1;
        if (r.x2 - r.x1 + 1 < 0)
            l2 = r.x2;
        else
            r2 = r.x2;

        if (l1 > r2 || l2 > r1)
            return Rect();

        int t1 = y1;
        int b1 = y1;
        if (y2 - y1 + 1 < 0)
            t1 = y2;
        else
            b1 = y2;

        int t2 = r.y1;
        int b2 = r.y1;
        if (r.y2 - r.y1 + 1 < 0)
            t2 = r.y2;
        else
            b2 = r.y2;

        if (t1 > b2 || t2 > b1)
            return Rect();

        Rect tmp;
        tmp.x1 =Max(l1, l2);
        tmp.x2 =Min(r1, r2);
        tmp.y1 =Max(t1, t2);
        tmp.y2 =Min(b1, b2);
        return tmp;
    }

    bool contains(const Rect &r, bool proper) const
    {
        if (isNull() || r.isNull())
            return false;

        int l1 = x1;
        int r1 = x1;
        if (x2 - x1 + 1 < 0)
            l1 = x2;
        else
            r1 = x2;

        int l2 = r.x1;
        int r2 = r.x1;
        if (r.x2 - r.x1 + 1 < 0)
            l2 = r.x2;
        else
            r2 = r.x2;

        if (proper) {
            if (l2 <= l1 || r2 >= r1)
                return false;
        } else {
            if (l2 < l1 || r2 > r1)
                return false;
        }

        int t1 = y1;
        int b1 = y1;
        if (y2 - y1 + 1 < 0)
            t1 = y2;
        else
            b1 = y2;

        int t2 = r.y1;
        int b2 = r.y1;
        if (r.y2 - r.y1 + 1 < 0)
            t2 = r.y2;
        else
            b2 = r.y2;

        if (proper) {
            if (t2 <= t1 || b2 >= b1)
                return false;
        } else {
            if (t2 < t1 || b2 > b1)
                return false;
        }

        return true;
    }

    bool contains(const Point &p, bool proper) const
    {
        int l, r;
        if (x2 < x1 - 1) {
            l = x2;
            r = x1;
        } else {
            l = x1;
            r = x2;
        }
        if (proper) {
            if (p.x() <= l || p.x() >= r)
                return false;
        } else {
            if (p.x() < l || p.x() > r)
                return false;
        }
        int t, b;
        if (y2 < y1 - 1) {
            t = y2;
            b = y1;
        } else {
            t = y1;
            b = y2;
        }
        if (proper) {
            if (p.y() <= t || p.y() >= b)
                return false;
        } else {
            if (p.y() < t || p.y() > b)
                return false;
        }
        return true;
    }

    bool intersects(const Rect &r) const
    {
        if (isNull() || r.isNull())
            return false;

        int l1 = x1;
        int r1 = x1;
        if (x2 - x1 + 1 < 0)
            l1 = x2;
        else
            r1 = x2;

        int l2 = r.x1;
        int r2 = r.x1;
        if (r.x2 - r.x1 + 1 < 0)
            l2 = r.x2;
        else
            r2 = r.x2;

        if (l1 > r2 || l2 > r1)
            return false;

        int t1 = y1;
        int b1 = y1;
        if (y2 - y1 + 1 < 0)
            t1 = y2;
        else
            b1 = y2;

        int t2 = r.y1;
        int b2 = r.y1;
        if (r.y2 - r.y1 + 1 < 0)
            t2 = r.y2;
        else
            b2 = r.y2;

        if (t1 > b2 || t2 > b1)
            return false;

        return true;
    }


    Rect marginsAdded(const Margins &margins) const 
    {
        return Rect(Point(x1 - margins.left(), y1 - margins.top()),
            Point(x2 + margins.right(), y2 + margins.bottom()));
    }

    Rect marginsRemoved(const Margins &margins) const 
    {
        return Rect(Point(x1 + margins.left(), y1 + margins.top()),
            Point(x2 - margins.right(), y2 - margins.bottom()));
    }

    Rect &operator+=(const Margins &margins) 
    {
        *this = marginsAdded(margins);
        return *this;
    }

    Rect &operator-=(const Margins &margins) 
    {
        *this = marginsRemoved(margins);
        return *this;
    }

    friend  inline bool operator==(const Rect &, const Rect &) ;
    friend  inline bool operator!=(const Rect &, const Rect &) ;

private:
    int x1;
    int y1;
    int x2;
    int y2;
};

 inline bool operator==(const Rect &, const Rect &) ;
 inline bool operator!=(const Rect &, const Rect &) ;


 inline bool operator==(const Rect &r1, const Rect &r2) 
{
    return r1.x1==r2.x1 && r1.x2==r2.x2 && r1.y1==r2.y1 && r1.y2==r2.y2;
}

 inline bool operator!=(const Rect &r1, const Rect &r2) 
{
    return r1.x1!=r2.x1 || r1.x2!=r2.x2 || r1.y1!=r2.y1 || r1.y2!=r2.y2;
}

 inline Rect operator+(const Rect &rectangle, const Margins &margins) 
{
    return Rect(Point(rectangle.left() - margins.left(), rectangle.top() - margins.top()),
                 Point(rectangle.right() + margins.right(), rectangle.bottom() + margins.bottom()));
}

 inline Rect operator+(const Margins &margins, const Rect &rectangle) 
{
    return Rect(Point(rectangle.left() - margins.left(), rectangle.top() - margins.top()),
                 Point(rectangle.right() + margins.right(), rectangle.bottom() + margins.bottom()));
}

 inline Rect operator-(const Rect &lhs, const Margins &rhs) 
{
    return Rect(Point(lhs.left() + rhs.left(), lhs.top() + rhs.top()),
                 Point(lhs.right() - rhs.right(), lhs.bottom() - rhs.bottom()));
}



class  RectF
{
public:
     RectF()  : xp(0.), yp(0.), w(0.), h(0.) {}
     RectF(qreal aleft, qreal atop, qreal awidth, qreal aheight) 
         : xp(aleft), yp(atop), w(awidth), h(aheight)
     {
     }

     RectF(const PointF &atopLeft, const SizeF &asize) 
         : xp(atopLeft.x()), yp(atopLeft.y()), w(asize.width()), h(asize.height())
     {
     }


     RectF(const PointF &atopLeft, const PointF &abottomRight) 
         : xp(atopLeft.x()), yp(atopLeft.y()), w(abottomRight.x() - atopLeft.x()), h(abottomRight.y() - atopLeft.y())
     {
     }

     RectF(const Rect &r) 
         : xp(r.x()), yp(r.y()), w(r.width()), h(r.height())
     {
     }

     //QT_WARNING_PUSH
     //QT_WARNING_DISABLE_CLANG("-Wfloat-equal")
     //QT_WARNING_DISABLE_GCC("-Wfloat-equal")

     bool isNull() const 
     { return w == 0. && h == 0.; }

     bool isEmpty() const 
     { return w <= 0. || h <= 0.; }



     bool isValid() const 
     { return w > 0. && h > 0.; }

     qreal x() const 
     { return xp; }

     qreal y() const 
     { return yp; }

     void setLeft(qreal pos) 
     { qreal diff = pos - xp; xp += diff; w -= diff; }

     void setRight(qreal pos) 
     { w = pos - xp; }

     void setTop(qreal pos) 
     { qreal diff = pos - yp; yp += diff; h -= diff; }

     void setBottom(qreal pos) 
     { h = pos - yp; }

     void setTopLeft(const PointF &p) 
     { setLeft(p.x()); setTop(p.y()); }

     void setTopRight(const PointF &p) 
     { setRight(p.x()); setTop(p.y()); }

     void setBottomLeft(const PointF &p) 
     { setLeft(p.x()); setBottom(p.y()); }

     void setBottomRight(const PointF &p) 
     { setRight(p.x()); setBottom(p.y()); }

     PointF center() const 
     { return PointF(xp + w/2, yp + h/2); }

     void moveLeft(qreal pos) 
     { xp = pos; }

     void moveTop(qreal pos) 
     { yp = pos; }

     void moveRight(qreal pos) 
     { xp = pos - w; }

     void moveBottom(qreal pos) 
     { yp = pos - h; }

     void moveTopLeft(const PointF &p) 
     { moveLeft(p.x()); moveTop(p.y()); }

     void moveTopRight(const PointF &p) 
     { moveRight(p.x()); moveTop(p.y()); }

     void moveBottomLeft(const PointF &p) 
     { moveLeft(p.x()); moveBottom(p.y()); }

     void moveBottomRight(const PointF &p) 
     { moveRight(p.x()); moveBottom(p.y()); }

     void moveCenter(const PointF &p) 
     { xp = p.x() - w/2; yp = p.y() - h/2; }

     qreal width() const 
     { return w; }

     qreal height() const 
     { return h; }

     SizeF size() const 
     { return SizeF(w, h); }

     void translate(qreal dx, qreal dy) 
     {
         xp += dx;
         yp += dy;
     }

     void translate(const PointF &p) 
     {
         xp += p.x();
         yp += p.y();
     }

     void moveTo(qreal ax, qreal ay) 
     {
         xp = ax;
         yp = ay;
     }

     void moveTo(const PointF &p) 
     {
         xp = p.x();
         yp = p.y();
     }

     RectF translated(qreal dx, qreal dy) const 
     { return RectF(xp + dx, yp + dy, w, h); }

     RectF translated(const PointF &p) const 
     { return RectF(xp + p.x(), yp + p.y(), w, h); }

     RectF transposed() const 
     { return RectF(topLeft(), size().transposed()); }

     void getRect(qreal *ax, qreal *ay, qreal *aaw, qreal *aah) const
     {
         *ax = this->xp;
         *ay = this->yp;
         *aaw = this->w;
         *aah = this->h;
     }

     void setRect(qreal ax, qreal ay, qreal aaw, qreal aah) 
     {
         this->xp = ax;
         this->yp = ay;
         this->w = aaw;
         this->h = aah;
     }

     void getCoords(qreal *xp1, qreal *yp1, qreal *xp2, qreal *yp2) const
     {
         *xp1 = xp;
         *yp1 = yp;
         *xp2 = xp + w;
         *yp2 = yp + h;
     }

     void setCoords(qreal xp1, qreal yp1, qreal xp2, qreal yp2) 
     {
         xp = xp1;
         yp = yp1;
         w = xp2 - xp1;
         h = yp2 - yp1;
     }

     void adjust(qreal xp1, qreal yp1, qreal xp2, qreal yp2) 
     { xp += xp1; yp += yp1; w += xp2 - xp1; h += yp2 - yp1; }

     RectF adjusted(qreal xp1, qreal yp1, qreal xp2, qreal yp2) const 
     { return RectF(xp + xp1, yp + yp1, w + xp2 - xp1, h + yp2 - yp1); }

     void setWidth(qreal aw) 
     { this->w = aw; }

     void setHeight(qreal ah) 
     { this->h = ah; }

     void setSize(const SizeF &s) 
     {
         w = s.width();
         h = s.height();
     }

     bool contains(qreal ax, qreal ay) const 
     {
         return contains(PointF(ax, ay));
     }

     RectF& operator|=(const RectF &r) 
     {
         *this = *this | r;
         return *this;
     }

     RectF& operator&=(const RectF &r) 
     {
         *this = *this & r;
         return *this;
     }

     RectF intersected(const RectF &r) const 
     {
         return *this & r;
     }

     RectF united(const RectF &r) const 
     {
         return *this | r;
     }

     RectF normalized() const
     {
         RectF r = *this;
         if (r.w < 0) {
             r.xp += r.w;
             r.w = -r.w;
         }
         if (r.h < 0) {
             r.yp += r.h;
             r.h = -r.h;
         }
         return r;
     }


     qreal left() const  { return xp; }
     qreal top() const  { return yp; }
     qreal right() const  { return xp + w; }
     qreal bottom() const  { return yp + h; }

     PointF topLeft() const  { return PointF(xp, yp); }
     PointF bottomRight() const  { return PointF(xp+w, yp+h); }
     PointF topRight() const  { return PointF(xp+w, yp); }
     PointF bottomLeft() const  { return PointF(xp, yp+h); }

     RectF operator|(const RectF &r) const
     {
         if (isNull())
             return r;
         if (r.isNull())
             return *this;

         qreal left = xp;
         qreal right = xp;
         if (w < 0)
             left += w;
         else
             right += w;

         if (r.w < 0) {
             left = Min(left, r.xp + r.w);
             right = Max(right, r.xp);
         } else {
             left = Min(left, r.xp);
             right = Max(right, r.xp + r.w);
         }

         qreal top = yp;
         qreal bottom = yp;
         if (h < 0)
             top += h;
         else
             bottom += h;

         if (r.h < 0) {
             top =Min(top, r.yp + r.h);
             bottom =Max(bottom, r.yp);
         } else {
             top = Min(top, r.yp);
             bottom = Max(bottom, r.yp + r.h);
         }

         return RectF(left, top, right - left, bottom - top);
     }

     RectF operator&(const RectF &r) const
     {
         qreal l1 = xp;
         qreal r1 = xp;
         if (w < 0)
             l1 += w;
         else
             r1 += w;
         if (l1 == r1) // null rect
             return RectF();

         qreal l2 = r.xp;
         qreal r2 = r.xp;
         if (r.w < 0)
             l2 += r.w;
         else
             r2 += r.w;
         if (l2 == r2) // null rect
             return RectF();

         if (l1 >= r2 || l2 >= r1)
             return RectF();

         qreal t1 = yp;
         qreal b1 = yp;
         if (h < 0)
             t1 += h;
         else
             b1 += h;
         if (t1 == b1) // null rect
             return RectF();

         qreal t2 = r.yp;
         qreal b2 = r.yp;
         if (r.h < 0)
             t2 += r.h;
         else
             b2 += r.h;
         if (t2 == b2) // null rect
             return RectF();

         if (t1 >= b2 || t2 >= b1)
             return RectF();

         RectF tmp;
         tmp.xp = Max(l1, l2);
         tmp.yp = Max(t1, t2);
         tmp.w = Min(r1, r2) - tmp.xp;
         tmp.h = Min(b1, b2) - tmp.yp;
         return tmp;
     }

    bool contains(const RectF &r) const
    {
        qreal l1 = xp;
        qreal r1 = xp;
        if (w < 0)
            l1 += w;
        else
            r1 += w;
        if (l1 == r1) // null rect
            return false;

        qreal l2 = r.xp;
        qreal r2 = r.xp;
        if (r.w < 0)
            l2 += r.w;
        else
            r2 += r.w;
        if (l2 == r2) // null rect
            return false;

        if (l2 < l1 || r2 > r1)
            return false;

        qreal t1 = yp;
        qreal b1 = yp;
        if (h < 0)
            t1 += h;
        else
            b1 += h;
        if (t1 == b1) // null rect
            return false;

        qreal t2 = r.yp;
        qreal b2 = r.yp;
        if (r.h < 0)
            t2 += r.h;
        else
            b2 += r.h;
        if (t2 == b2) // null rect
            return false;

        if (t2 < t1 || b2 > b1)
            return false;

        return true;
    }

    bool contains(const PointF &p) const
    {
        qreal l = xp;
        qreal r = xp;
        if (w < 0)
            l += w;
        else
            r += w;
        if (l == r) // null rect
            return false;

        if (p.x() < l || p.x() > r)
            return false;

        qreal t = yp;
        qreal b = yp;
        if (h < 0)
            t += h;
        else
            b += h;
        if (t == b) // null rect
            return false;

        if (p.y() < t || p.y() > b)
            return false;

        return true;
    }

    bool intersects(const RectF &r) const
    {
        qreal l1 = xp;
        qreal r1 = xp;
        if (w < 0)
            l1 += w;
        else
            r1 += w;
        if (l1 == r1) // null rect
            return false;

        qreal l2 = r.xp;
        qreal r2 = r.xp;
        if (r.w < 0)
            l2 += r.w;
        else
            r2 += r.w;
        if (l2 == r2) // null rect
            return false;

        if (l1 >= r2 || l2 >= r1)
            return false;

        qreal t1 = yp;
        qreal b1 = yp;
        if (h < 0)
            t1 += h;
        else
            b1 += h;
        if (t1 == b1) // null rect
            return false;

        qreal t2 = r.yp;
        qreal b2 = r.yp;
        if (r.h < 0)
            t2 += r.h;
        else
            b2 += r.h;
        if (t2 == b2) // null rect
            return false;

        if (t1 >= b2 || t2 >= b1)
            return false;

        return true;
    }


    RectF marginsAdded(const MarginsF &margins) const 
    {
        return RectF(PointF(xp - margins.left(), yp - margins.top()),
            SizeF(w + margins.left() + margins.right(), h + margins.top() + margins.bottom()));
    }

    RectF marginsRemoved(const MarginsF &margins) const 
    {
        return RectF(PointF(xp + margins.left(), yp + margins.top()),
            SizeF(w - margins.left() - margins.right(), h - margins.top() - margins.bottom()));
    }

    RectF &operator+=(const MarginsF &margins) 
    {
        *this = marginsAdded(margins);
        return *this;
    }

    RectF &operator-=(const MarginsF &margins) 
    {
        *this = marginsRemoved(margins);
        return *this;
    }


    friend  inline bool operator==(const RectF &, const RectF &) ;
    friend  inline bool operator!=(const RectF &, const RectF &) ;


    Rect toRect() const 
    {
        return Rect(Point(Round(xp),Round(yp)), Point(Round(xp + w) - 1,Round(yp + h) - 1));
    }
    Rect toAlignedRect() const
    {
        int xmin = int(qFloor(xp));
        int xmax = int(qCeil(xp + w));
        int ymin = int(qFloor(yp));
        int ymax = int(qCeil(yp + h));
        return Rect(xmin, ymin, xmax - xmin, ymax - ymin);
    }
private:
    qreal xp;
    qreal yp;
    qreal w;
    qreal h;
};


 inline bool operator==(const RectF &, const RectF &) ;
 inline bool operator!=(const RectF &, const RectF &) ;


 inline bool operator==(const RectF &r1, const RectF &r2) 
{
    return FuzzyCompare(r1.xp, r2.xp) && FuzzyCompare(r1.yp, r2.yp)
           && FuzzyCompare(r1.w, r2.w) && FuzzyCompare(r1.h, r2.h);
}

 inline bool operator!=(const RectF &r1, const RectF &r2) 
{
    return !FuzzyCompare(r1.xp, r2.xp) || !FuzzyCompare(r1.yp, r2.yp)
           || !FuzzyCompare(r1.w, r2.w) || !FuzzyCompare(r1.h, r2.h);
}


 inline RectF operator+(const RectF &lhs, const MarginsF &rhs) 
{
    return RectF(PointF(lhs.left() - rhs.left(), lhs.top() - rhs.top()),
                  SizeF(lhs.width() + rhs.left() + rhs.right(), lhs.height() + rhs.top() + rhs.bottom()));
}

 inline RectF operator+(const MarginsF &lhs, const RectF &rhs) 
{
    return RectF(PointF(rhs.left() - lhs.left(), rhs.top() - lhs.top()),
                  SizeF(rhs.width() + lhs.left() + lhs.right(), rhs.height() + lhs.top() + lhs.bottom()));
}

 inline RectF operator-(const RectF &lhs, const MarginsF &rhs) 
{
    return RectF(PointF(lhs.left() + rhs.left(), lhs.top() + rhs.top()),
                  SizeF(lhs.width() - rhs.left() - rhs.right(), lhs.height() - rhs.top() - rhs.bottom()));
}

#endif

