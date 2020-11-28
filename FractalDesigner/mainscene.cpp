#include "mainscene.h"
#include "funcenter.h"

#include<QImage>
#include<QPixmap>
#include<QColor>
#include <Qt>

#include <complex>
#include <cmath>
#include <algorithm>

MainScene::MainScene(QObject* parent) : QGraphicsScene(parent) {
    pparentWidget = parent;
    x_coord = -2;
    y_coord = -2;
    width = 4;
    colormap = 10;
    scale = 1;
    x_picsize = 800;
    y_picsize = 800;
}

void MainScene::mousePressEvent(QGraphicsSceneMouseEvent * event) {
    long double x0, y0, x1, y1;
    x0 = event->scenePos().x();
    x1 = x_coord + (x0 / x_picsize) * width;
    y0 = event->scenePos().y();
    y1 = y_coord + (1 - y0 / y_picsize) * width;
    if (event->button() == Qt::LeftButton) {
        width /= 4;
        scale += 1;
    } else {
        width *= 4;
        scale -= 1;
    }
    x_coord = x1 - width / 2;
    y_coord = y1 - width / 2;
    unsigned int iter = 20 + scale * 5;
    QImage img = PlotMandel(colormap, x_coord, y_coord, width, iter);
    addPixmap(QPixmap::fromImage(img));
}

//void MainScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//    // Отрисовываем линии с использованием предыдущей координаты
//    addLine(previousPoint.x(),
//            previousPoint.y(),
//            event->scenePos().x(),
//            event->scenePos().y(),
//            QPen(Qt::red,10,Qt::SolidLine,Qt::RoundCap));
//    // Обновляем данные о предыдущей координате
//    previousPoint = event->scenePos();
//}


QImage MainScene::PlotMandel(int colormap,
                             long double xw0,
                             long double yw0,
                             long double cs,
                             unsigned int iter) {
    QImage img(x_picsize, y_picsize, QImage::Format_RGB32);
    if (img.isNull()) {
        exit(1);
    }
    img.fill(qRgb(0,0,0));
    std::complex<double> varsForFunc[] = { 0, 0 };
    for (int x = 0; x < x_picsize; ++x) {
        for (int y = 0; y < y_picsize; ++y) {
            double x0 = x * cs / x_picsize + xw0;
            double y0 = - y * cs / y_picsize + yw0 + cs;
            std::complex<double> c(x0, y0);
            std::complex<double> z(0, 0);
            varsForFunc[1] = c;
            for (unsigned int i = 0; i < std::max(iter, 5u); ++i) {
                // z = z * z + c;
                std::complex<double> var = z * z + c;
                varsForFunc[0] = z;
                z = fparser->eval(varsForFunc);
                if (abs(z) > 2) {
                    //img.setPixel(x, y, qRgb((i * colormap) % 255, (i * colormap * 3) % 255, (i * colormap * 5) % 255));
                    //img.setPixel(x, y, qRgb(60 * colormap * sin(arg(z)), 30 * colormap * sin(arg(z)), 40 * colormap * sin(arg(z))));
                    //img.setPixel(x, y, qRgb(6 * colormap * sin(abs(z)) + colormap, 8 * colormap * sin(abs(z)) + 2 * colormap, 10 * colormap * sin(abs(z)) + 3 * colormap));
                    img.setPixel(x, y, qRgb(20 * colormap + i * colormap, 10 * colormap + i * 2 * colormap, 5 * colormap * i));
                    break;
                }
            }

        }
    }
    return img;
}
