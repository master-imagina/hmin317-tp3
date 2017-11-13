#ifndef TEXTURE_H
#define TEXTURE_H

#include <functional>
#include <string>

#include <QMetaType>


struct Texture2D
{
    std::string path;

    friend bool operator==(const Texture2D &lhs, const Texture2D &rhs)
    {
        return lhs.path == rhs.path;
    }
};

Q_DECLARE_METATYPE(Texture2D)



namespace std
{
    template<> struct hash<Texture2D>
    {
        typedef Texture2D argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type &texture) const
        {
            const result_type ret ( std::hash<std::string>{}(texture.path) );
            return ret;
        }
    };
}

#endif // TEXTURE_H
