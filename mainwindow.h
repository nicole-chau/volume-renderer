#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QGraphicsScene>

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

private:
    Ui::MainWindow *ui;

    //This is used to display the QImage produced by RenderScene in the GUI
    QGraphicsScene graphicsScene;

    //This is the image rendered by your program when it loads a scene
    QImage renderedImage;


public slots:
    void slot_loadFile();
};
#endif // MAINWINDOW_H
