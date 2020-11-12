#include"mainwindow.h"
#include<QMessageBox>
#include<QImage>
#include<QPixmap>
#include<QColor>
#include <complex>
#include <cmath>

QImage PlotMandel(int colormap = 100) {
    QImage img(800, 800, QImage::Format_RGB32);
    if (img.isNull()) {
        exit(1);
    }
    img.fill(qRgb(0,0,0));
    for (int x = 0; x < 800; ++x) {
        for (int y = 0; y < 800; ++y) {
            double x0 = x, y0 = y;
            x0 = x0 / 200 - 2;
            y0 = y0 / 200 - 2;
            std::complex<double> c(x0, y0);
            std::complex<double> z(0, 0);
            for (int i = 0; i < 30; ++i) {
                z = z * z + c;
                if (abs(z) > 2) {
                    img.setPixel(x, y, qRgb((i * colormap) % 255, (i * colormap * 3) % 255, (i * colormap * 5) % 255));
                    break;
                }
            }

        }
    }
    return img;
}

MainWindow::MainWindow(QWidget* parent) : QDialog(parent) {
    lbl = new QLabel("Enter colormap");

    pic = new QLabel;
    QImage img = PlotMandel();
    pic->setPixmap(QPixmap::fromImage(img));

    line = new QLineEdit;

    //cb1 = new QCheckBox("Upper case");

    //cb2 = new QCheckBox("Inverse");

    ok = new QPushButton("Plot");
    ok->setDefault(true);
    ok->setEnabled(false);

    close = new QPushButton("Close");

    clear = new QPushButton("Clear");

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(lbl);
    layout->addWidget(line);

    QVBoxLayout* right = new QVBoxLayout;
    right->addLayout(layout);
    //right->addWidget(cb1);
    //right->addWidget(cb2);
    right->addWidget(ok);
    right->addWidget(clear);
    right->addWidget(close);


    QHBoxLayout* main = new QHBoxLayout;
    main->addWidget(pic);
    main->addLayout(right);

    setLayout(main);
    setWindowTitle("Pipka");

    connect(line, SIGNAL(textChanged(QString)), this, SLOT(TextChanged(QString)));
    connect(close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ok, SIGNAL(clicked()), this, SLOT(OkClicked()));
    connect(clear, SIGNAL(clicked()), this, SLOT(ClearClicked()));
}

void MainWindow::TextChanged(QString str) {
    ok->setEnabled(!str.isEmpty());
}

void MainWindow::ClearClicked() {
    QImage img(800, 800, QImage::Format_RGB32);
    img.fill(qRgb(255,255,255));
    pic->setPixmap(QPixmap::fromImage(img));
}



void MainWindow::OkClicked() {
    int num1 = line->text().toInt();
    QImage img = PlotMandel(num1);
    pic->setPixmap(QPixmap::fromImage(img));
}
