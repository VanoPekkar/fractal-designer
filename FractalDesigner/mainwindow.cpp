#include"mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    main = new QWidget;

    lbl = new QLabel("Enter colormap");

    scene = new MainScene(this);
    QImage img = scene->PlotMandel();
    scene->addPixmap(QPixmap::fromImage(img));

    view = new QGraphicsView(scene);
    view->show();

    line = new QLineEdit;

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
    right->addWidget(ok);
    right->addWidget(clear);
    right->addWidget(close);


    QHBoxLayout* mainLayout = new QHBoxLayout;
    //mainLayout->addWidget(pic);
    mainLayout->addWidget(view);
    mainLayout->addLayout(right);

    main->setLayout(mainLayout);
    setCentralWidget(main);
    setWindowTitle("Fractal Designer");

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
    scene->addPixmap(QPixmap::fromImage(img));
}

void MainWindow::OkClicked() {
    int num1 = line->text().toInt();
    scene->x_coord = -2;
    scene->y_coord = -2;
    scene->width = 4;
    scene->colormap = num1;
    QImage img = scene->PlotMandel(num1, scene->x_coord, scene->y_coord, scene->width);
    scene->addPixmap(QPixmap::fromImage(img));
}

