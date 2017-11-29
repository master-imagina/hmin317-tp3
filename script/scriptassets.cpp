#include "scriptassets.h"

#include <iostream>

#include <QFile>

#include "core/assetmanager.h"


Script scriptFromFile(const std::string &path)
{
    Script ret;
    ret.path = path;

    AssetManager *assetManager = AssetManager::self();

    if (assetManager->bigFile().entryCount() == 0) {
        const QString assetPath = assetManager->pathFor(QString::fromStdString(path));

        QFile file(assetPath);

        if (!file.open(QFile::ReadOnly)) {
            std::cerr << "[ERROR] scriptFromFile() : "
                      << "can't open " << path
                      << std::endl;
        }
        else {
            ret.sourceCode = file.readAll();
        }
    }
    else {
        ret.sourceCode = assetManager->bigFile().data(path);
    }

    return ret;
}
