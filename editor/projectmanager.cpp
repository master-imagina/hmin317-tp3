#include "projectmanager.h"

#include <QCoreApplication>
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QUrl>


QUrl projectsUrl()
{
    const QUrl appUrl = QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/");
    const QUrl ret = appUrl.resolved(QUrl("projects/"));

    return ret;
}

QString projectsPath()
{
    return projectsUrl().toString(QUrl::PreferLocalFile);
}

QString createNewProject(const QString &projectName)
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
            projectUrl.resolved(QUrl("scripts/"))
        };

        for (const QUrl &url : subdirUrls) {
            dirMaker.mkpath(url.toString(QUrl::PreferLocalFile));
        }
    }

    return projectPath;
}

QStringList recentProjects()
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
