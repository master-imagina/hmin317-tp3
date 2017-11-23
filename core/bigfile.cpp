#include "bigfile.h"

#include <iostream>

#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>


////////////////////// BigFile //////////////////////

BigFile::BigFile() :
    m_entries(),
    m_data()
{}

void BigFile::allocateEntryCount(int entryCount)
{
    m_entries.reserve(entryCount);
}

void BigFile::addEntryFromFile(const std::string &entryPath,
                               qint64 startIndex,
                               qint64 size)
{
    m_entries.insert( {entryPath, Entry {entryPath, startIndex, size} });
}

int BigFile::entryCount() const
{
    return m_entries.size();
}

std::vector<std::string> BigFile::entries() const
{
    std::vector<std::string> ret;
    ret.reserve(m_entries.size());

    for (const auto &entry : m_entries) {
        ret.emplace_back(entry.first);
    }

    return ret;
}

void BigFile::allocateDataCount(int byteCount)
{
    m_data.reserve(byteCount);
}

void BigFile::appendData(const QByteArray &data)
{
    m_data.append(data);
}

QByteArray BigFile::data(const std::string &entryPath) const
{
    const auto entryFound = m_entries.find(entryPath);

    if (entryFound == m_entries.end()) {
        std::cout << "[WARNING] BigFile::data(): entry "
                  << entryPath << " not found"
                  << std::endl;
        return {};
    }

    const Entry entry = entryFound->second;

    return m_data.mid(entry.startIndex, entry.size);
}


////////////////////// Functions //////////////////////

namespace {

void updateFilesPathsList(const QString &folderPath, QStringList &filesPaths)
{
    QDirIterator folderIt(folderPath, QDirIterator::Subdirectories);

    while (folderIt.hasNext()) {
        const QString filePath = folderIt.next();
        const QFileInfo fileInfo(filePath);

        if (fileInfo.isFile()) {
            if (filesPaths.contains(filePath)) {
                std::cout << "[WARNING] createBigFile: "
                          << "duplicated entry for " << filePath.toStdString()
                          << std::endl;
            }
            else {
                filesPaths << filePath;
            }
        }
    }

}

} // anon namespace


void createBigFile(const std::string &folderPath,
                   const std::string &bigFilePath)
{
    QStringList filesPaths;

    const QString qFolderPath = QString::fromStdString(folderPath);

    // Process common folder then main folder
    updateFilesPathsList("common", filesPaths);
    updateFilesPathsList(qFolderPath, filesPaths);

    // Init big file on disk
    const int entryCount = filesPaths.size();

    QFile bigFileOnDisk(QString::fromStdString(bigFilePath));
    bigFileOnDisk.open(QFile::WriteOnly);

    QDataStream outStream(&bigFileOnDisk);

    // Write header (entry count and entries)
    outStream << entryCount;

    qint64 startIndex = 0;

    for (const QString &filePath : filesPaths) {
        QString entryPath = filePath;
        entryPath.remove(qFolderPath + "/");

        const QFileInfo fileInfo(filePath);
        const qint64 fileSize = fileInfo.size();

        outStream << entryPath
                  << startIndex
                  << fileSize;

        startIndex += fileSize;
    }

    // Write files contents
    for (const QString &filePath : filesPaths) {
        QFile entryFile(filePath);
        entryFile.open(QFile::ReadOnly);

        const qint64 entrySize = entryFile.size();

        outStream << entryFile.readAll();

        startIndex += entrySize;
    }
}

BigFile importBigFile(const std::string &filePath)
{
    BigFile ret;

    QFile bigFileOnDisk(QString::fromStdString(filePath));
    bigFileOnDisk.open(QFile::ReadOnly);

    QDataStream outStream(&bigFileOnDisk);

    int entryCount = 0;
    outStream >> entryCount;

    // Read header
    ret.allocateEntryCount(entryCount);

    qint64 byteCount = 0;

    for (int entryIndex = 0; entryIndex < entryCount; entryIndex++) {
        QString entryPath;
        qint64 startIndex = 0;
        qint64 size = 0;

        outStream >> entryPath
                  >> startIndex
                  >> size;

        ret.addEntryFromFile(entryPath.toStdString(), startIndex, size);

        byteCount += size;
    }

    ret.allocateDataCount(byteCount);

    // Read contents
    for (int i = 0; i < ret.entryCount(); i++) {
        QByteArray ba;
        outStream >> ba;

        ret.appendData(ba);
    }

    return ret;
}
