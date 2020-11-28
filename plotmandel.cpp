#include<QImage>
#include<QPixmap>
#include<QColor>

#include <complex>
#include <cmath>

QImage PlotMandel(int colormap = 10, long double xw0 = -2, long double yw0 = -2, long double cs = 4) {
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
                    img.setPixel(x, y, qRgb(6 * colormap * sin(abs(z)), 30 * colormap * sin(abs(z)), 40 * colormap * sin(abs(z))));
                    break;
                }
            }

        }
    }
    return img;
}
