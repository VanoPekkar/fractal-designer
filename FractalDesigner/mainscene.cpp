#include "mainscene.h"

#include<QImage>
#include<QPixmap>
#include<QColor>
#include <QPainter>
#include <Qt>

#include <complex>
#include <cmath>
#include <algorithm>
#include <vector>

#include <windows.h>
#include <iostream>

MainScene::MainScene(QObject* parent) : QGraphicsScene(parent) {
    pparentWidget = parent;
    x_coord = -2;
    y_coord = -2;
    width = 4;
    colormap = 10;
    scale = 1;
    x_picsize = 800;
    y_picsize = 800;
    funcEnter.setText("z^2 + c");
    reset();
//    //pic->fill(qRgb(255,0,0));
//    pixmap2 = new GridItem(PlotMOn(800, 0, iterCount()), nullptr, 800, 0);
//    addItem(pixmap2);
//    pixmap2->is_top = true;
//    pixmap2->is_right = true;
//    pic->fill(qRgb(0,255,0));
//    pixmap3 = new GridItem(QPixmap::fromImage(*pic), nullptr, 0, 800);
//    addItem(pixmap3);
//    pixmap3->is_bottom = true;
//    pixmap3->is_left = true;
//    pic->fill(qRgb(0,0,255));
//    pixmap4 = new GridItem(QPixmap::fromImage(*pic), nullptr, 800, 800);
//    addItem(pixmap4);
//    pixmap4->is_bottom = true;
//    pixmap4->is_right = true;
//    grid_vector.push_back(pixmap1);
//    grid_vector.push_back(pixmap2);
//    grid_vector.push_back(pixmap3);
//    grid_vector.push_back(pixmap4);
//    PlotMandel();
    temp = 5;
//    QTimer *timer = new QTimer;
//    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//    timer->start(10);
//    test = new QRectF(0, 0, 200, 200);
//    addRect(*test, QPen());

    //painter = new QPainter(this);
    //QPainter abcd(this);
}

void MainScene::zoomIn() {

}

void MainScene::zoomOut() {

}

unsigned int MainScene::iterCount() {
    return 20 + scale * 5;
}

void MainScene::addTop(int x, int y) {
    y -= 800;
    long double x1 = x_coord + (x / x_picsize) * width;
    long double y1 = y_coord + (1 - y / y_picsize) * width;
    QPixmap tmp = PlotMOn(x1, y1 - width, iterCount());
    pixmap_vector.append(tmp);

    GridItem* temp_pixmap3 = new GridItem(pixmap_vector.back(), nullptr, x, y);
    temp_pixmap3->is_top = true;
    grid_vector.push_back(temp_pixmap3);
    addItem(grid_vector.back());
}

void MainScene::addBottom(int x, int y) {
    y += 800;
    long double x1 = x_coord + (x / x_picsize) * width;
    long double y1 = y_coord + (1 - y / y_picsize) * width;
    QPixmap tmp = PlotMOn(x1, y1 - width, iterCount());
    pixmap_vector.append(tmp);

    GridItem* temp_pixmap3 = new GridItem(pixmap_vector.back(), nullptr, x, y);
    temp_pixmap3->is_bottom = true;
    grid_vector.push_back(temp_pixmap3);
    addItem(grid_vector.back());
}

void MainScene::addLeft(int x, int y) {
    x -= 800;
    long double x1 = x_coord + (x / x_picsize) * width;
    long double y1 = y_coord + (1 - y / y_picsize) * width;
    QPixmap tmp = PlotMOn(x1, y1 - width, iterCount());
    pixmap_vector.append(tmp);

    GridItem* temp_pixmap3 = new GridItem(pixmap_vector.back(), nullptr, x, y);
    temp_pixmap3->is_left = true;
    grid_vector.push_back(temp_pixmap3);
    addItem(grid_vector.back());
}

void MainScene::addRight(int x, int y) {
    x += 800;
    long double x1 = x_coord + (x / x_picsize) * width;
    long double y1 = y_coord + (1 - y / y_picsize) * width;
    QPixmap tmp = PlotMOn(x1, y1 - width, iterCount());
    pixmap_vector.append(tmp);

    GridItem* temp_pixmap3 = new GridItem(pixmap_vector.back(), nullptr, x, y);
    temp_pixmap3->is_right = true;
    grid_vector.push_back(temp_pixmap3);
    addItem(grid_vector.back());
}

void MainScene::addTopLeft(int x, int y) {
    x -= 800;
    y -= 800;
    long double x1 = x_coord + (x / x_picsize) * width;
    long double y1 = y_coord + (1 - y / y_picsize) * width;
    QPixmap tmp = PlotMOn(x1, y1 - width, iterCount());
    pixmap_vector.append(tmp);

    GridItem* temp_pixmap3 = new GridItem(pixmap_vector.back(), nullptr, x, y);
    temp_pixmap3->is_top = true;
    temp_pixmap3->is_left = true;
    grid_vector.push_back(temp_pixmap3);
    addItem(grid_vector.back());
}

void MainScene::addTopRight(int x, int y) {
    x += 800;
    y -= 800;
    long double x1 = x_coord + (x / x_picsize) * width;
    long double y1 = y_coord + (1 - y / y_picsize) * width;
    QPixmap tmp = PlotMOn(x1, y1 - width, iterCount());
    pixmap_vector.append(tmp);

    GridItem* temp_pixmap3 = new GridItem(pixmap_vector.back(), nullptr, x, y);
    temp_pixmap3->is_top = true;
    temp_pixmap3->is_right = true;
    grid_vector.push_back(temp_pixmap3);
    addItem(grid_vector.back());
}

void MainScene::addBottomLeft(int x, int y) {
    x -= 800;
    y += 800;
    long double x1 = x_coord + (x / x_picsize) * width;
    long double y1 = y_coord + (1 - y / y_picsize) * width;
    QPixmap tmp = PlotMOn(x1, y1 - width, iterCount());
    pixmap_vector.append(tmp);

    GridItem* temp_pixmap3 = new GridItem(pixmap_vector.back(), nullptr, x, y);
    temp_pixmap3->is_bottom = true;
    temp_pixmap3->is_left = true;
    grid_vector.push_back(temp_pixmap3);
    addItem(grid_vector.back());
}

void MainScene::addBottomRight(int x, int y) {
    x += 800;
    y += 800;
    long double x1 = x_coord + (x / x_picsize) * width;
    long double y1 = y_coord + (1 - y / y_picsize) * width;
    QPixmap tmp = PlotMOn(x1, y1 - width, iterCount());
    pixmap_vector.append(tmp);

    GridItem* temp_pixmap3 = new GridItem(pixmap_vector.back(), nullptr, x, y);
    temp_pixmap3->is_bottom = true;
    temp_pixmap3->is_right = true;
    grid_vector.push_back(temp_pixmap3);
    addItem(grid_vector.back());
}

void MainScene::wheelEvent(QGraphicsSceneWheelEvent * event) {
    long double x0 = event->scenePos().x(), y0 = event->scenePos().y(), x1, y1;
    x1 = x_coord + (x0 / x_picsize) * width;
    y1 = y_coord + (1 - y0 / y_picsize) * width;
    if (event->delta() > 0) {
        x_coord = x_coord + 3 * (x1 - x_coord) / 4;
        y_coord = y_coord + 3 * (y1 - y_coord) / 4;
        width /= 4;
        scale += 1;
    } else {
        x_coord = x_coord - 3 * (x1 - x_coord);
        y_coord = y_coord - 3 * (y1 - y_coord);
        width *= 4;
        scale -= 1;
    }
    reset();
    //x_coord = x1 - width / 2 - (x1 - x_coord) / 4;
    //y_coord = y1 - width / 2 - (y1 - y_coord) / 4;
    //PlotMandel(colormap, x_coord, y_coord, width, iterCount());
    //QGraphicsScene::wheelEvent(event);
}



void MainScene::PlotMandel(int colormap, long double xw0, long double yw0, long double cs, unsigned int iter) {
    QImage img(x_picsize, y_picsize, QImage::Format_RGB32);
    if (img.isNull()) {
        exit(1);
    }
    img.fill(qRgb(0,0,0));
    std::vector<std::vector<std::complex<double>>> z_points(x_picsize, std::vector<std::complex<double>>(y_picsize));
    for (int x = 0; x < x_picsize; ++x) {
        for (int y = 0; y < y_picsize; ++y) {
            double x0 = x * cs / x_picsize + xw0;
            double y0 = - y * cs / y_picsize + yw0 + cs;
            std::complex<double> c(x0, y0);
            std::complex<double> z = z_points[x][y];
            z_points[x][y] = z;
            for (unsigned int i = 0; i < iter; ++i) {
                z = z * z + c;
                if (abs(z) > 2) {
                    //img.setPixel(x, y, qRgb((i * colormap) % 255, (i * colormap * 3) % 255, (i * colormap * 5) % 255));
                    //img.setPixel(x, y, qRgb(60 * colormap * sin(arg(z)), 30 * colormap * sin(arg(z)), 40 * colormap * sin(arg(z))));
                    //img.setPixel(x, y, qRgb(6 * colormap * sin(abs(z)) + colormap, 8 * colormap * sin(abs(z)) + 2 * colormap, 10 * colormap * sin(abs(z)) + 3 * colormap));
                    //img.setPixel(x, y, qRgb(20 * colormap + i * colormap, 10 * colormap + i * 2 * colormap, 5 * colormap * i));
                    img.setPixel(x, y, qRgb(50 + log(log(abs(z))) * 10,50 + log(log(abs(z))) * 10, 50 + log(log(abs(z))) * 10));
                    break;
                }
            }
        }
    }
    pixmap1->setPixmap(QPixmap::fromImage(img));
    //Sleep(100);
    //std::cout << "kek\n";
    //update();
}

void MainScene::PlotM(long double xw0, long double yw0, unsigned int iter, QGraphicsPixmapItem* pixmap) {
    QImage img(x_picsize, y_picsize, QImage::Format_RGB32);
    if (img.isNull()) {
        exit(1);
    }
    img.fill(qRgb(0,0,0));
    std::vector<std::vector<std::complex<double>>> z_points(x_picsize, std::vector<std::complex<double>>(y_picsize));
    for (int x = 0; x < x_picsize; ++x) {
        for (int y = 0; y < y_picsize; ++y) {
            double x0 = x * width / x_picsize + xw0;
            double y0 = - y * width / y_picsize + yw0 + width;
            std::complex<double> c(x0, y0);
            std::complex<double> z = z_points[x][y];
            z_points[x][y] = z;
            for (unsigned int i = 0; i < iter; ++i) {
                z = z * z + c;
                if (abs(z) > 2) {
                    //img.setPixel(x, y, qRgb((i * colormap) % 255, (i * colormap * 3) % 255, (i * colormap * 5) % 255));
                    //img.setPixel(x, y, qRgb(60 * colormap * sin(arg(z)), 30 * colormap * sin(arg(z)), 40 * colormap * sin(arg(z))));
                    //img.setPixel(x, y, qRgb(6 * colormap * sin(abs(z)) + colormap, 8 * colormap * sin(abs(z)) + 2 * colormap, 10 * colormap * sin(abs(z)) + 3 * colormap));
                    //img.setPixel(x, y, qRgb(20 * colormap + i * colormap, 10 * colormap + i * 2 * colormap, 5 * colormap * i));
                    img.setPixel(x, y, qRgb(50 + log(log(abs(z))) * 10,50 + log(log(abs(z))) * 10, 50 + log(log(abs(z))) * 10));
                    break;
                }
            }
        }
    }
    pixmap->setPixmap(QPixmap::fromImage(img));
    //painter->drawImage(*test, img);
    //addRect(*test, QPen());
    //auto tmp =  addPixmap(QPixmap::fromImage(img));
    //tmp->setPos(800, 800);
    //Sleep(100);
    //std::cout << "kek\n";
    //update();
}

QPixmap MainScene::PlotMOn(long double xw0, long double yw0, unsigned int iter) {
    this->funcEnter.parse_func();
    QImage img(x_picsize, y_picsize, QImage::Format_RGB32);
    if (img.isNull()) {
        exit(1);
    }
    img.fill(qRgb(0,0,0));
    std::vector<std::vector<std::complex<double>>> z_points(x_picsize, std::vector<std::complex<double>>(y_picsize));
    std::complex<double> params[2] = {0, 0};
    for (int x = 0; x < x_picsize; ++x) {
        for (int y = 0; y < y_picsize; ++y) {
            double x0 = x * width / x_picsize + xw0;
            double y0 = - y * width / y_picsize + yw0 + width;
            std::complex<double> c(x0, y0);
            std::complex<double> z = z_points[x][y];
            z_points[x][y] = z;
            for (unsigned int i = 0; i < iter; ++i) {
                //z = z * z + c;
                params[0] = z;
                params[1] = c;
                z = this->funcEnter.eval(params);
                if (abs(z) > 2) {
                    //img.setPixel(x, y, qRgb((i * colormap) % 255, (i * colormap * 3) % 255, (i * colormap * 5) % 255));
                    //img.setPixel(x, y, qRgb(60 * colormap * sin(arg(z)), 30 * colormap * sin(arg(z)), 40 * colormap * sin(arg(z))));
                    //img.setPixel(x, y, qRgb(6 * colormap * sin(abs(z)) + colormap, 8 * colormap * sin(abs(z)) + 2 * colormap, 10 * colormap * sin(abs(z)) + 3 * colormap));
                    img.setPixel(x, y, qRgb(50 + log(log(abs(z))) * 10,50 + log(log(abs(z))) * 10, 50 + log(log(abs(z))) * 10));
                    break;
                }
            }
        }
    }
    return QPixmap::fromImage(img);
}

void MainScene::keyPressEvent(QKeyEvent * event) {
    if (event->key() == Qt::Key_Q) {
        PlotMandel(colormap, x_coord, y_coord, width, 20 * scale + 10);
    } else if (event->key() == Qt::Key_E) {
//        PlotM(x_coord + width, y_coord, iterCount(), pixmap2);
//        PlotM(x_coord, y_coord - width, iterCount(), pixmap3);
//        PlotM(x_coord + width, y_coord - width, iterCount(), pixmap4);
        for (auto elem : grid_vector) {
            PlotM(x_coord + width * (elem->x_pixel) / 800, y_coord - width * (elem->y_pixel) / 800, iterCount(), elem);
        }
        //PlotM(x_coord, y_coord - width, temp, pixmap3);
        //PlotM(x_coord + width, y_coord - width, temp, pixmap4);
        temp += 2;
    }
}

void MainScene::reset() {
    grid_vector.clear();
    pixmap_vector.clear();
    //pic = new QImage(x_picsize, y_picsize, QImage::Format_RGB32);
    pixmap1 = new GridItem(PlotMOn(x_coord, y_coord, iterCount()), nullptr, 0, 0);
    pixmap1->is_top = true;
    pixmap1->is_left = true;
    pixmap1->is_right = true;
    pixmap1->is_bottom = true;
    addItem(pixmap1);
    addTop(0, 0);
    addLeft(0, 0);
    addRight(0, 0);
    addBottom(0, 0);
    addTopLeft(0, 0);
    addBottomLeft(0, 0);
    addTopRight(0, 0);
    addBottomRight(0, 0);
}
