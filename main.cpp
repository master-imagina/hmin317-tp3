/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>
#include <QTimer>
#include "mainwidget.h"
#ifndef QT_NO_OPENGL
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("Particule");
    app.setApplicationVersion("0.1");

#ifndef QT_NO_OPENGL
    MainWidget* win_hiver     = new MainWidget(nullptr, 30, WINTER);
    MainWidget* win_printemps = new MainWidget(nullptr, 30, SPRING);
    MainWidget* win_ete       = new MainWidget(nullptr, 30, SUMMER);
    MainWidget* win_automne   = new MainWidget(nullptr, 30, AUTOMN);

    win_hiver->move(0, 0);
    win_printemps->move(win_hiver->size().width(), 0);
    win_ete->move(0, win_hiver->size().height());
    win_automne->move(win_hiver->size().width(), win_hiver->size().height());

    win_hiver->show();
    win_printemps->show();
    win_ete->show();
    win_automne->show();

    // Prepare le timer des saisons
    QTimer* saisonTimer = new QTimer;

    QObject::connect(saisonTimer, SIGNAL(timeout()), win_hiver, SLOT(nextSaison()));
    QObject::connect(saisonTimer, SIGNAL(timeout()), win_printemps, SLOT(nextSaison()));
    QObject::connect(saisonTimer, SIGNAL(timeout()), win_ete, SLOT(nextSaison()));
    QObject::connect(saisonTimer, SIGNAL(timeout()), win_automne, SLOT(nextSaison()));

    saisonTimer->start(10*1000);
#else
    QLabel note("OpenGL Support required");
    note.show();
#endif
    return app.exec();
}
