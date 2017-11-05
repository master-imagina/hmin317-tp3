#include "mainwindow.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>


MainWindow::MainWindow()
{
//    // Build UI
//    auto centralWidget = new QWidget(this);
//    centralWidget->setFocusPolicy(Qt::StrongFocus);

//    m_cameraController = new CameraController(centralWidget);
//    centralWidget->installEventFilter(m_cameraController);

//    auto centralLayout = new QVBoxLayout(centralWidget);

//    // Viewport
//    auto viewportsLayout = new QGridLayout;

//    // Create game widget
//    m_renderWidget = new RenderWidget(centralWidget);

//    viewportsLayout->addWidget(m_renderWidget);

//    // Add controls to game widget
//    auto *renderWidgetLayout = new QVBoxLayout(m_renderWidget);
//    auto *renderWidgetTopLayout = new QHBoxLayout;
//    renderWidgetTopLayout->addWidget(m_fpsLabel);
//    renderWidgetTopLayout->addStretch();
//    renderWidgetTopLayout->addWidget(new CameraControls(m_camera.get(), m_renderWidget));
//    renderWidgetLayout->addLayout(renderWidgetTopLayout);
//    renderWidgetLayout->addStretch();

//    auto cameraControllerControls = new CameraControllerControls(m_cameraController,
//                                                                 centralWidget);

////    centralLayout->addLayout(fpsControlsLayout);
//    centralLayout->addLayout(viewportsLayout, 1);
//    centralLayout->addWidget(cameraControllerControls);

//    setCentralWidget(centralWidget);

//    centralWidget->setFocus();
}

MainWindow::~MainWindow()
{}
