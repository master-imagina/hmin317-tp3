#ifndef FPSWIDGETS_H
#define FPSWIDGETS_H

class GameLoop;

class QWidget;


QWidget *createFpsLabel(GameLoop *gameLoop, QWidget *parent = nullptr);
QWidget *createFpsSlider(GameLoop *gameLoop, QWidget *parent = nullptr);

#endif // FPSWIDGETS_H
