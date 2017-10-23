#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {

    class MainWindow;

}

class MainWindow : public QMainWindow {

    Q_OBJECT

private:

    // Instance de l'UI
    Ui::MainWindow *ui;

public:

    // Constructeur de base
    explicit MainWindow(QWidget *parent = 0);

    // Destructeur
    ~MainWindow();

};

#endif // MAINWINDOW_H
