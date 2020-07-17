#ifndef QRGB_H
#define QRGB_H

#include "../Basic/global.h"

//#include <QtGui/qtguiglobal.h>
//#include <QtCore/qprocessordetection.h>
//QT_BEGIN_NAMESPACE


typedef unsigned int Rgb_t;                        // RGB triplet

// non-namespaced Qt global variable
const  Rgb_t  RGB_MASK    = 0x00ffffff;     // masks RGB values

inline  int qRed(Rgb_t rgb)                // get red part of RGB
{ return ((rgb >> 16) & 0xff); }

inline  int qGreen(Rgb_t rgb)                // get green part of RGB
{ return ((rgb >> 8) & 0xff); }

inline  int qBlue(Rgb_t rgb)                // get blue part of RGB
{ return (rgb & 0xff); }

inline  int qAlpha(Rgb_t rgb)                // get alpha part of RGBA
{ return rgb >> 24; }

inline  Rgb_t qRgb(int r, int g, int b)// set RGB value
{ return (0xffu << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu); }

inline  Rgb_t qRgba(int r, int g, int b, int a)// set RGBA value
{ return ((a & 0xffu) << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu); }

inline  int qGray(int r, int g, int b)// convert R,G,B to gray 0..255
{ return (r*11+g*16+b*5)/32; }

inline  int qGray(Rgb_t rgb)                // convert RGB to gray 0..255
{ return qGray(qRed(rgb), qGreen(rgb), qBlue(rgb)); }

inline  bool qIsGray(Rgb_t rgb)
{ return qRed(rgb) == qGreen(rgb) && qRed(rgb) == qBlue(rgb); }

inline  Rgb_t qPremultiply(Rgb_t x)
{
    const uint a = qAlpha(x);
    uint t = (x & 0xff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff) * a;
    x = (x + ((x >> 8) & 0xff) + 0x80);
    x &= 0xff00;
    return x | t | (a << 24);
}

 extern const uint qt_inv_premul_factor[];

inline Rgb_t qUnpremultiply(Rgb_t p)
{
    const uint alpha = qAlpha(p);
    // Alpha 255 and 0 are the two most common values, which makes them beneficial to short-cut.
    if (alpha == 255)
        return p;
    if (alpha == 0)
        return 0;
    // (p*(0x00ff00ff/alpha)) >> 16 == (p*255)/alpha for all p and alpha <= 256.
    const uint invAlpha = qt_inv_premul_factor[alpha];
    // We add 0x8000 to get even rounding. The rounding also ensures that qPremultiply(qUnpremultiply(p)) == p for all p.
    return qRgba((qRed(p)*invAlpha + 0x8000)>>16, (qGreen(p)*invAlpha + 0x8000)>>16, (qBlue(p)*invAlpha + 0x8000)>>16, alpha);
}

//QT_END_NAMESPACE















#ifndef Q_INT64_C
#define Q_INT64_C(c) (c ## LL)
#endif

#ifndef Q_UNT64_C
#define Q_UINT64_C(c) (c ## ULL)
#endif

class QRgba64 {
    uint64 rgba;

    // Make sure that the representation always has the order: red green blue alpha, independent
    // of byte order. This way, vector operations that assume 4 16-bit values see the correct ones.
    enum Shifts {

        //#if Q_BYTE_ORDER == Q_BIG_ENDIAN  ,

        //大小端改动态决议
#if 1  

        RedShift = 48,
        GreenShift = 32,
        BlueShift = 16,
        AlphaShift = 0
#else // little endian:
        RedShift = 0,
        GreenShift = 16,
        BlueShift = 32,
        AlphaShift = 48
#endif
    };

    explicit   QRgba64(uint64 c) : rgba(c) { }
public:
    //QRgba64() = default;

    static
        QRgba64 fromRgba64(uint64 c)
    {
        return QRgba64(c);
    }
    static
        QRgba64 fromRgba64(uint16 red,uint16 green,uint16 blue,uint16 alpha)
    {
        return fromRgba64(uint64(red)   << RedShift
            |uint64(green) << GreenShift
            |uint64(blue)  << BlueShift
            |uint64(alpha) << AlphaShift);
    }
    static QRgba64 fromRgba(uint8 red,uint8 green,uint8 blue,uint8 alpha)
    {
        QRgba64 rgb64 = fromRgba64(red, green, blue, alpha);
        // Expand the range so that 0x00 maps to 0x0000 and 0xff maps to 0xffff.
        rgb64.rgba |= rgb64.rgba << 8;
        return rgb64;
    }
    static
        QRgba64 fromArgb32(uint rgb)
    {
        return fromRgba(uint8(rgb >> 16),uint8(rgb >> 8),uint8(rgb),uint8(rgb >> 24));
    }

    bool isOpaque() const
    {
        return (rgba & alphaMask()) == alphaMask();
    }
    bool isTransparent() const
    {
        return (rgba & alphaMask()) == 0;
    }

    uint16 red()   const { return uint16(rgba >> RedShift);   }
    uint16 green() const { return uint16(rgba >> GreenShift); }
    uint16 blue()  const { return uint16(rgba >> BlueShift);  }
    uint16 alpha() const { return uint16(rgba >> AlphaShift); }
    void setRed(uint16 _red)     { rgba = (rgba & ~(Q_UINT64_C(0xffff) << RedShift))   | (uint64(_red) << RedShift); }
    void setGreen(uint16 _green) { rgba = (rgba & ~(Q_UINT64_C(0xffff) << GreenShift)) | (uint64(_green) << GreenShift); }
    void setBlue(uint16 _blue)   { rgba = (rgba & ~(Q_UINT64_C(0xffff) << BlueShift))  | (uint64(_blue) << BlueShift); }
    void setAlpha(uint16 _alpha) { rgba = (rgba & ~(Q_UINT64_C(0xffff) << AlphaShift)) | (uint64(_alpha) << AlphaShift); }

    uint8 red8()   const { return div_257(red()); }
    uint8 green8() const { return div_257(green()); }
    uint8 blue8()  const { return div_257(blue()); }
    uint8 alpha8() const { return div_257(alpha()); }
    uint toArgb32() const
    {
        return uint((alpha8() << 24) | (red8() << 16) | (green8() << 8) | blue8());
    }
    ushort toRgb16() const
    {
        return ushort((red() & 0xf800) | ((green() >> 10) << 5) | (blue() >> 11));
    }

    QRgba64 premultiplied() const
    {
        if (isOpaque())
            return *this;
        if (isTransparent())
            return QRgba64::fromRgba64(0);
        const uint32 a = alpha();
        const uint16 r = div_65535(red()   * a);
        const uint16 g = div_65535(green() * a);
        const uint16 b = div_65535(blue()  * a);
        return fromRgba64(r, g, b, uint16(a));
    }

    QRgba64 unpremultiplied() const
    {
#if Q_PROCESSOR_WORDSIZE < 8
        return unpremultiplied_32bit();
#else
        return unpremultiplied_64bit();
#endif
    }

    operator uint64() const
    {
        return rgba;
    }

    QRgba64 operator=(uint64 _rgba)
    {
        rgba = _rgba;
        return *this;
    }

private:
    static   uint64 alphaMask() { return Q_UINT64_C(0xffff) << AlphaShift; }

    static   uint8 div_257_floor(uint x) { return uint8((x - (x >> 8)) >> 8); }
    static   uint8 div_257(uint16 x) { return div_257_floor(x + 128U); }
    static   uint16 div_65535(uint x) { return uint16((x + (x>>16) + 0x8000U) >> 16); }
    QRgba64 unpremultiplied_32bit() const
    {
        if (isOpaque() || isTransparent())
            return *this;
        const uint32 a = alpha();
        const uint16 r =uint16((red()   * 0xffff + a/2) / a);
        const uint16 g =uint16((green() * 0xffff + a/2) / a);
        const uint16 b =uint16((blue()  * 0xffff + a/2) / a);
        return fromRgba64(r,g,b,uint16(a));
    }
    QRgba64 unpremultiplied_64bit() const
    {
        if (isOpaque() || isTransparent())
            return *this;
        const uint64 a = alpha();
        const uint64 fa = (Q_UINT64_C(0xffff00008000) + a/2) / a;
        const uint16 r =uint16((red()   * fa + 0x80000000) >> 32);
        const uint16 g =uint16((green() * fa + 0x80000000) >> 32);
        const uint16 b =uint16((blue()  * fa + 0x80000000) >> 32);
        return fromRgba64(r,g,b,uint16(a));
    }
};

//Q_DECLARE_TYPEINFO(QRgba64, Q_PRIMITIVE_TYPE);

inline QRgba64 qRgba64(uint16 r,uint16 g,uint16 b,uint16 a)
{
    return QRgba64::fromRgba64(r, g, b, a);
}

inline QRgba64 qRgba64(uint64 c)
{
    return QRgba64::fromRgba64(c);
}

inline QRgba64 qPremultiply(QRgba64 c)
{
    return c.premultiplied();
}

inline QRgba64 qUnpremultiply(QRgba64 c)
{
    return c.unpremultiplied();
}

inline  uint qRed(QRgba64 rgb)
{ return rgb.red8(); }

inline  uint qGreen(QRgba64 rgb)
{ return rgb.green8(); }

inline  uint qBlue(QRgba64 rgb)
{ return rgb.blue8(); }

inline  uint qAlpha(QRgba64 rgb)
{ return rgb.alpha8(); }

//QT_END_NAMESPACE




#endif // QRGB_H
