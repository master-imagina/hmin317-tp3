#ifndef BIGFILE_H
#define BIGFILE_H

#include <string>
#include <unordered_map>
#include <vector>

#include <QByteArray>
#include <QtGlobal>


class BigFile
{
public:
    struct Entry
    {
        std::string path;
        qint64 startIndex = 0;
        qint64 size = 0;
    };

    BigFile();

    void allocateEntryCount(int entryCount);

    void addEntryFromFile(const std::string &entryPath,
                          qint64 startIndex,
                          qint64 size);

    int entryCount() const;
    std::vector<std::string> entries() const;

    void allocateDataCount(int byteCount);

    void appendData(const QByteArray &data);

    QByteArray data(const std::string &entryPath) const;

private:
    std::unordered_map<std::string, Entry> m_entries;
    QByteArray m_data;
};


void createBigFile(const std::string &folderPath, const std::string &bigFilePath);

BigFile importBigFile(const std::string &filePath);

#endif // BIGFILE_H
