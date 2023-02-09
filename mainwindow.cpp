#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->loadButton, SIGNAL(clicked(bool)), this, SLOT(slot_loadFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_loadFile() {

    QString directoryName = QFileDialog::getExistingDirectory(this, QString("Load DICOM Files"),
                                                    QDir::currentPath() + (QString("../..")),
                                                    QFileDialog::ShowDirsOnly);

    FileLoader fileLoader(directoryName.toStdString());
}

