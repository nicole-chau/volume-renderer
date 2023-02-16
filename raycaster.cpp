#include "raycaster.h"

RayCaster::RayCaster()
{

}

QImage RayCaster::RenderScene()
{
    QImage result(512, 512, QImage::Format_RGB32);
    result.fill(qRgb(0.f, 0.f, 0.f));
    return result;
}
