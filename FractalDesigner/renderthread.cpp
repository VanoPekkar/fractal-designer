#include "renderthread.h"


RenderThread::RenderThread(QObject *parent): QThread(parent) {

}

RenderThread::~RenderThread() {
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void RenderThread::render(double centerX, double centerY, double scaleFactor,
                          QSize resultSize, QImage bitMask , double devicePixelRatio) {
    QMutexLocker locker(&mutex);

    this->centerX = centerX;
    this->centerY = centerY;
    this->scaleFactor = scaleFactor;
    this->devicePixelRatio = devicePixelRatio;
    this->resultSize = resultSize;
    this->mask = bitMask;

    if (!isRunning()) {
        start(LowPriority);
    } else {
        restart = true;
        condition.wakeOne();
    }
}


Mandelbrot_Julia_Thread::Mandelbrot_Julia_Thread(QWidget* parent) : RenderThread(parent) {

}

Mandelbrot_Julia_Thread::~Mandelbrot_Julia_Thread() {

}

void Mandelbrot_Julia_Thread::run() {
    forever {
        mutex.lock();
        const double devicePixelRatio = this->devicePixelRatio;
        const QSize resultSize = this->resultSize * devicePixelRatio;
        const double requestedScaleFactor = this->scaleFactor;
        const double scaleFactor = requestedScaleFactor / devicePixelRatio;
        const double centerX = this->centerX;
        const double centerY = this->centerY;
        mutex.unlock();
        int halfWidth = resultSize.width() / 2;
        int halfHeight = resultSize.height() / 2;
        QVector<QVector<type>> res(resultSize.height(), QVector<type>(resultSize.width()));
        QVector<QVector<type>> iters_a(resultSize.height(), QVector<type>(resultSize.width()));
        QVector<QVector<type>> iters_b(resultSize.height(), QVector<type>(resultSize.width()));
        QVector<QVector<size_t>> iters_num(resultSize.height(), QVector<size_t>(resultSize.width(), 0));
        const int NumPasses = 3;
        int pass = 0;

        long double log_zn;
        long double nu;
        std::complex<double> params_for_parser[2];

        while (pass < NumPasses) {
            size_t MaxIterations = (1 << (2 * pass + 6)) + 32;
            const int Limit = 4;
            bool allBlack = true;

            for (int y = 0; y < resultSize.height(); ++y) {
                if (restart)
                    break;
                if (abort)
                    return;
                type ay = centerY + ((y - halfHeight) * scaleFactor);

                for (int x = 0; x < resultSize.width(); ++x) {
                    type ax = centerX + ((x - halfWidth) * scaleFactor);
                    params_for_parser[1].real(ax);  // TODO: always or not? (for diff fractals)
                    params_for_parser[1].imag(ay);
                    type a1;
                    type b1;
                    size_t numIterations;
                    if (pass) {
                        a1 = iters_a[y][x];
                        b1 = iters_b[y][x];
                        numIterations = iters_num[y][x];
                    } else {
                        a1 = ax;
                        b1 = ay;
                        numIterations = 0;
                    }

                    mutex.lock();
                    int opaque = mask.pixelColor(x, y).value();
                    mutex.unlock();
                    if (!opaque) {
                        res[y][x] = -1;
                        continue;
                    }
                    while (numIterations < MaxIterations) {
                        if ((a1 * a1) + (b1 * b1) > Limit) {
                            log_zn = log((a1 * a1) + (b1 * b1)) / 2;
                            nu = log(log_zn / log(2)) / log(2);
                            iters_a[y][x] = a1;
                            iters_b[y][x] = b1;
                            break;
                        }
                        ++numIterations;
                        type a2, b2;
                        params_for_parser[0].real(a1);
                        params_for_parser[0].imag(b1);
                        params_for_parser[0] = fparser->eval(params_for_parser);
                        a2 = params_for_parser[0].real();
                        b2 = params_for_parser[0].imag();
//                        type a2 = (a1 * a1) - (b1 * b1) + ax;
//                        type b2 = (2 * a1 * b1) + ay;
                        if ((a2 * a2) + (b2 * b2) > Limit) {
                            log_zn = log((a2 * a2) + (b2 * b2)) / 2;
                            nu = log(log_zn / log(2)) / log(2);
                            iters_a[y][x] = a2;
                            iters_b[y][x] = b2;

                            break;
                        }
                        ++numIterations;
                        params_for_parser[0].real(a2);
                        params_for_parser[0].imag(b2);
                        params_for_parser[0] = fparser->eval(params_for_parser);
                        a1 = params_for_parser[0].real();
                        b1 = params_for_parser[0].imag();
//                        a1 = (a2 * a2) - (b2 * b2) + ax;
//                        b1 = (2 * a2 * b2) + ay;

                    }

                    if (numIterations < MaxIterations) {
                        iters_num[y][x] = numIterations;
                        res[y][x] = numIterations + 30 - nu;
                        allBlack = false;
                    } else {
                        res[y][x] = 0;
                    }
                }
            }

            if (allBlack && pass == 0) {
                pass = 2;
            } else {
                if (!restart) {
                    //qDebug() << pass;
                    emit renderedImage(res);
                }
                ++pass;
            }
        }
        mutex.lock();
        if (!restart)
            condition.wait(&mutex);
        restart = false;
        mutex.unlock();
    }
}



Newton_Thread::Newton_Thread(QWidget* parent) : RenderThread(parent) {

}

Newton_Thread::~Newton_Thread() {

}

void Newton_Thread::run() {
    forever {
        mutex.lock();
        const double devicePixelRatio = this->devicePixelRatio;
        const QSize resultSize = this->resultSize * devicePixelRatio;
        const double requestedScaleFactor = this->scaleFactor;
        const double scaleFactor = requestedScaleFactor / devicePixelRatio;
        const double centerX = this->centerX;
        const double centerY = this->centerY;
        mutex.unlock();
        int halfWidth = resultSize.width() / 2;
        int halfHeight = resultSize.height() / 2;
        QVector<QVector<type>> res(resultSize.height(), QVector<type>(resultSize.width()));
        QVector<QVector<type>> iters_a(resultSize.height(), QVector<type>(resultSize.width()));
        QVector<QVector<type>> iters_b(resultSize.height(), QVector<type>(resultSize.width()));
        QVector<QVector<size_t>> iters_num(resultSize.height(), QVector<size_t>(resultSize.width(), 0));
        const int NumPasses = 3;
        int pass = 0;

        std::complex<double> params_for_parser[2];
        type prev_real, prev_imag;
        double tolerance = 0.000001;

        while (pass < NumPasses) {
            size_t MaxIterations = (1 << (2 * pass + 6)) + 32;
            const int Limit = 4;
            bool allBlack = true;

            for (int y = 0; y < resultSize.height(); ++y) {
                if (restart)
                    break;
                if (abort)
                    return;
                type ay = centerY + ((y - halfHeight) * scaleFactor);

                for (int x = 0; x < resultSize.width(); ++x) {
                    type ax = centerX + ((x - halfWidth) * scaleFactor);
                    prev_real = 0;
                    prev_imag = 0;
                    type a1;
                    type b1;
                    size_t numIterations;
                    if (pass) {
                        a1 = iters_a[y][x];
                        b1 = iters_b[y][x];
                        numIterations = iters_num[y][x];
                    } else {
                        a1 = ax;
                        b1 = ay;
                        numIterations = 0;
                    }

                    mutex.lock();
                    int opaque = mask.pixelColor(x, y).value();
                    mutex.unlock();
                    if (!opaque) {
                        res[y][x] = -1;
                        continue;
                    }
                    while (numIterations < MaxIterations) {
                        if (std::sqrt(
                                    (a1 - prev_real) * (a1 - prev_real) +
                                    (b1 - prev_imag) * (b1 - prev_imag)
                                    ) < tolerance) {
                            iters_a[y][x] = a1;
                            iters_b[y][x] = b1;
                            break;
                        }
                        ++numIterations;
                        prev_imag = b1;
                        prev_real = a1;
                        type a2, b2;
                        params_for_parser[0].real(a1);
                        params_for_parser[0].imag(b1);
                        params_for_parser[0] = fparser->eval(params_for_parser);  // f(z)
                        a2 = params_for_parser[0].real();
                        b2 = params_for_parser[0].imag();
                        params_for_parser[0].real(a1);
                        params_for_parser[0].imag(b1);
                        params_for_parser[0] = fparser_derivative->eval(params_for_parser);  // f'(z)
                        // divide f(z) by f'(z) and subtract from z
                        a1 = a1 - (a2 * params_for_parser[0].real() + b2 * params_for_parser[0].imag()) /
                                    (params_for_parser[0].real() * params_for_parser[0].real() +
                                    params_for_parser[0].imag() * params_for_parser[0].imag());
                        b2 = b1 - (params_for_parser[0].real() * b2 - a2 * params_for_parser[0].imag()) /
                                    (params_for_parser[0].real() * params_for_parser[0].real() +
                                    params_for_parser[0].imag() * params_for_parser[0].imag());
                        a2 = a1;
//                        type a2 = (a1 * a1) - (b1 * b1) + ax;
//                        type b2 = (2 * a1 * b1) + ay;
                        if (std::sqrt(
                                    (a2 - prev_real) * (a2 - prev_real) +
                                    (b2 - prev_imag) * (b2 - prev_imag)
                                    ) < tolerance) {
                            iters_a[y][x] = a2;
                            iters_b[y][x] = b2;
                            break;
                        }
                        ++numIterations;
                        params_for_parser[0].real(a2);
                        params_for_parser[0].imag(b2);
                        params_for_parser[0] = fparser->eval(params_for_parser);  // f(z)
                        a1 = params_for_parser[0].real();
                        b1 = params_for_parser[0].imag();
                        params_for_parser[0].real(a2);
                        params_for_parser[0].imag(b2);
                        params_for_parser[0] = fparser_derivative->eval(params_for_parser);  // f'(z)
                        // divide f(z) by f'(z) and subtract from z
                        a2 = a2 - (a1 * params_for_parser[0].real() + b1 * params_for_parser[0].imag()) /
                                    (params_for_parser[0].real() * params_for_parser[0].real() +
                                    params_for_parser[0].imag() * params_for_parser[0].imag());
                        b1 = b2 - (params_for_parser[0].real() * b1 - a1 * params_for_parser[0].imag()) /
                                    (params_for_parser[0].real() * params_for_parser[0].real() +
                                    params_for_parser[0].imag() * params_for_parser[0].imag());
                        a1 = a2;
//                        a1 = (a2 * a2) - (b2 * b2) + ax;
//                        b1 = (2 * a2 * b2) + ay;

                    }

                    if (numIterations < MaxIterations) {
                        iters_num[y][x] = numIterations;
                        res[y][x] = numIterations;
                        allBlack = false;
                    } else {
                        res[y][x] = 0;
                    }
                }
            }

            if (allBlack && pass == 0) {
                pass = 2;
            } else {
                if (!restart) {
                    //qDebug() << pass;
                    emit renderedImage(res);
                }
                ++pass;
            }
        }
        mutex.lock();
        if (!restart)
            condition.wait(&mutex);
        restart = false;
        mutex.unlock();
    }
}



