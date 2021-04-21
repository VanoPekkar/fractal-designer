#include"mainwindow.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    main = new QWidget;

    lbl = new QLabel("Enter colormap:");

    scene = new MainScene(this);
    //scene->PlotM();
    //pixmap = scene->addPixmap(QPixmap::fromImage(img));

    view = new MainView(scene);

    line = new QLineEdit;
    line->setText("0");

    ok = new QPushButton("Plot");
    ok->setDefault(true);
//    ok->setEnabled(false);

    close = new QPushButton("Close");

//    reset = new QPushButton("Reset");

    palette_slider = new QSlider(Qt::Horizontal);
    palette_slider->setMinimum(0);
    palette_slider->setMaximum(50);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(lbl);
    layout->addWidget(line);

//    lbl_derivative = new QLabel("Enter derivative:");
//    layout_derivEnter = new QHBoxLayout;
//    layout_derivEnter->addWidget(lbl_derivative);
//    layout_derivEnter->addWidget(&(scene->derivativeEnter));

    lbl_funcEnter = new QLabel("Enter function:");
    QHBoxLayout* layout_funcEnter = new QHBoxLayout;
    layout_funcEnter->addWidget(lbl_funcEnter);
    layout_funcEnter->addWidget(&(scene->funcEnter));

    right_layout = new QVBoxLayout;
    right_layout->addLayout(layout);
    right_layout->addLayout(layout_funcEnter);
    right_layout->addWidget(ok);
//    right->addWidget(reset);
    right_layout->addWidget(close);
    right_layout->addWidget(palette_slider);


    QHBoxLayout* mainLayout = new QHBoxLayout;
    //mainLayout->addWidget(pic);
    mainLayout->addWidget(view);
    mainLayout->addLayout(right_layout);

    main->setLayout(mainLayout);
    setCentralWidget(main);
    setWindowTitle("Fractal Designer");

    connect(line, SIGNAL(textChanged(QString)), this, SLOT(TextChanged(QString)));
    connect(close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ok, SIGNAL(clicked()), this, SLOT(OkClicked()));
//    connect(reset, SIGNAL(clicked()), this, SLOT(resetClicked()));
    connect(palette_slider, SIGNAL(valueChanged(int)), this, SLOT(sliderMoved(int)));
}

void MainWindow::TextChanged(QString str) {
//    ok->setEnabled(!str.isEmpty());
    palette_slider->setValue(std::stoi(str.toStdString()));
}

//void MainWindow::resetClicked() {
//    scene->reset();
//    view->render();
//}

void MainWindow::OkClicked() {
    switch (scene->thread->fractal_type) {
    case Fractals::Mandelbrot:
        scene->funcEnter.parse_two_vars();
        break;
    case Fractals::JuliaSet:
        scene->funcEnter.parse_one_var();
        break;
    case Fractals::Newton:
        scene->funcEnter.parse_one_var();
        scene->derivativeEnter->parse_one_var();
        break;
    }
    scene->reset();
    view->render();
//    int num1 = line->text().toInt();
//    scene->temp = num1;
//    scene->updateColormap();
//    scene->drawImage();
}

void MainWindow::sliderMoved(int val) {
    //int num1 = palette_slider->value();
    line->setText(std::to_string(val).c_str());
    scene->temp = val;
    scene->updateColormap();
    //scene->drawImage();
}

void MainWindow::ChangeToMandelbrot() {
    if (scene->thread->fractal_type == Fractals::Newton) {
        // delete derivative enter for newton fractal
        delete scene->derivativeEnter;
        delete lbl_derivative;
        right_layout->removeItem(layout_derivEnter);
        delete layout_derivEnter;
    }
    disconnect(scene->thread, &RenderThread::renderedImage,
            scene, &MainScene::setValueMatrix);
    delete scene->thread;
    scene->funcEnter.setText("z^2+c");
    scene->thread = new Mandelbrot_Julia_Thread;
    scene->thread->fractal_type = Fractals::Mandelbrot;
    scene->thread->fparser = &scene->funcEnter;
    connect(scene->thread, &RenderThread::renderedImage,
            scene, &MainScene::setValueMatrix);
    ok->click();
}

void MainWindow::ChangeToJuliaSet() {
    if (scene->thread->fractal_type == Fractals::Newton) {
        // delete derivative enter for newton fractal
        delete scene->derivativeEnter;
        delete lbl_derivative;
        right_layout->removeItem(layout_derivEnter);
        delete layout_derivEnter;
    }
    disconnect(scene->thread, &RenderThread::renderedImage,
            scene, &MainScene::setValueMatrix);
    delete scene->thread;
    scene->funcEnter.setText("z^2-0,4-0,59i");
    scene->thread = new Mandelbrot_Julia_Thread;
    scene->thread->fractal_type = Fractals::JuliaSet;
    scene->thread->fparser = &scene->funcEnter;
    connect(scene->thread, &RenderThread::renderedImage,
            scene, &MainScene::setValueMatrix);
    ok->click();
}

void MainWindow::ChangeToNewton() {
    // create field for derivative
    layout_derivEnter = new QHBoxLayout;
    lbl_derivative = new QLabel("Enter derivative:");
    layout_derivEnter->addWidget(lbl_derivative);
    scene->derivativeEnter = new FuncEnterLineEdit;
    layout_derivEnter->addWidget(scene->derivativeEnter);
    right_layout->insertLayout(2, layout_derivEnter);

    disconnect(scene->thread, &RenderThread::renderedImage,
            scene, &MainScene::setValueMatrix);
    delete scene->thread;
    scene->funcEnter.setText("z^3-1");
    scene->derivativeEnter->setText("3*z^2");
    scene->thread = new Newton_Thread;
    scene->thread->fractal_type = Fractals::Newton;
    scene->thread->fparser = &scene->funcEnter;
    scene->thread->fparser_derivative = scene->derivativeEnter;
    connect(scene->thread, &RenderThread::renderedImage,
            scene, &MainScene::setValueMatrix);
    ok->click();
}

