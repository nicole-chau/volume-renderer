#include "cameracontrols.h"
#include "ui_cameracontrols.h"

CameraControls::CameraControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraControls)
{
    ui->setupUi(this);
}

CameraControls::~CameraControls()
{
    delete ui;
}
