#include <QFileDialog>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), rayCast(RayCast())
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
    connect(ui->loadButton, SIGNAL(clicked(bool)), this, SLOT(slot_loadFile()));

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

void MainWindow::slot_loadFile() {

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

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    float amount = 0.5f;
    switch(e->key())
    {
//    case Qt::Key_Escape : on_actionQuit_Esc_triggered();  break;
    case Qt::Key_W: rayCast.camera.translateAlongLook(amount); break;
    case Qt::Key_S: rayCast.camera.translateAlongLook(-amount); break;
    case Qt::Key_A: rayCast.camera.translateAlongRight(-amount); break;
    case Qt::Key_D: rayCast.camera.translateAlongRight(amount); break;
    case Qt::Key_Q: rayCast.camera.translateAlongUp(amount); break;
    case Qt::Key_E: rayCast.camera.translateAlongUp(-amount); break;
    case Qt::Key_Up: rayCast.camera.rotateTheta(-amount); break;
    case Qt::Key_Down: rayCast.camera.rotateTheta(amount); break;
    case Qt::Key_Left: rayCast.camera.rotatePhi(-amount); break;
    case Qt::Key_Right: rayCast.camera.rotatePhi(amount); break;
    case Qt::Key_F: rayCast.camera.reset();
//    case Qt::Key_Z: rayCast.camera.rotateForward(-amount); break;
//    case Qt::Key_X: rayCast.camera.rotateForward(amount); break;
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
        rayCast.camera.rotatePhi(-diff.x);
        rayCast.camera.rotateTheta(-diff.y);
    }
    else if(e->buttons() & Qt::RightButton)
    {
        // Panning
        glm::vec2 diff = 0.05f * (pos - m_mousePosPrev);
        m_mousePosPrev = pos;
        rayCast.camera.translateAlongRight(-diff.x);
        rayCast.camera.translateAlongUp(diff.y);
    }
}
