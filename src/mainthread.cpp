#include "mainthread.h"


MainThread::MainThread(QObject *parent): QThread(parent) {

}

MainThread::~MainThread() {
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void MainThread::render(double centerX, double centerY, double scaleFactor,
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


Mandelbrot_Julia_Thread::Mandelbrot_Julia_Thread(QWidget* parent) : MainThread(parent) {

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
        emit started(resultSize.height() - 1);
        std::complex<double> params_for_parser[2];
        size_t MaxIterations;
        QImage tmask = mask;
        while (pass < NumPasses) {
            emit format(QString("Iteration %1 of %2: %p%").arg(pass + 1).arg(NumPasses));
            MaxIterations = (1 << (2 * pass + 6)) + 32;
            const int Limit = 40;
            bool allBlack = true;

            for (int y = 0; y < resultSize.height(); ++y) {
                emit nextLine(y);
                if (restart)
                    break;
                if (abort)
                    return;
                type ay = centerY + ((y - halfHeight) * scaleFactor);
                auto cur = res[y].begin();

                for (int x = 0; x < resultSize.width(); ++x) {
                    type ax = centerX + ((x - halfWidth) * scaleFactor);
                    params_for_parser[1].real(ax);
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

                    int opaque;
                    mutex.lock();
                    opaque = tmask.pixelColor(x, y).value();
                    mutex.unlock();
                    if (!opaque) {
                        *cur++ = TRANSP;
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
                    }

                    if (numIterations < MaxIterations) {
                        iters_num[y][x] = numIterations;
                        *cur++ = numIterations - nu;
                        allBlack = false;
                    } else {
                        *cur++ = 0;
                    }
                }
            }

            if (allBlack && pass == 0) {
                pass = 2;
            } else {
                if (!restart) {
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



Newton_Thread::Newton_Thread(QWidget* parent) : MainThread(parent) {

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
        std::complex<double> params_for_parser[2];
        type prev_real, prev_imag;
        QImage tmask = mask;
        emit started(resultSize.height() - 1);
        emit format(QString("%p%"));
        for (int y = 0; y < resultSize.height(); ++y) {
            emit nextLine(y);
            if (restart)
                break;
            if (abort)
                return;
            type ay = centerY + ((y - halfHeight) * scaleFactor);
            for (int x = 0; x < resultSize.width(); ++x) {
                type ax = centerX + ((x - halfWidth) * scaleFactor);
                prev_real = 0;
                prev_imag = 0;
                type a1, a2 = 0;
                type b1, b2 = 0;
                size_t numIterations;
                a1 = ax;
                b1 = ay;
                numIterations = 0;

                if (is_mandel_nova) {
                    newton_c_real = ax;
                    newton_c_imag = ay;
                }

                mutex.lock();
                int opaque = tmask.pixelColor(x, y).value();
                mutex.unlock();
                if (!opaque) {
                    res[y][x] = TRANSP;
                    continue;
                }
                while (numIterations < MaxIterations) {
                    ++numIterations;
                    prev_imag = b1;
                    prev_real = a1;
                    if (fractal_type == Fractals::Newton) {
                        params_for_parser[0].real(a1);
                        params_for_parser[0].imag(b1);
                        params_for_parser[0] = fparser->eval(params_for_parser);  // f(z)
                        a2 = params_for_parser[0].real();
                        b2 = params_for_parser[0].imag();
                        params_for_parser[0].real(a1);
                        params_for_parser[0].imag(b1);
                        params_for_parser[0] = fparser_derivative->eval(params_for_parser);  // f'(z)
                        // multiply f(z) by a
                        params_for_parser[1].real(a2 * newton_a_real - b2 * newton_a_imag);
                        b2 = a2 * newton_a_imag + b2 * newton_a_real;
                        a2 = params_for_parser[1].real();
                        // divide a*f(z) by f'(z), subtract c and subtract from z
                        a1 = a1 - (a2 * params_for_parser[0].real() + b2 * params_for_parser[0].imag()) /
                                    (params_for_parser[0].real() * params_for_parser[0].real() +
                                    params_for_parser[0].imag() * params_for_parser[0].imag()) +
                                newton_c_real;
                        b2 = b1 - (params_for_parser[0].real() * b2 - a2 * params_for_parser[0].imag()) /
                                    (params_for_parser[0].real() * params_for_parser[0].real() +
                                    params_for_parser[0].imag() * params_for_parser[0].imag()) +
                                newton_c_imag;
                        a2 = a1;
                    } else {
                        // z_{n-1} - z_{n-2}
                        type res_real = a1 - a2, res_imag = b1 - b2;

                        params_for_parser[0].real(a1);
                        params_for_parser[0].imag(b1);
                        params_for_parser[1] = fparser->eval(params_for_parser);  // f(z_{n-1})
                        params_for_parser[0].real(a2);
                        params_for_parser[0].imag(b2);
                        params_for_parser[0] = fparser->eval(params_for_parser);  // f(z_{n-2})
                        // f(z_{n-1}) * (z_{n-1} - z_{n-2})
                        a2 = res_real * params_for_parser[1].real() -
                                res_imag * params_for_parser[1].imag();
                        res_imag = res_real * params_for_parser[1].imag() +
                                res_imag * params_for_parser[1].real();
                        res_real = a2;
                        // divide result by f(z_{n-1}) - f(z_{n-2})
                        // and subtract from z_{n-1}
                        params_for_parser[1] -= params_for_parser[0];
                        a2 = a1 - (res_real * params_for_parser[1].real() + res_imag * params_for_parser[1].imag()) /
                                (params_for_parser[1].real() * params_for_parser[1].real() +
                                params_for_parser[1].imag() * params_for_parser[1].imag());
                        b2 = b1 - (params_for_parser[1].real() * res_imag - res_real * params_for_parser[1].imag()) /
                                (params_for_parser[1].real() * params_for_parser[1].real() +
                                params_for_parser[1].imag() * params_for_parser[1].imag());
                    }
                    if ((a2 - prev_real) * (a2 - prev_real) + (b2 - prev_imag) * (b2 - prev_imag) < tolerance * tolerance) {
                        break;
                    }
                    prev_imag = b2;
                    prev_real = a2;
                    ++numIterations;
                    if (fractal_type == Fractals::Newton) {
                        params_for_parser[0].real(a2);
                        params_for_parser[0].imag(b2);
                        params_for_parser[0] = fparser->eval(params_for_parser);  // f(z)
                        a1 = params_for_parser[0].real();
                        b1 = params_for_parser[0].imag();
                        params_for_parser[0].real(a2);
                        params_for_parser[0].imag(b2);
                        params_for_parser[0] = fparser_derivative->eval(params_for_parser);  // f'(z)
                        // multiply f(z) by a
                        params_for_parser[1].real(a1 * newton_a_real - b1 * newton_a_imag);
                        b1 = a1 * newton_a_imag + b1 * newton_a_real;
                        a1 = params_for_parser[1].real();
                        // divide a*f(z) by f'(z), subtract c and subtract from z
                        a2 = a2 - (a1 * params_for_parser[0].real() + b1 * params_for_parser[0].imag()) /
                                    (params_for_parser[0].real() * params_for_parser[0].real() +
                                    params_for_parser[0].imag() * params_for_parser[0].imag()) +
                                newton_c_real;
                        b1 = b2 - (params_for_parser[0].real() * b1 - a1 * params_for_parser[0].imag()) /
                                    (params_for_parser[0].real() * params_for_parser[0].real() +
                                    params_for_parser[0].imag() * params_for_parser[0].imag()) +
                                newton_c_imag;
                        a1 = a2;
                    } else {
                        // z_{n-1} - z_{n-2}
                        type res_real = a2 - a1, res_imag = b2 - b1;

                        params_for_parser[0].real(a2);
                        params_for_parser[0].imag(b2);
                        params_for_parser[1] = fparser->eval(params_for_parser);  // f(z_{n-1})
                        params_for_parser[0].real(a1);
                        params_for_parser[0].imag(b1);
                        params_for_parser[0] = fparser->eval(params_for_parser);  // f(z_{n-2})
                        // f(z_{n-1}) * (z_{n-1} - z_{n-2})
                        a1 = res_real * params_for_parser[1].real() -
                                res_imag * params_for_parser[1].imag();
                        res_imag = res_real * params_for_parser[1].imag() +
                                res_imag * params_for_parser[1].real();
                        res_real = a1;
                        // divide result by f(z_{n-1}) - f(z_{n-2})
                        params_for_parser[1] -= params_for_parser[0];
                        a1 = a2 - (res_real * params_for_parser[1].real() + res_imag * params_for_parser[1].imag()) /
                                (params_for_parser[1].real() * params_for_parser[1].real() +
                                params_for_parser[1].imag() * params_for_parser[1].imag());
                        b1 = b2 - (params_for_parser[1].real() * res_imag - res_real * params_for_parser[1].imag()) /
                                (params_for_parser[1].real() * params_for_parser[1].real() +
                                params_for_parser[1].imag() * params_for_parser[1].imag());
                    }
                }
                if (numIterations < MaxIterations) {
                    if (calc_flag == 0) {
                        res[y][x] = a1 + b1;
                    } else {
                        res[y][x] = numIterations;
                    }
                } else {
                    res[y][x] = a1 + b1;
                }
            }
        }
        if (!restart) {
            emit renderedImage(res);
        }
        mutex.lock();
        if (!restart)
            condition.wait(&mutex);
        restart = false;
        mutex.unlock();
    }
}

IFS_Thread::IFS_Thread(QWidget* parent) : MainThread(parent) {

}

IFS_Thread::~IFS_Thread() {

}

void IFS_Thread::run() {
    forever {
        if (restart)
            break;
        if (abort)
            return;
        mutex.lock();
        if (!restart)
            condition.wait(&mutex);
        restart = false;
        mutex.unlock();
    }
}

LSys_Thread::LSys_Thread(QWidget* parent) : MainThread(parent) {

}

LSys_Thread::~LSys_Thread() {

}

void LSys_Thread::run() {
    forever {
        if (restart)
            break;
        if (abort)
            return;
        mutex.lock();
        if (!restart)
            condition.wait(&mutex);
        restart = false;
        mutex.unlock();
    }
}
