#pragma once

#include <QImage>

class RayCaster
{
public:
    RayCaster();
    QImage RenderData();
    void createPhantom();

private:
    // depth x height x width
    float phantom[512][512][512];
};

