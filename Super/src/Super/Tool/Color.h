
#ifndef COLOR_Header
#define COLOR_Header

#include "../Basic/global.h"
#include "RGB.h"
#include  <string>
#include  <vector>






NAME_SPACE_MAIN_BEGIN
class CwColor
{
public:
    CwColor(){}//显式定义一个无参数的构造函数
    CwColor(unsigned char R,unsigned char G,unsigned char B)
        :R(R),G(G),B(B)
    {

    }
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

class GradientLinear
{
public:
    GradientLinear(){}
    GradientLinear(CwColor Cbegin,CwColor CEnd)
    {
        reset(Cbegin,CEnd);
    }
    void reset(CwColor Cbegin,CwColor CEnd)
    {
        m_Col[0]=Cbegin;
        m_Col[255]=CEnd;
        float IncreaseR=(CEnd.R-Cbegin.R)/255.0F;     //256 //255
        float IncreaseG=(CEnd.G-Cbegin.G)/255.0F;
        float IncreaseB=(CEnd.B-Cbegin.B)/255.0F;
        for (int n=1;n<255;n++)
        {
            m_Col[n].R=(unsigned char)(Cbegin.R+n* IncreaseR);
            m_Col[n].G=(unsigned char)(Cbegin.G+n* IncreaseG);
            m_Col[n].B=(unsigned char)(Cbegin.B+n* IncreaseB);
        }
    }

    //新增多点渐变功能
    void setColor(float per,CwColor col)
    {

    }

    CwColor getPercent(unsigned char percent)
    {
        return m_Col[percent];
    }
    CwColor m_Col[256];   
};




class Color;
class QVariant;


class  Color
{
public:
    enum Spec { Invalid, Rgb, Hsv, Cmyk, Hsl };
    enum NameFormat { HexRgb, HexArgb };

    Color() 
    { invalidate(); }

    Color(int r, int g, int b, int a)
    { setRgb(r, g, b, a); }


    Color(const std::string& aname)
    { setNamedColor(aname); }

    Color(Sp::GlobalColor color);
    Color(Rgb_t color)
    {
        cspec = Rgb;
        ct.argb.alpha = 0xffff;
        ct.argb.red   = qRed(color)   * 0x101;
        ct.argb.green = qGreen(color) * 0x101;
        ct.argb.blue  = qBlue(color)  * 0x101;
        ct.argb.pad   = 0;
    }


//#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    Color(const Color &acolor) : cspec(acolor.cspec)
    { ct.argb = acolor.ct.argb; }
//#endif

    //QColor(const std::string& name);
    //QColor(const char *aname) : QColor(std::string(aname)) {}
    Color(const char *aname)
    {
         setNamedColor(aname);
    }
    Color(Spec spec)
    {
        switch (spec) {
        case Invalid:
            invalidate();
            break;
        case Rgb:
            setRgb(0, 0, 0);
            break;
        case Hsv:
            setHsv(0, 0, 0);
            break;
        case Cmyk:
            setCmyk(0, 0, 0, 0);
            break;
        case Hsl:
            setHsl(0, 0, 0, 0);
            break;
        }
    }


    Color &operator=(Sp::GlobalColor color) ;

    //bool isValid() const ;

    // ### Qt 6: merge overloads
    std::string name() const
    {
        return name(HexRgb);
    }

    std::string name(NameFormat format) const
    {
        char tmp[128];
        switch (format) {
        case HexRgb:
            sprintf(tmp,"#%02X%02X%02X",red(),green(),blue());

            //return std::string('#') + QString::number(rgba() | 0x1000000, 16).rightRef(6);
        case HexArgb:
            sprintf(tmp,"#%02X%02X%02X%02X",alpha(),red(),green(),blue());
            // it's called rgba() but it does return AARRGGBB
            //return std::string('#') + QString::number(rgba() | 0x100000000, 16).rightRef(8);
        }
        return std::string(tmp);
    }



    void setNamedColor(const std::string& name);

    static std::vector<std::string> colorNames();

    Spec spec() const { return cspec; }

    int alpha() const ;
    void setAlpha(int alpha);

    qreal alphaF() const ;
    void setAlphaF(qreal alpha);

    int red() const ;
    int green() const ;
    int blue() const ;
    void setRed(int red);
    void setGreen(int green);
    void setBlue(int blue);

    qreal redF() const ;
    qreal greenF() const ;
    qreal blueF() const ;
    void setRedF(qreal red);
    void setGreenF(qreal green);
    void setBlueF(qreal blue);

    void getRgb(int *r, int *g, int *b, int *a = NULL) const;
    void setRgb(int r, int g, int b, int a = 255);

    void getRgbF(qreal *r, qreal *g, qreal *b, qreal *a = NULL) const;
    void setRgbF(qreal r, qreal g, qreal b, qreal a = 1.0);

    //QRgba64 rgba64() const ;
    //void setRgba64(QRgba64 rgba) ;

    Rgb_t rgba() const ;
    void setRgba(Rgb_t rgba) ;

    Rgb_t rgb() const ;
    void setRgb(Rgb_t rgb) ;

    int hue() const ; // 0 <= hue < 360
    int saturation() const ;
    int hsvHue() const ; // 0 <= hue < 360
    int hsvSaturation() const ;
    int value() const ;

    qreal hueF() const ; // 0.0 <= hueF < 360.0
    qreal saturationF() const ;
    qreal hsvHueF() const ; // 0.0 <= hueF < 360.0
    qreal hsvSaturationF() const ;
    qreal valueF() const ;

    void getHsv(int *h, int *s, int *v, int *a = NULL) const;
    void setHsv(int h, int s, int v, int a = 255);

    void getHsvF(qreal *h, qreal *s, qreal *v, qreal *a = NULL) const;
    void setHsvF(qreal h, qreal s, qreal v, qreal a = 1.0);

    int cyan() const ;
    int magenta() const ;
    int yellow() const ;
    int black() const ;

    qreal cyanF() const ;
    qreal magentaF() const ;
    qreal yellowF() const ;
    qreal blackF() const ;

    void getCmyk(int *c, int *m, int *y, int *k, int *a = NULL);
    void setCmyk(int c, int m, int y, int k, int a = 255);

    void getCmykF(qreal *c, qreal *m, qreal *y, qreal *k, qreal *a = NULL);
    void setCmykF(qreal c, qreal m, qreal y, qreal k, qreal a = 1.0);

    int hslHue() const ; // 0 <= hue < 360
    int hslSaturation() const ;
    int lightness() const ;

    qreal hslHueF() const ; // 0.0 <= hueF < 360.0
    qreal hslSaturationF() const ;
    qreal lightnessF() const ;

    void getHsl(int *h, int *s, int *l, int *a = NULL) const;
    void setHsl(int h, int s, int l, int a = 255);

    void getHslF(qreal *h, qreal *s, qreal *l, qreal *a = NULL) const;
    void setHslF(qreal h, qreal s, qreal l, qreal a = 1.0);

    Color toRgb() const ;
    Color toHsv() const ;
    Color toCmyk() const ;
    Color toHsl() const ;

    Color convertTo(Spec colorSpec) const ;

    static Color fromRgb(Rgb_t rgb) ;
    static Color fromRgba(Rgb_t rgba) ;

    static Color fromRgb(int r, int g, int b, int a = 255);
    static Color fromRgbF(qreal r, qreal g, qreal b, qreal a = 1.0);

    //static QColor fromRgba64(ushort r, ushort g, ushort b, ushort a = USHRT_MAX) ;
    //static QColor fromRgba64(QRgba64 rgba) ;

    static Color fromHsv(int h, int s, int v, int a = 255);
    static Color fromHsvF(qreal h, qreal s, qreal v, qreal a = 1.0);

    static Color fromCmyk(int c, int m, int y, int k, int a = 255);
    static Color fromCmykF(qreal c, qreal m, qreal y, qreal k, qreal a = 1.0);

    static Color fromHsl(int h, int s, int l, int a = 255);
    static Color fromHslF(qreal h, qreal s, qreal l, qreal a = 1.0);

     Color light(int f = 150) const ;
     Color dark(int f = 200) const ;

     bool isValid() const 
     { return cspec != Invalid; }

     Color lighter(int f = 150) const 
     { return light(f); }

     Color darker(int f = 200) const 
     { return dark(f); }


    bool operator==(const Color &c) const ;
    bool operator!=(const Color &c) const ;

    operator QVariant() const;

    static bool isValidColor(const std::string &name);
private:

    void invalidate() ;

    Spec cspec;
    union {
        struct {
            ushort alpha;
            ushort red;
            ushort green;
            ushort blue;
            ushort pad;
        } argb;
        struct {
            ushort alpha;
            ushort hue;
            ushort saturation;
            ushort value;
            ushort pad;
        } ahsv;
        struct {
            ushort alpha;
            ushort cyan;
            ushort magenta;
            ushort yellow;
            ushort black;
        } acmyk;
        struct {
            ushort alpha;
            ushort hue;
            ushort saturation;
            ushort lightness;
            ushort pad;
        } ahsl;
        ushort array[5];
    } ct;

    friend class QColormap;
};

#endif
