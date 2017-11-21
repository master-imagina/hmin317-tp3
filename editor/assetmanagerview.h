#ifndef ASSETMANAGERVIEW_H
#define ASSETMANAGERVIEW_H

#include <vector>

#include <QMainWindow>

class BigFile;
class FlowLayout;

class QFileSystemModel;
class QLabel;
class QTreeView;


class PreviewWidget : public QWidget
{
public:
    PreviewWidget(const QString &name, QWidget *parent = nullptr);
    PreviewWidget(const QString &name, const QImage &img,
                  QWidget *parent = nullptr);
    ~PreviewWidget() = default;

    void setImage(const QImage &img);

private:
    void init(const QString &name);

private:
    QLabel *m_pixmapLabel;
    QLabel *m_pathLabel;
};


class AssetManagerView : public QMainWindow
{
    Q_OBJECT

public:
    explicit AssetManagerView(QWidget *parent = nullptr);
    ~AssetManagerView() = default;

    void setProjectPath(const QString &dirPath);

    void packBigFile();

private:
    void setViewPath(const QString &dirPath);

private:
    QString m_currentFolderPath;

    QLabel *m_currentFolderLabel;
    QTreeView *m_fileSystemView;
    QFileSystemModel *m_fileSystemModel;

    QWidget *m_folderView;
    FlowLayout *m_folderViewLayout;

    std::vector<QString> m_filePaths;
};

#endif // ASSETMANAGERVIEW_H
