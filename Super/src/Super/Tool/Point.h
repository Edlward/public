#ifndef POINT_Header
#define POINT_Header


#include "../Basic/global.h"

class  Point
{
public:
    Point() : xp(0), yp(0) {}
    Point(int xpos, int ypos) : xp(xpos), yp(ypos) {}
    bool isNull() const {return xp == 0 && yp == 0; }
    int x() const{ return xp; }
    int y() const{ return yp; }
    void setX(int xpos){ xp = xpos; }
    void setY(int ypos){ yp = ypos; }

    int  manhattanLength() const{ return Abs(x())+Abs(y()); }
    int &rx(){ return xp; }
    int &ry(){ return yp; }

    Point &operator+=(const Point &p){ xp+=p.xp; yp+=p.yp; return *this; }
    Point &operator-=(const Point &p){ xp-=p.xp; yp-=p.yp; return *this; }
    Point &operator*=(float factor){ xp = Round(xp*factor); yp = Round(yp*factor); return *this; }
    Point &operator*=(double factor){ xp = Round(xp*factor); yp = Round(yp*factor); return *this; }
    Point &operator*=(int factor){ xp = xp*factor; yp = yp*factor; return *this; }
    Point &operator/=(qreal c){xp =Round(xp/c);yp =Round(yp/c);return *this;}

     static inline int dotProduct(const Point &p1, const Point &p2)
    { 
        return p1.xp * p2.xp + p1.yp * p2.yp; 
     }

    friend  inline bool operator==(const Point &, const Point &);
    friend  inline bool operator!=(const Point &, const Point &);
    friend  inline const Point operator+(const Point &, const Point &);
    friend  inline const Point operator-(const Point &, const Point &);
    friend  inline const Point operator*(const Point &, float);
    friend  inline const Point operator*(float, const Point &);
    friend  inline const Point operator*(const Point &, double);
    friend  inline const Point operator*(double, const Point &);
    friend  inline const Point operator*(const Point &, int);
    friend  inline const Point operator*(int, const Point &);
    friend  inline const Point operator+(const Point &);
    friend  inline const Point operator-(const Point &);
    friend  inline const Point operator/(const Point &, qreal);
private:
    friend class QTransform;
    int xp;
    int yp;
};


inline bool operator==(const Point &p1, const Point &p2){ return p1.xp == p2.xp && p1.yp == p2.yp; }
inline bool operator!=(const Point &p1, const Point &p2){ return p1.xp != p2.xp || p1.yp != p2.yp; }
inline const Point operator+(const Point &p1, const Point &p2){ return Point(p1.xp+p2.xp, p1.yp+p2.yp); }
inline const Point operator-(const Point &p1, const Point &p2){ return Point(p1.xp-p2.xp, p1.yp-p2.yp); }
inline const Point operator*(const Point &p, float factor){ return Point(Round(p.xp*factor), Round(p.yp*factor)); }
inline const Point operator*(const Point &p, double factor){ return Point(Round(p.xp*factor),Round(p.yp*factor)); }
inline const Point operator*(const Point &p, int factor){ return Point(p.xp*factor, p.yp*factor); }
inline const Point operator*(float factor, const Point &p){ return Point(Round(p.xp*factor),Round(p.yp*factor)); }
inline const Point operator*(double factor, const Point &p){ return Point(Round(p.xp*factor),Round(p.yp*factor)); }
inline const Point operator*(int factor, const Point &p){ return Point(p.xp*factor, p.yp*factor); }
inline const Point operator+(const Point &p){ return p; }
inline const Point operator-(const Point &p){ return Point(-p.xp, -p.yp); }
inline const Point operator/(const Point &p, qreal c){return Point(Round(p.xp/c),Round(p.yp/c));}


class  PointF
{
public:
    PointF() : xp(0), yp(0) { }
    PointF(qreal xpos, qreal ypos) : xp(xpos), yp(ypos) { }
    PointF(const Point &p) : xp(p.x()), yp(p.y()) { }
    qreal manhattanLength() const{return Abs(x())+Abs(y());}
    bool isNull() const{return IsNull(xp) && IsNull(yp);}
    qreal x() const{return xp;}
    qreal y() const{return yp;}
    void setX(qreal xpos){xp = xpos;}
    void setY(qreal ypos){yp = ypos;}
    qreal &rx(){return xp;}
    qreal &ry(){return yp;}

    PointF &operator+=(const PointF &p){xp+=p.xp;yp+=p.yp;return *this;}
    PointF &operator-=(const PointF &p){xp-=p.xp; yp-=p.yp; return *this;}
    PointF &operator*=(qreal c){xp*=c; yp*=c; return *this;}
    PointF &operator/=(qreal divisor){xp/=divisor;yp/=divisor;return *this;}
    Point toPoint() const{return Point(Round(xp),Round(yp));}

     static inline qreal dotProduct(const PointF &p1, const PointF &p2)
    { return p1.xp * p2.xp + p1.yp * p2.yp; }

    friend  inline bool operator==(const PointF &, const PointF &);
    friend  inline bool operator!=(const PointF &, const PointF &);
    friend  inline const PointF operator+(const PointF &, const PointF &);
    friend  inline const PointF operator-(const PointF &, const PointF &);
    friend  inline const PointF operator*(qreal, const PointF &);
    friend  inline const PointF operator*(const PointF &, qreal);
    friend  inline const PointF operator+(const PointF &);
    friend  inline const PointF operator-(const PointF &);
    friend  inline const PointF operator/(const PointF &, qreal);
private:
    friend class QMatrix;
    friend class QTransform;
    qreal xp;
    qreal yp;
};

inline bool operator==(const PointF &p1, const PointF &p2)
{return FuzzyIsNull(p1.xp - p2.xp) && FuzzyIsNull(p1.yp - p2.yp);}
inline bool operator!=(const PointF &p1, const PointF &p2)
{return !FuzzyIsNull(p1.xp - p2.xp) || !FuzzyIsNull(p1.yp - p2.yp);}
inline const PointF operator+(const PointF &p1, const PointF &p2){return PointF(p1.xp+p2.xp, p1.yp+p2.yp);}

inline const PointF operator-(const PointF &p1, const PointF &p2){return PointF(p1.xp-p2.xp, p1.yp-p2.yp);
}
inline const PointF operator*(const PointF &p, qreal c){ return PointF(p.xp*c, p.yp*c);}
inline const PointF operator*(qreal c, const PointF &p){return PointF(p.xp*c, p.yp*c);}
inline const PointF operator+(const PointF &p){return p;}
inline const PointF operator-(const PointF &p){ return PointF(-p.xp, -p.yp);}
inline const PointF operator/(const PointF &p, qreal divisor){return PointF(p.xp/divisor, p.yp/divisor);}

#endif

