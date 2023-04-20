#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

#include <QWidget>

namespace Ui {
class CameraControls;
}

class CameraControls : public QWidget
{
    Q_OBJECT

public:
    explicit CameraControls(QWidget *parent = nullptr);
    ~CameraControls();

private:
    Ui::CameraControls *ui;
};

#endif // CAMERACONTROLS_H
