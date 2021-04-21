#include "mainscene.h"

#include<QImage>
#include<QPixmap>
#include<QColor>
#include <QPainter>
#include <Qt>
#include <QSize>
#include <QTime>

#include <complex>
#include <cmath>
#include <algorithm>
#include <vector>


#include <iostream>

//Q_DECLARE_METATYPE(long double);
//Q_DECLARE_METATYPE(QVector<long double>);
//Q_DECLARE_METATYPE(QVector<QVector<long double>>);
//Q_DECLARE_OPAQUE_POINTER(QVector<QVector<long double>>&);

unsigned colormapGreyScale(int i) {
    return qRgb(i, i, i);
}

uint colormapBasic(int i) {
    double wave = 380.0 + (i * 500.0 / MainScene::ColormapSize);
    double r = 0;
    double g = 0;
    double b = 0;

    if (wave >= 380.0 && wave <= 440.0) {
        r = -1.0 * (wave - 440.0) / (440.0 - 380.0);
        b = 1.0;
    } else if (wave >= 440.0 && wave <= 490.0) {
        g = (wave - 440.0) / (490.0 - 440.0);
        b = 1.0;
    } else if (wave >= 490.0 && wave <= 510.0) {
        g = 1.0;
        b = -1.0 * (wave - 510.0) / (510.0 - 490.0);
    } else if (wave >= 510.0 && wave <= 580.0) {
        r = (wave - 510.0) / (580.0 - 510.0);
        g = 1.0;
    } else if (wave >= 580.0 && wave <= 645.0) {
        r = 1.0;
        g = -1.0 * (wave - 645.0) / (645.0 - 580.0);
    } else if (wave >= 645.0 && wave <= 780.0) {
        r = 1.0;
    }

    double s = 1.0;
    if (wave > 700.0)
        s = 0.3 + 0.7 * (780.0 - wave) / (780.0 - 700.0);
    else if (wave <  420.0)
        s = 0.3 + 0.7 * (wave - 380.0) / (420.0 - 380.0);

    r = std::pow(r * s, 0.8);
    g = std::pow(g * s, 0.8);
    b = std::pow(b * s, 0.8);
    return qRgb(int(r * 255), int(g * 255), int(b * 255));
}


MainScene::MainScene(QObject* parent,
                     long double x_coord, long double y_coord,
                     long double scale,
                     long double scale_factor,
                     const QString formula)
    : QGraphicsScene(parent)
    , x_coord(x_coord)
    , y_coord(y_coord)
    , scale(scale)
    , scale_factor(scale_factor) {
    pparentWidget = parent;
    width = 4;
    temp = 0;
    current_colormap = 0;
    colormaps.resize(10);
    colormaps[0] = colormapBasic;
    colormaps[1] = colormapGreyScale;
    for (int i = 0; i < ColormapSize; ++i) {
        colormap[i] = colormaps[current_colormap](i + temp);
    }
    x_picsize = 800;
    y_picsize = 800;
    funcEnter.setText(formula);
    funcEnter.parse_two_vars();
    //qRegisterMetaType<long double>("long double");
    //qRegisterMetaType<QVector<long double> >("QVector<long double>");
    qRegisterMetaType<QVector<QVector<type>>>("QVector<QVector<type>>");
    //RenderThread::renderedImage()
    thread = new Mandelbrot_Julia_Thread;
    thread->fractal_type = Fractals::Mandelbrot;
    thread->fparser = &funcEnter;
    connect(thread, &RenderThread::renderedImage,
            this, &MainScene::setValueMatrix);
}

void MainScene::zoomIn() {

}

void MainScene::zoomOut() {

}

unsigned int MainScene::iterCount() {
    return 20 + scale * 5;
}

void MainScene::keyPressEvent(QKeyEvent * event) {
    if (event->key() == Qt::Key_Q) {

    } else if (event->key() == Qt::Key_E) {
        temp += 2;
    }
}

void MainScene::setValueMatrix(const QVector<QVector<type>>& res) {
    int di = -last_offset.y(), dj = -last_offset.x();
    int h = res.size(), w = res[0].size();
    if (values.size() == 0 || values.last().size() != h || values.last()[0].size() != w) {
        values = {res};
        offsets = {{di, dj}};
        drawImage();
        return;
    }
    //qDebug() << values.size();
    if (di == 0 && dj == 0) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                type r = res[i][j];
                if (r != -1) {
                    values.last()[i][j] = r;
                }
            }
        }
        drawImage();
        return;
    }
    values_new = QVector<QVector<type>>(h, QVector<type>(w));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int ni = i + di, nj = j + dj;
            type r = res[i][j];
            int s = values.size() - 1;
            while (s >= 0 && r == -1) {
                if (0 <= ni && ni < h && 0 <= nj && nj < w && values[s][ni][nj] != -1) {
                    r = values[s][ni][nj];
                } else {
                    ni += offsets[s].x();
                    nj += offsets[s].y();
                    --s;
                }

            }
            values_new[i][j] = r;
        }
    }
    values.push_back(values_new);
    offsets.push_back({di, dj});
    //if (res.size() > 100)
    last_offset = {0, 0};
    drawImage();
}

void MainScene::updateColormap() {
    for (int i = 0; i < ColormapSize; ++i) {
        //colormap[i] = qRgb(i * sin(i) - temp, i * cos(temp * i) - 50 , temp * i);
        //colormap[i] = qRgb(sin(i) + temp * 15, cos(i + temp) * 5  , i * temp);
        //colormap[i] = qRgb(i, i, i);
        colormap[i] = colormaps[current_colormap](i + temp);
    }
    createImage();
    pixmap1->setPixmap(QPixmap::fromImage(image));
    for (auto* x : items()) {
        if (x != pixmap1) {
            removeItem(x);
        }
    }
    values = {values.last()};
    offsets = {offsets.last()};
}

void MainScene::createImage() {
    QSize resultSize(values.last()[0].size(), values.last().size());
    image = QImage(resultSize, QImage::Format_ARGB32);


    int y = 0, x;
    long double add, fl;
    unsigned int c1, c2, op = 0xff000000;
    uint r, g, b;
    //qDebug() << "start:" << QTime::currentTime();
    for (auto a : values.last()) {
        x = 0;
        for (type mu : a) {
            if (mu == -1) {
                image.setPixel(x, y, qRgba(0, 0, 0, 0));
                ++x;
                continue;
            }
            fl = floorl(mu);
            add = mu - fl;
            c1 = colormap[uint(fl) % ColormapSize];
            c2 = colormap[uint(fl + 1) % ColormapSize];
            r = (((c1 & 0xff0000) >> 16) * (1 - add) + ((c2 & 0xff0000) >> 16) * (add));
            g = ((c1 & 0xff00) >> 8) * (1 - add) + ((c2 & 0xff00) >> 8) * (add);
            b = (c1 & 0xff) * (1 - add) + (c2 & 0xff) * (add);
            image.setPixel(x, y, op + (r << 16) + (g << 8) + b);
            ++x;
        }
        ++y;
    }
    //qDebug() << "finish:" << QTime::currentTime() << "\n";
}

void MainScene::drawImage() {
    createImage();
    pixmap1 = new GridItem(QPixmap::fromImage(image), nullptr, visible_rect.x(), visible_rect.y());
    addItem(pixmap1);
}

void MainScene::reset() {
    x_coord = 0;
    y_coord = 0;
    scale = 0.005;
}
