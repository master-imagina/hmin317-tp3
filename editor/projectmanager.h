#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QList>
#include <QString>
#include <QUrl>


QUrl projectsUrl();
QString projectsPath();
QString createNewProject(const QString &path);
QStringList recentProjects();

#endif // PROJECTMANAGER_H
