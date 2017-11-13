#ifndef URLEDIT_H
#define URLEDIT_H

#include <QUrl>
#include <QWidget>

class UrlEditPrivate;


class UrlEdit : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QUrl folder READ folder WRITE setFolder NOTIFY folderChanged)
    Q_PROPERTY(QStringList nameFilters READ nameFilters WRITE setNameFilters NOTIFY nameFiltersChanged)
    Q_PROPERTY(bool selectFolder READ selectFolder WRITE setSelectFolder NOTIFY selectFolderChanged)

public:
    explicit UrlEdit(QWidget *parent = nullptr);
    ~UrlEdit();

    QUrl url() const;

    QString title() const;
    QUrl folder() const;
    QStringList nameFilters() const;
    bool selectFolder() const;

Q_SIGNALS:
    void urlChanged(const QUrl &);

    void titleChanged(const QString &);
    void folderChanged(const QUrl &);
    void nameFiltersChanged(const QStringList &);
    void selectFolderChanged(bool);

public Q_SLOTS:
    void setUrl(const QUrl &url);

    void setTitle(const QString &title);
    void setFolder(const QUrl &folder);
    void setNameFilters(const QStringList &nameFilters);
    void setSelectFolder(bool selectFolder);

private Q_SLOTS:
    void openFileDialog();

private:
    const QScopedPointer<UrlEditPrivate> d;
};

#endif // URLEDIT_H
