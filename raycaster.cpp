#include "raycaster.h"

RayCaster::RayCaster()
{

}

void RayCaster::createPhantom()
{
    // Create phantom cube for testing
    int depth = sizeof(phantom) / sizeof(int);
    int height = sizeof(phantom[0]) / sizeof(int);
    int width = sizeof(phantom[0][0]) / sizeof(int);

    int min = -100;
    int max = 100;

    for (int d = 0; d < depth; ++d) {
        for (int h = 0; h < height; ++h) {
            for (int w = 0; w < width; ++w) {
                if ((d >= min && h >= min && w >= min)
                    || (d <= max && h <= max && w <= max))
                {
                    // set density to 1
                    phantom[d][h][w] = 1;
                } else
                {
                    phantom[d][h][w] = 0;
                }

            }
        }
    }
}

QImage RayCaster::RenderData()
{
    QImage result(512, 512, QImage::Format_RGB32);
    result.fill(qRgb(0.f, 0.f, 0.f));
    return result;
}
