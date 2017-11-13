#ifndef TEXTURE_H
#define TEXTURE_H

#include <QImage>


struct Texture2DParams
{
    enum WrapMode
    {
        ClampToEdge = 0x812F,
        ClampToBorder = 0x812D,
        MirroredRepeat = 0x8370,
        Repeat = 0x2901
    };

    enum FilterMode
    {
        Nearest = 0x2600,
        Linear = 0x2601,
        NearestMimapNearest = 0x2700,
        NearestMipmapLinear = 0x2702,
        LinearMipmapNearest = 0x2701,
        LinearMipmapLinear = 0x2703
    };

    enum Format
    {
        Red = 0x1903,
        Rgb = 0x1907,
        Rgba = 0x1908,
    };

    int width = 0;
    int height = 0;
    Format format = Rgb;

    WrapMode wrapS = WrapMode::Repeat;
    WrapMode wrapT = WrapMode::Repeat;

    FilterMode minFilter = FilterMode::Linear;
    FilterMode magFilter = FilterMode::Linear;

    bool genMipMap = true;
};


struct Texture2D
{
    std::string name;
    QImage image;
    Texture2DParams params;
};

Q_DECLARE_METATYPE (Texture2D *)

#endif // TEXTURE_H
