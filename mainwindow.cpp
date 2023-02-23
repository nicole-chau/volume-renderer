#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), rayCast(RayCast())
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    connect(ui->loadButton, SIGNAL(clicked(bool)), this, SLOT(slot_loadFile()));

//   rayCast.createCube();
   rayCast.createSphere();
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

void MainWindow::slot_loadFile() {

    QString directoryName = QFileDialog::getExistingDirectory(this, QString("Load DICOM Files"),
                                                    QDir::currentPath() + (QString("../..")),
                                                    QFileDialog::ShowDirsOnly);

    FileLoader fileLoader(directoryName.toStdString());
//    fileLoader.processPixelData();
    fileLoader.getTags();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    float amount = 1.0f;
    switch(e->key())
    {
//    case Qt::Key_Escape : on_actionQuit_Esc_triggered();  break;
    case Qt::Key_W: rayCast.camera.translateForward(amount); break;
    case Qt::Key_S: rayCast.camera.translateForward(-amount); break;
    case Qt::Key_A: rayCast.camera.translateRight(-amount); break;
    case Qt::Key_D: rayCast.camera.translateRight(amount); break;
    case Qt::Key_Q: rayCast.camera.translateUp(0.5); break;
    case Qt::Key_E: rayCast.camera.translateUp(-0.5); break;
    case Qt::Key_Up: rayCast.camera.rotateRight(-amount); break;
    case Qt::Key_Down: rayCast.camera.rotateRight(amount); break;
    case Qt::Key_Left: rayCast.camera.rotateUp(-amount); break;
    case Qt::Key_Right: rayCast.camera.rotateUp(amount); break;
    case Qt::Key_Z: rayCast.camera.rotateForward(-5.f); break;
    case Qt::Key_X: rayCast.camera.rotateForward(5.f); break;
    }

    renderedImage = rayCast.renderData();
    DisplayQImage(renderedImage);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & (Qt::LeftButton | Qt::RightButton))
    {
        m_mousePosPrev = glm::vec2(e->pos().x(), e->pos().y());
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    glm::vec2 pos(e->pos().x(), e->pos().y());
    if(e->buttons() & Qt::LeftButton)
    {
        // Rotation
        glm::vec2 diff = 0.2f * (pos - m_mousePosPrev);
        m_mousePosPrev = pos;
        rayCast.camera.rotateRight(-diff.x);
        rayCast.camera.rotateUp(-diff.y);
    }
    else if(e->buttons() & Qt::RightButton)
    {
        // Panning
        glm::vec2 diff = 0.05f * (pos - m_mousePosPrev);
        m_mousePosPrev = pos;
        rayCast.camera.translateRight(-diff.x);
        rayCast.camera.translateUp(diff.y);
    }
}
