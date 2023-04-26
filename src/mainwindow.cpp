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

    connect(ui->rangeSlider , SIGNAL(minimumValueChanged(int)), this, SLOT(slot_setHURangeSliderMin(int)));
    connect(ui->rangeSlider , SIGNAL(maximumValueChanged(int)), this, SLOT(slot_setHURangeSliderMax(int)));

    connect(ui->spinBoxMin, SIGNAL(valueChanged(int)), this, SLOT(slot_setHUMinSpinBox(int)));
    connect(ui->spinBoxMax, SIGNAL(valueChanged(int)), this, SLOT(slot_setHUMaxSpinBox(int)));

    connect(ui->confirmButton, SIGNAL(clicked(bool)), this, SLOT(slot_confirmHUValue()));

    setColorMapMin(ui->rangeSlider->minimumValue());
    setColorMapMax(ui->rangeSlider->maximumValue());

//   rayCast.createCubeVector();
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

void MainWindow::setColorMapMin(int min)
{
    float percent = (ui->rangeSlider->maximumValue() - min) / (abs(HU_MIN) + HU_MAX);
    int height = percent * COLOR_MAP_HEIGHT;

    ui->colorMap->setGeometry(COLOR_MAP_X, ui->colorMap->y(), COLOR_MAP_WIDTH, height);
}

void MainWindow::setColorMapMax(int max)
{
    int min = ui->rangeSlider->minimumValue();
    float percentMin = (min - HU_MIN) / (abs(HU_MIN) + HU_MAX);
    int currMinHeight = (COLOR_MAP_HEIGHT - (percentMin * COLOR_MAP_HEIGHT));

    float percentMax = (HU_MAX - max) / (abs(HU_MIN) + HU_MAX);
    int y = 200 + ((percentMax) * 291);

    int height = currMinHeight - (y - 200);

    ui->colorMap->setGeometry(COLOR_MAP_X, y, COLOR_MAP_WIDTH, height);
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

void MainWindow::slot_setHURangeSliderMin(int min)
{
    ui->spinBoxMin->setValue(min);
    setColorMapMin(min);
}

void MainWindow::slot_setHURangeSliderMax(int max)
{
    ui->spinBoxMax->setValue(max);
    setColorMapMax(max);
}

void MainWindow::slot_setHUMinSpinBox(int min)
{
    ui->rangeSlider->setMinimumValue(min);
    setColorMapMin(min);
}

void MainWindow::slot_setHUMaxSpinBox(int max)
{
    ui->rangeSlider->setMaximumValue(max);
    setColorMapMax(max);
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
