#include "savewindow.h"
#include "mainview.h"

#include <QStyleFactory>
#include <QFileDialog>

SaveWindow::SaveWindow(QWidget* parent,
                       const MainScene* scene_ptr,
                       QMap<QChar, QString>* rules_map,
                       QString* axiom_str,
                       QMap<QChar, QString>* actions_map,
                       double angle,
                       int iterations,
                       QVector<QVector<double>>* func_table,
                       QPointF start_point)
    : parentW(parent)
    , scene(new MainScene(scene_ptr))
    , main_scene(scene_ptr)
    , rules_map(rules_map)
    , axiom_str(axiom_str)
    , actions_map(actions_map)
    , angle(angle)
    , iterations(iterations)
    , func_table(func_table)
    , start_point(start_point) {
    // note that copy constructor of main scene does not copy
    // thread and a few fields

    //this->setStyle(QStyleFactory::create("windowsvista"));
    main = new QWidget;
    parentW->setEnabled(false);

    progress_bar = new QProgressBar(this);
    progress_bar->setAlignment(Qt::AlignHCenter);
    progress_bar->setMinimum(0);
    progress_bar->setFormat("Rendering image: %p%");
    progress_bar->hide();

    filename_layout = new QHBoxLayout;
    filename_lbl = new QLabel("Chosen file: None");
    file_chooser = new QPushButton("Choose file");
    filename_layout->addWidget(file_chooser);
    filename_layout->addWidget(filename_lbl);

    resolution_layout = new QHBoxLayout;
    resolution_chooser = new QComboBox(main);
    resolution_chooser->addItems(QStringList({
                                                 "600x600",
                                                 "720x480",
                                                 "720x576",
                                                 "800x800",
                                                 "1280x720",
                                                 "1920x1080 (HD)",
                                                 "3840x2160 (4k UHD)",
                                                 "7680x4320 (8k UHD)",
                                                 QString::number(main_scene->visible_rect.width()) +
                                                 "x" +
                                                 QString::number(main_scene->visible_rect.height()) +
                                                 " (current)"
                                             }));
    resolution_layout->addWidget(resolution_chooser);
    if (scene_ptr->thread->fractal_type == Fractals::IFS ||
            scene_ptr->thread->fractal_type == Fractals::LSys) {
        resolution_chooser->setCurrentIndex(8);
        resolution_chooser->setEnabled(false);
    }

    save_layout = new QHBoxLayout;
    save_file_btn = new QPushButton("Save!");
    save_layout->addWidget(save_file_btn);

    main_layout = new QVBoxLayout;
    main_layout->addLayout(filename_layout);
    main_layout->addLayout(resolution_layout);
    main_layout->addLayout(save_layout);
    main_layout->addWidget(progress_bar);
    main_layout->insertSpacing(0, 20);
    main_layout->insertSpacing(2, 20);
    main_layout->insertSpacing(4, 20);
    main_layout->insertSpacing(6, 20);
    main_layout->insertSpacing(8, 20);

    main->setLayout(main_layout);
    setCentralWidget(main);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("Save Fractal");

    connect(save_file_btn, SIGNAL(clicked()), this, SLOT(SaveClicked()));
    connect(file_chooser, SIGNAL(clicked()), this, SLOT(FileChooserClicked()));

    connect(scene, &MainScene::started, this, [=](int max){progress_bar->setMaximum(max);});
    connect(scene, &MainScene::nextLine, this, [=](int val){progress_bar->setValue(val);});
    connect(scene, &MainScene::format, this, [=](QString f){progress_bar->setFormat(f);});

    this->show();
}

SaveWindow::~SaveWindow() {
    parentW->setEnabled(true);
}

void SaveWindow::FileChooserClicked() {
    /*filename = "sas";QFileDialog::getSaveFileName(this,
                                            "Save File",
                                            "my_fractal.png",
                                            "Images (*.png)");*/
    //QFileDialog dialog;
    //dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    //qDebug() << QStyleFactory::keys();
    //dialog.setStyle(QStyleFactory::create("windowsvista"));
    //dialog.setStyleSheet(styleSheet());
    //QFileDialog::getSaveFileName(this, "Save File", "my_fractal.png", "Images (*.png)");
    filename = QFileDialog::getSaveFileName(this,
                               "Save File",
                               "my_fractal.png",
                               "Images (*.png)", nullptr);
    filename_lbl->setText("Chosen file: " + filename);
}

void SaveWindow::SaveClicked() {
    if (filename.isEmpty()) {
        filename_lbl->setText("Choose file first!");
        return;
    }
    QStringList lst = resolution_chooser->currentText().split(" ").first().split("x");  // for example 10x15

    int resolution_x = lst.first().toInt(), resolution_y = lst.last().toInt();

    QSize size(resolution_x, resolution_y);
    QImage tempMask = QImage(size, QImage::Format_MonoLSB);
    tempMask.fill(1);

    scene->scale *= static_cast<double>(main_scene->visible_rect.width()) / resolution_x;

    switch (main_scene->thread->fractal_type) {
    case Fractals::Mandelbrot:
        scene->thread = new Mandelbrot_Julia_Thread;
        connect(scene->thread, SIGNAL(ImageReady()), this, SLOT(ImageCreated()));
        scene->thread->fractal_type = Fractals::Mandelbrot;
        scene->thread->fparser = &scene->funcEnter;
        scene->thread->fparser->parse_two_vars();

        scene->connectAll();
        progress_bar->show();

        scene->thread->render(scene->x_coord, scene->y_coord, scene->scale, size, tempMask, 1);
        return;
    case Fractals::JuliaSet:
        scene->thread = new Mandelbrot_Julia_Thread;
        connect(scene->thread, SIGNAL(ImageReady()), this, SLOT(ImageCreated()));
        scene->thread->fractal_type = Fractals::JuliaSet;
        scene->thread->fparser = &scene->funcEnter;
        scene->thread->fparser->parse_one_var();

        scene->connectAll();
        progress_bar->show();

        scene->thread->render(scene->x_coord, scene->y_coord, scene->scale, size, tempMask, 1);
        break;
    case Fractals::Newton:
        scene->thread = new Newton_Thread;
        connect(scene->thread, SIGNAL(ImageReady()), this, SLOT(ImageCreated()));
        scene->thread->fractal_type = Fractals::Newton;
        scene->thread->fparser = &scene->funcEnter;
        scene->thread->fparser_derivative = scene->derivativeEnter;

        scene->thread->fparser->parse_one_var();
        scene->thread->fparser_derivative->parse_one_var();

        scene->thread->newton_a_real = main_scene->thread->newton_a_real;
        scene->thread->newton_a_imag = main_scene->thread->newton_a_imag;
        scene->thread->newton_c_real = main_scene->thread->newton_c_real;
        scene->thread->newton_c_imag = main_scene->thread->newton_c_imag;
        scene->thread->calc_flag = main_scene->thread->calc_flag;
        scene->thread->tolerance = main_scene->thread->tolerance;
        scene->thread->MaxIterations = main_scene->thread->MaxIterations;

        scene->connectAll();
        progress_bar->show();

        scene->thread->render(scene->x_coord, scene->y_coord, scene->scale, size, tempMask, 1);
        break;
    case Fractals::Secant:
        scene->thread = new Newton_Thread;
        connect(scene->thread, SIGNAL(ImageReady()), this, SLOT(ImageCreated()));
        scene->thread->fractal_type = Fractals::Secant;
        scene->thread->fparser = &scene->funcEnter;

        scene->thread->fparser->parse_one_var();

        scene->thread->newton_a_real = main_scene->thread->newton_a_real;
        scene->thread->newton_a_imag = main_scene->thread->newton_a_imag;
        scene->thread->newton_c_real = main_scene->thread->newton_c_real;
        scene->thread->newton_c_imag = main_scene->thread->newton_c_imag;
        scene->thread->calc_flag = main_scene->thread->calc_flag;
        scene->thread->tolerance = main_scene->thread->tolerance;
        scene->thread->MaxIterations = main_scene->thread->MaxIterations;

        scene->connectAll();
        progress_bar->show();

        scene->thread->render(scene->x_coord, scene->y_coord, scene->scale, size, tempMask, 1);
        break;
    case Fractals::IFS:
        scene->image = QImage(main_scene->visible_rect.width(), main_scene->visible_rect.height(), QImage::Format_RGB32);
        scene->image.fill(QColor(Qt::white).rgb());
        scene->drawIFS(*func_table, iterations, start_point);
        this->ImageCreated();
        break;
    case Fractals::LSys:
        scene->image = QImage(main_scene->visible_rect.width(), main_scene->visible_rect.height(), QImage::Format_RGB32);
        scene->image.fill(QColor(Qt::white).rgb());
        scene->drawLSys(*rules_map, *axiom_str, *actions_map, angle, iterations);
        this->ImageCreated();
        break;
    }
}

void SaveWindow::ImageCreated() {
    QPainter painter(&scene->image);
    Fractals f_type = main_scene->thread->fractal_type;

    if (f_type == Fractals::Mandelbrot || f_type == Fractals::JuliaSet ||
            f_type == Fractals::Newton || f_type == Fractals::Secant) {
        scene->render(&painter);
    } else {
        QRectF rect(0, 0,
                    main_scene->visible_rect.width(),
                    main_scene->visible_rect.height());
        QRectF rect2(main_scene->visible_rect.x(),
                    main_scene->visible_rect.y(),
                    main_scene->visible_rect.width(),
                    main_scene->visible_rect.height());
        scene->render(&painter, rect, rect2);
    }

    bool flag = scene->image.save(filename);
    Q_ASSERT(flag);
    this->close();
}



