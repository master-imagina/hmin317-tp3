#include "textureutils.h"

#include <iostream>
#include <assert.h>

#include <QFile>
#include <QDebug>


uptr<Texture2D> textureFromFile(const std::string &path)
{
    const QString qstringPath = QString::fromStdString(path);
    if (!QFile::exists(qstringPath)) {
        std::cerr << "textureFromFile(): "
                  << path << " does not exist" << std::endl;

        return nullptr;
    }


    auto ret = std::make_unique<Texture2D>();
    ret->name = path;
    ret->image = QImage(qstringPath).convertToFormat(QImage::Format_RGBA8888);

    switch (ret->image.pixelFormat().channelCount()) {
    case 1:
        ret->params.format = Texture2DParams::Format::Red;
        break;
    case 3:
        ret->params.format = Texture2DParams::Format::Rgb;
        break;
    case 4:
        ret->params.format = Texture2DParams::Format::Rgba;
        break;
    default:
        assert (false);
    }


    ret->params.width = ret->image.width();
    ret->params.height = ret->image.height();

    return ret;
}
