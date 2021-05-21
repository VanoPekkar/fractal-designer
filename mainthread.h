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

typedef double type;
enum {
    TRANSP = -123456789
};
enum Fractals {
    Mandelbrot,
    JuliaSet,
    Newton,
    Secant,
    IFS,
    LSys
};

class MainThread : public QThread
{
    Q_OBJECT

public:
    MainThread(QObject *parent = nullptr);
    ~MainThread();

    void render(double centerX, double centerY, double scaleFactor, QSize resultSize, QImage bitMask, double devicePixelRatio);

signals:
    void renderedImage(const QVector<QVector<type>>& res);
    void started(int max);
    void nextLine(int val);
    void format(QString f);

public:
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


    FuncEnterLineEdit* fparser;
    FuncEnterLineEdit* fparser_derivative;
    Fractals fractal_type;

    int calc_flag = 0;
    type tolerance = 0.000001;
    size_t MaxIterations = 100;

    type newton_c_real, newton_c_imag, newton_a_real, newton_a_imag;
    bool is_mandel_nova = false;

    enum { ColormapSize = 256 };
    uint colormap[ColormapSize];
};

class Mandelbrot_Julia_Thread : public MainThread {
public:
    Mandelbrot_Julia_Thread(QWidget* parent = nullptr);
    ~Mandelbrot_Julia_Thread();

protected:
    void run() override;

};

class Newton_Thread : public MainThread {
public:
    Newton_Thread(QWidget* parent = nullptr);
    ~Newton_Thread();

protected:
    void run() override;

};

class IFS_Thread : public MainThread {
public:
    IFS_Thread(QWidget* parent = nullptr);
    ~IFS_Thread();

protected:
    void run() override;

};

class LSys_Thread : public MainThread {
public:
    LSys_Thread(QWidget* parent = nullptr);
    ~LSys_Thread();

protected:
    void run() override;

};


#endif // RENDERTHREAD_H
