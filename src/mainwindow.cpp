#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"

#include <iostream>

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GLWidget * glWidget = new GLWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(glWidget);
    centralWidget()->setLayout(layout);

    setWindowTitle(tr("gamengin - A little game engine"));
    resize(720, 480);
}

MainWindow::~MainWindow()
{
    delete ui;
}
