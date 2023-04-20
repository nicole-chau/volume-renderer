#include <QFileDialog>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cameracontrols.h"
#include "huchart.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), rayCast(RayCast())
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    connect(ui->loadButton, SIGNAL(clicked(bool)), this, SLOT(slot_loadFile()));
    connect(ui->displayRGB, SIGNAL(clicked(bool)), this, SLOT(slot_toggleRGB(bool)));
    connect(ui->rangeSlider, SIGNAL(valuesChanged(int, int)), this, SLOT(slot_setHURangeSlider(int, int)));
    connect(ui->spinBoxMin, SIGNAL(valueChanged(int)), this, SLOT(slot_setHUMinSpinBox(int)));
    connect(ui->spinBoxMax, SIGNAL(valueChanged(int)), this, SLOT(slot_setHUMaxSpinBox(int)));
    connect(ui->confirmButton, SIGNAL(clicked(bool)), this, SLOT(slot_confirmHUValue()));

//   rayCast.createCube();
//   rayCast.createSphere();
   rayCast.createCubeVector();
   renderedImage = rayCast.renderData();
   DisplayQImage(renderedImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::DisplayQImage(QImage &i)
{
    QPixmap pixmap(QPixmap::fromImage(i));
    graphicsScene.addPixmap(pixmap);
    graphicsScene.setSceneRect(pixmap.rect());
    ui->scene_display->setScene(&graphicsScene);
}

void MainWindow::slot_loadFile()
{

    QString directoryName = QFileDialog::getExistingDirectory(this, QString("Load DICOM Files"),
                                                    QDir::currentPath() + (QString("../..")),
                                                    QFileDialog::ShowDirsOnly);

    FileLoader fileLoader(directoryName.toStdString());
    rayCast.loadData(fileLoader.width, fileLoader.height, fileLoader.data);

    renderedImage = rayCast.renderData();
    DisplayQImage(renderedImage);

    // FOR DEBUGGING: Uncomment to display raw pixel data as RGB values
//    QImage img(fileLoader.pixelData, fileLoader.width, fileLoader.height, QImage::Format_Indexed8 );
//    DisplayQImage(img);
}

void MainWindow::slot_toggleRGB(bool useRGB)
{
    rayCast.useRGB = useRGB;
    rayCast.setRGBMinMaxRange(ui->rangeSlider->minimumValue(), ui->rangeSlider->maximumValue());
    renderedImage = rayCast.renderData();
    DisplayQImage(renderedImage);
}

void MainWindow::slot_setHURangeSlider(int min, int max)
{
    ui->spinBoxMin->setValue(min);
    ui->spinBoxMax->setValue(max);
}

void MainWindow::slot_setHUMinSpinBox(int min)
{
    ui->rangeSlider->setMinimumValue(min);
}

void MainWindow::slot_setHUMaxSpinBox(int max)
{
    ui->rangeSlider->setMaximumValue(max);
}

void MainWindow::slot_confirmHUValue()
{
    rayCast.setRGBMinMaxRange(ui->spinBoxMin->value(), ui->spinBoxMax->value());

    renderedImage = rayCast.renderData();
    DisplayQImage(renderedImage);
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControls* c = new CameraControls();
    c->show();
}

void MainWindow::on_actionHounsfield_Unit_Chart_triggered()
{
    HUChart * h = new HUChart();
    h->show();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    float translateAmt = 10.f;
    float rotateAmt = 0.1f;
    switch(e->key())
    {
//    case Qt::Key_Escape : on_actionQuit_Esc_triggered();  break;
    case Qt::Key_W: rayCast.camera.translateAlongLook(translateAmt); break;
    case Qt::Key_S: rayCast.camera.translateAlongLook(-translateAmt); break;
    case Qt::Key_A: rayCast.camera.translateAlongRight(-translateAmt); break;
    case Qt::Key_D: rayCast.camera.translateAlongRight(translateAmt); break;
    case Qt::Key_Q: rayCast.camera.translateAlongUp(translateAmt); break;
    case Qt::Key_E: rayCast.camera.translateAlongUp(-translateAmt); break;
    case Qt::Key_Up: rayCast.camera.rotateTheta(-rotateAmt); break;
    case Qt::Key_Down: rayCast.camera.rotateTheta(rotateAmt); break;
    case Qt::Key_Left: rayCast.camera.rotatePhi(-rotateAmt); break;
    case Qt::Key_Right: rayCast.camera.rotatePhi(rotateAmt); break;
    case Qt::Key_F: rayCast.camera.reset();
//    case Qt::Key_Z: rayCast.camera.rotateForward(-amount); break;
//    case Qt::Key_X: rayCast.camera.rotateForward(amount); break;
    }

    renderedImage = rayCast.renderData();
    DisplayQImage(renderedImage);
}
