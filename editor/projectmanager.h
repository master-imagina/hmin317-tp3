#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QList>
#include <QObject>
#include <QString>
#include <QUrl>


class ProjectManager : public QObject
{
    Q_OBJECT

public:
    explicit ProjectManager(QObject *parent = nullptr);
    ~ProjectManager() = default;

    QUrl projectsUrl() const;
    QString projectsPath() const;
    QString currentProjectPath() const;
    QStringList recentProjects() const;

    void create(const QString &path);
    void load(const QString &path);

    void build();

Q_SIGNALS:
    void projectCreated(const QString &path);
    void projectLoaded(const QString &path);

private:
    QString m_currentProjectPath;
};

#endif // PROJECTMANAGER_H
