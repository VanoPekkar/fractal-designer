#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>

#include <QImage>
#include <QTime>
#include <cmath>
#include <QDebug>
#include <QtMath>
#include <complex>
#include "funcenter.h"

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

typedef float type;
enum Fractals {
    Mandelbrot,
    JuliaSet
};

class RenderThread : public QThread
{
    Q_OBJECT

public:
    RenderThread(QObject *parent = nullptr);
    ~RenderThread();

    void render(double centerX, double centerY, double scaleFactor, QSize resultSize, QImage bitMask, double devicePixelRatio);

signals:
    void renderedImage(const QVector<QVector<type>>& res);

protected:
    static uint rgbFromWaveLength(double wave);

    QMutex mutex;
    QWaitCondition condition;
    double centerX;
    double centerY;
    double scaleFactor;
    double devicePixelRatio;
    QSize resultSize;
    bool restart = false;
    bool abort = false;
    QImage mask;

public:
    FuncEnterLineEdit* fparser;
    Fractals fractal_type;

    enum { ColormapSize = 256 };
    uint colormap[ColormapSize];
};

class Mandelbrot_Julia_Thread : public RenderThread {
public:
    Mandelbrot_Julia_Thread(QWidget* parent = nullptr);
    ~Mandelbrot_Julia_Thread();

protected:
    void run() override;

};


#endif // RENDERTHREAD_H
