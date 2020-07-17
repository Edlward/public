#ifndef LINE_Header
#define LINE_Header


#include "Point.h"
#include "../Basic/Math.h"


class  Line
{
public:
    Line() { }
    Line(const Point &pt1_, const Point &pt2_) : pt1(pt1_), pt2(pt2_) { }
    Line(int x1pos, int y1pos, int x2pos, int y2pos) : pt1(Point(x1pos, y1pos)), pt2(Point(x2pos, y2pos)) { }
    bool isNull() const{return pt1 == pt2;}
    int x1() const{return pt1.x();}
    int y1() const{return pt1.y();}
    int x2() const{return pt2.x();}
    int y2() const{return pt2.y();}
    Point p1() const{return pt1;}
    Point p2() const{return pt2;}
    int dx() const{return pt2.x() - pt1.x();}
    int dy() const{return pt2.y() - pt1.y();}
    void translate(const Point &point){pt1 += point;pt2 += point;}
    void translate(int adx, int ady){this->translate(Point(adx, ady));}
    Line translated(const Point &p) const{return Line(pt1 + p, pt2 + p);}
    Line translated(int adx, int ady) const{return translated(Point(adx, ady));}
    Point center() const
    {
        return Point(int((int64_t(pt1.x()) + pt2.x()) / 2), int((int64_t(pt1.y()) + pt2.y()) / 2));
    }

    void setP1(const Point &aP1){pt1 = aP1;}
    void setP2(const Point &aP2){pt2 = aP2;}
    void setPoints(const Point &aP1, const Point &aP2){pt1 = aP1;pt2 = aP2;}
    void setLine(int aX1, int aY1, int aX2, int aY2){pt1 = Point(aX1, aY1);pt2 = Point(aX2, aY2);}

    bool operator==(const Line &d) const{return pt1 == d.pt1 && pt2 == d.pt2;}
    bool operator!=(const Line &d) const { return !(*this == d); }
private:
    Point pt1, pt2;
};


class  LineF 
{
public:
    enum IntersectType { NoIntersection, BoundedIntersection, UnboundedIntersection };

    LineF(){}
    LineF(const PointF &apt1, const PointF &apt2)
        : pt1(apt1), pt2(apt2)
    {
    }
    LineF(qreal x1pos, qreal y1pos, qreal x2pos, qreal y2pos)
        : pt1(x1pos, y1pos), pt2(x2pos, y2pos)
    {
    }
    qreal x1() const{return pt1.x();}
    qreal y1() const{return pt1.y();}
    qreal x2() const{return pt2.x();}
    qreal y2() const{return pt2.y();}
    bool isNull() const
    {
        return FuzzyCompare(pt1.x(), pt2.x()) &&FuzzyCompare(pt1.y(), pt2.y());
    }

    PointF p1() const{return pt1;}
    PointF p2() const{return pt2;}
    qreal dx() const{return pt2.x() - pt1.x();}
    qreal dy() const{return pt2.y() - pt1.y();}

    LineF normalVector() const
    {
        return LineF(p1(), p1() + PointF(dy(), -dx()));
    }

    void translate(const PointF &point)
    {
        pt1 += point;
        pt2 += point;
    }
    void translate(qreal adx, qreal ady){this->translate(PointF(adx, ady));}
    LineF translated(const PointF &p) const{return LineF(pt1 + p, pt2 + p);}
    LineF translated(qreal adx, qreal ady) const{return translated(PointF(adx, ady));}

    PointF center() const
    {
        return PointF(0.5 * pt1.x() + 0.5 * pt2.x(), 0.5 * pt1.y() + 0.5 * pt2.y());
    }

    void setLength(qreal len)
    {
        if (isNull())
            return;
        LineF v = unitVector();
        pt2 = PointF(pt1.x() + v.dx() * len, pt1.y() + v.dy() * len);
    }

    PointF pointAt(qreal t) const
    {
        return PointF(pt1.x() + (pt2.x() - pt1.x()) * t, pt1.y() + (pt2.y() - pt1.y()) * t);
    }

    Line toLine() const
    {
        return Line(pt1.toPoint(), pt2.toPoint());
    }

    void setP1(const PointF &aP1){pt1 = aP1;}
    void setP2(const PointF &aP2){pt2 = aP2;}

    void setPoints(const PointF &aP1, const PointF &aP2)
    {
        pt1 = aP1;
        pt2 = aP2;
    }

    void setLine(qreal aX1, qreal aY1, qreal aX2, qreal aY2)
    {
        pt1 = PointF(aX1, aY1);
        pt2 = PointF(aX2, aY2);
    }

    bool operator==(const LineF &d) const
    {
        return pt1 == d.pt1 && pt2 == d.pt2;
    }

     static LineF fromPolar(qreal length, qreal angle)
     {
         const qreal angleR = angle * M_2PI / 360.0;
         return LineF(0, 0, qCos(angleR) * length, -qSin(angleR) * length);
     }

     qreal length() const
     {
         qreal x = pt2.x() - pt1.x();
         qreal y = pt2.y() - pt1.y();
         return qSqrt(x*x + y*y);
     }

    //void setLength(qreal len);
    //qreal angle() const;
     qreal angle() const
     {
         const qreal dx = pt2.x() - pt1.x();
         const qreal dy = pt2.y() - pt1.y();
         const qreal theta = qAtan2(-dy, dx) * 360.0 / M_2PI;
         const qreal theta_normalized = theta < 0 ? theta + 360 : theta;
         if (FuzzyCompare(theta_normalized, qreal(360)))
             return qreal(0);
         else
             return theta_normalized;
     }
     void setAngle(qreal angle)
     {
         const qreal angleR = angle * M_2PI / 360.0;
         const qreal l = length();

         const qreal dx = qCos(angleR) * l;
         const qreal dy = -qSin(angleR) * l;

         pt2.rx() = pt1.x() + dx;
         pt2.ry() = pt1.y() + dy;
     }

    qreal angleTo(const LineF &l) const
    {
        if (isNull() || l.isNull())
            return 0;

        const qreal a1 = angle();
        const qreal a2 = l.angle();

        const qreal delta = a2 - a1;
        const qreal delta_normalized = delta < 0 ? delta + 360 : delta;

        if (FuzzyCompare(delta, qreal(360)))
            return 0;
        else
            return delta_normalized;
    }

      //inline QLineF normalVector() const;
    LineF unitVector() const
    {
        qreal x = pt2.x() - pt1.x();
        qreal y = pt2.y() - pt1.y();

        qreal len = qSqrt(x*x + y*y);
        LineF f(p1(), PointF(pt1.x() + x/len, pt1.y() + y/len));
        return f;
    }

    //// ### Qt 6: rename intersects() or intersection() and rename IntersectType IntersectionType
    IntersectType intersect(const LineF &l, PointF *intersectionPoint) const
    {
        // ipmlementation is based on Graphics Gems III's "Faster Line Segment Intersection"
        const PointF a = pt2 - pt1;
        const PointF b = l.pt1 - l.pt2;
        const PointF c = pt1 - l.pt1;

        const qreal denominator = a.y() * b.x() - a.x() * b.y();
        if (denominator == 0 || !qt_is_finite(denominator))
        {
            return NoIntersection;
        }

        const qreal reciprocal = 1 / denominator;
        const qreal na = (b.y() * c.x() - b.x() * c.y()) * reciprocal;
        if (intersectionPoint)
            *intersectionPoint = pt1 + a * na;

        if (na < 0 || na > 1)
            return UnboundedIntersection;

        const qreal nb = (a.x() * c.y() - a.y() * c.x()) * reciprocal;
        if (nb < 0 || nb > 1)
            return UnboundedIntersection;

        return BoundedIntersection;
    }

    qreal angle(const LineF &l) const
    {
        if (isNull() || l.isNull())
            return 0;
        qreal cos_line = (dx()*l.dx() + dy()*l.dy()) / (length()*l.length());
        qreal rad = 0;
        // only accept cos_line in the range [-1,1], if it is outside, use 0 (we return 0 rather than PI for those cases)
        if (cos_line >= -1.0 && cos_line <= 1.0) rad = qAcos( cos_line );
        return rad * 360 / M_2PI;
    }
private:
    PointF pt1, pt2;
};

#endif

