#include "projectmanager.h"

#include <QCoreApplication>
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QUrl>

#include "core/bigfile.h"


ProjectManager::ProjectManager(QObject *parent) :
    QObject(parent),
    m_currentProjectPath()
{}

QUrl ProjectManager::projectsUrl() const
{
    const QUrl appUrl = QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/");
    const QUrl ret = appUrl.resolved(QUrl("projects/"));

    return ret;
}

QString ProjectManager::projectsPath() const
{
    return projectsUrl().toString(QUrl::PreferLocalFile);
}

QString ProjectManager::currentProjectPath() const
{
    return m_currentProjectPath;
}

QStringList ProjectManager::recentProjects() const
{
    QStringList ret;

    QDirIterator projectsDirIt(projectsPath(),
                              QDir::Dirs |
                              QDir::NoDotAndDotDot |
                              QDir::NoSymLinks);

    while (projectsDirIt.hasNext()) {
        ret << projectsDirIt.next();
    }

    return ret;
}

void ProjectManager::create(const QString &projectName)
{
    const QUrl projectsDirUrl = projectsUrl();

    // Create projects folder if needed
    const QString projectsPath = projectsDirUrl.toString(QUrl::PreferLocalFile);

    const QFileInfo projectsDirInfo(projectsPath);

    if (!projectsDirInfo.exists()) {
        QDir().mkpath(projectsPath);
    }

    // Create the new project folder
    const QUrl projectUrl = projectsDirUrl.resolved(projectName + "/");
    const QString projectPath = projectUrl.toString(QUrl::PreferLocalFile);

    const QFileInfo projectDirInfo(projectPath);

    if (!projectDirInfo.exists()) {
        QDir dirMaker;
        dirMaker.mkpath(projectPath);

        // Create project subdirectories
        const std::vector<QUrl> subdirUrls {
            projectUrl.resolved(QUrl("images/")),
            projectUrl.resolved(QUrl("meshes/")),
            projectUrl.resolved(QUrl("shaders/")),
            projectUrl.resolved(QUrl("scripts/"))
        };

        for (const QUrl &url : subdirUrls) {
            dirMaker.mkpath(url.toString(QUrl::PreferLocalFile));
        }
    }

    m_currentProjectPath = projectPath;

    Q_EMIT projectCreated(m_currentProjectPath);
}

void ProjectManager::load(const QString &path)
{
    m_currentProjectPath = path;

    Q_EMIT projectLoaded(m_currentProjectPath);
}

void ProjectManager::build()
{
    if (!m_currentProjectPath.isNull()) {
        createBigFile(m_currentProjectPath.toStdString(), "data.pak");
    }
}
