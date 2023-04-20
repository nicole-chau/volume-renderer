#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QGraphicsScene>
#include <QKeyEvent>

#include "fileloader.h"
#include "raycast.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void DisplayQImage(QImage &i);

    void keyPressEvent(QKeyEvent *e);

private:
    Ui::MainWindow *ui;

    //This is used to display the QImage produced by RenderScene in the GUI
    QGraphicsScene graphicsScene;

    //This is the image rendered by your program when it loads a scene
    QImage renderedImage;

    // Ray caster used to render scene
    RayCast rayCast;


public slots:
    void slot_loadFile();
    void slot_toggleRGB(bool useRGB);
    void slot_setHURangeSlider(int min, int max);
    void slot_setHUMinSpinBox(int min);
    void slot_setHUMaxSpinBox(int max);
    void slot_confirmHUValue();

    void on_actionCamera_Controls_triggered();
    void on_actionHounsfield_Unit_Chart_triggered();
};
#endif // MAINWINDOW_H
