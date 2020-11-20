#include "mainscene.h"

#include<QImage>
#include<QPixmap>
#include<QColor>

#include <complex>
#include <cmath>

QImage PlotMandelAgain(int colormap = 5, long double xw0 = -2, long double yw0 = -2, long double cs = 4) {
    QImage img(800, 800, QImage::Format_RGB32);
    if (img.isNull()) {
        exit(1);
    }
    img.fill(qRgb(0,0,0));
    int xpicsize = 800;
    int ypicsize = 800;
    for (int x = 0; x < xpicsize; ++x) {
        for (int y = 0; y < ypicsize; ++y) {
            double x0 = x * cs / xpicsize + xw0;
            double y0 = - y * cs / ypicsize + yw0 + cs;
            std::complex<double> c(x0, y0);
            std::complex<double> z(0, 0);
            for (int i = 0; i < 30; ++i) {
                z = z * z + c;
                if (abs(z) > 2) {
                    //img.setPixel(x, y, qRgb((i * colormap) % 255, (i * colormap * 3) % 255, (i * colormap * 5) % 255));
                    //img.setPixel(x, y, qRgb(60 * colormap * sin(arg(z)), 30 * colormap * sin(arg(z)), 40 * colormap * sin(arg(z))));
                    img.setPixel(x, y, qRgb(6 * colormap * sin(abs(z)) + colormap, 8 * colormap * sin(abs(z)) + 2 * colormap, 10 * colormap * sin(abs(z)) + 3 * colormap));
                    break;
                }
            }

        }
    }
    return img;
}


MainScene::MainScene(QObject* parent) : QGraphicsScene(parent) {
    pparentWidget = parent;
    x_coord = -2;
    y_coord = -2;
    width = 4;
    colormap = 5;
}

void MainScene::mousePressEvent(QGraphicsSceneMouseEvent * event) {
    long double x0 = event->scenePos().x();
    long double x1 = x_coord + (x0 / 800) * width;
    long double y0 = event->scenePos().y();
    long double y1 = y_coord + (1 - y0 / 800) * width;
    width = width / 4;
    x_coord = x1 - width / 2;
    y_coord = y1 - width / 2;
    QImage img = PlotMandelAgain(colormap, x_coord, y_coord, width);
    //parent()->scene->addPixmap(QPixmap::fromImage(img));
    //addEllipse(event->scenePos().x() - 5,
    //            event->scenePos().y() - 5,
    //            10,
    //            10,
    //            QPen(Qt::NoPen),
    //            QBrush(Qt::red));
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

