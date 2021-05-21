#include"mainwindow.h"

double paletteLen(int val) {
    return exp(float(val) / 10000);
}

double paletteOff(int val) {
    return (double)val / 1000;
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    main = new QWidget;
    progress_bar = new QProgressBar(this);
    progress_bar->setAlignment(Qt::AlignHCenter);
    progress_bar->setMinimum(0);
    progress_bar->setFormat("Iteration 1 of 1: %p%");

    colormaps_combo = new QComboBox();
    colormaps_combo->addItems(QStringList({ "Basic", "GreyScale", "Fire", "Wine", "Autumn", "Lazurite"}));
    lbl_colormap = new QLabel("Select Colormap:");

    scene = new MainScene(this);
    view = new MainView(scene);

    ok = new QPushButton("Plot");
    ok->setDefault(true);
    int def_h = ok->minimumSizeHint().height();

    mouse_position = new QLabel("Real: 0  Imag: 0");
    mouse_position->setMargin(0);
    mouse_position->setAlignment(Qt::AlignCenter);
    view->mouse_position = mouse_position;

    palette_offset_slider = new QSlider(Qt::Vertical);
    palette_offset_slider->setMinimum(0);
    palette_offset_slider->setMaximum(1000);
    start_offset = 920;
    scene->palette_offset = paletteOff(start_offset);
    palette_offset_slider->setSliderPosition(start_offset);

    palette_length_slider = new QSlider(Qt::Vertical);
    palette_length_slider->setMinimum(-20000);
    palette_length_slider->setMaximum(45000);
    start_length = 25000;
    scene->palette_length = paletteLen(start_length);
    palette_length_slider->setSliderPosition(start_length);

    lbl_offset = new QLabel("Palette offset");
    lbl_length = new QLabel("Palette length");
    lbl_offset->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    lbl_length->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    offset_slider_layout = new QVBoxLayout;
    offset_slider_layout->addWidget(lbl_offset);
    offset_slider_layout->addWidget(palette_offset_slider);
    offset_slider_layout->setSpacing(7);
    offset_slider_layout->setAlignment(palette_offset_slider, Qt::AlignHCenter);

    length_slider_layout = new QVBoxLayout;
    length_slider_layout->addWidget(lbl_length);
    length_slider_layout->addWidget(palette_length_slider);
    length_slider_layout->setSpacing(7);
    length_slider_layout->setAlignment(palette_length_slider, Qt::AlignHCenter);

    sliders_and_names = new QHBoxLayout;
    sliders_and_names->addLayout(offset_slider_layout);
    sliders_and_names->addLayout(length_slider_layout);

    close = new QPushButton("Close");

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(lbl_colormap);
    layout->addWidget(colormaps_combo);
    layout->setStretch(0, 1);
    layout->setStretch(1, 10);

    newton_type_changer = new QWidget;
    newton_type_lbl = new QLabel("Select type:");
    newton_type_layout = new QHBoxLayout(newton_type_changer);
    newton_type_layout->setMargin(0);
    newton_type_layout->addWidget(newton_type_lbl);
    newton_type_box = new QComboBox;
    newton_type_box->addItems(QStringList({"Root", "Speed"}));
    newton_type_layout->addWidget(newton_type_box);
    newton_type_changer->hide();

    newton_args = new QWidget;
    newton_args_layout = new QVBoxLayout(newton_args);
    newton_args_layout->setMargin(0);
    newton_c_layout = new QHBoxLayout;
    newton_c_lbl1 = new QLabel("Enter c:");
    newton_c_real = new QLineEdit("0");
    newton_c_lbl2 = new QLabel("+");
    newton_c_imag = new QLineEdit("0");
    newton_c_lbl3 = new QLabel("i");
    newton_c_layout->addWidget(newton_c_lbl1);
    newton_c_layout->addWidget(newton_c_real);
    newton_c_layout->addWidget(newton_c_lbl2);
    newton_c_layout->addWidget(newton_c_imag);
    newton_c_layout->addWidget(newton_c_lbl3);
    newton_a_layout = new QHBoxLayout;
    newton_a_lbl1 = new QLabel("Enter a:");
    newton_a_real = new QLineEdit("1");
    newton_a_lbl2 = new QLabel("+");
    newton_a_imag = new QLineEdit("0");
    newton_a_lbl3 = new QLabel("i");
    newton_a_layout->addWidget(newton_a_lbl1);
    newton_a_layout->addWidget(newton_a_real);
    newton_a_layout->addWidget(newton_a_lbl2);
    newton_a_layout->addWidget(newton_a_imag);
    newton_a_layout->addWidget(newton_a_lbl3);
    newton_args_layout->addLayout(newton_c_layout);
    newton_args_layout->addLayout(newton_a_layout);
    newton_args->hide();

    derivative_field = new QWidget;
    lbl_derivative = new QLabel("Enter derivative:");
    layout_derivEnter = new QHBoxLayout(derivative_field);
    layout_derivEnter->setMargin(0);
    layout_derivEnter->addWidget(lbl_derivative);
    scene->derivativeEnter = new FuncEnterLineEdit;
    layout_derivEnter->addWidget(scene->derivativeEnter);
    derivative_field->hide();

    lbl_funcEnter = new QLabel("Enter function:");
    QHBoxLayout* layout_funcEnter = new QHBoxLayout;
    layout_funcEnter->addWidget(lbl_funcEnter);
    layout_funcEnter->addWidget(&(scene->funcEnter));

    tolerance = new QWidget;
    tolerance_layout = new QHBoxLayout(tolerance);
    tolerance_layout->setMargin(0);
    tolerance_lbl = new QLabel("Tolerance:");
    tolerance_enter = new QLineEdit("0.000001");
    tolerance_layout->addWidget(tolerance_lbl);
    tolerance_layout->addWidget(tolerance_enter);
    tolerance->hide();

    max_iter = new QWidget;
    max_iter_layout = new QHBoxLayout(max_iter);
    max_iter_layout->setMargin(0);
    max_iter_lbl = new QLabel("Max iter:");
    max_iter_enter = new QLineEdit("100");
    max_iter_layout->addWidget(max_iter_lbl);
    max_iter_layout->addWidget(max_iter_enter);
    max_iter->hide();

    right_menu_thread = new QWidget;
    right_layout = new QVBoxLayout(right_menu_thread);
    right_layout->addWidget(progress_bar);
    right_layout->addWidget(mouse_position);
    right_layout->addLayout(layout);
    right_layout->addWidget(newton_type_changer);
    right_layout->addWidget(newton_args);
    right_layout->addWidget(tolerance);
    right_layout->addWidget(max_iter);
    right_layout->addLayout(layout_funcEnter);
    right_layout->addWidget(derivative_field);
    right_layout->addWidget(ok);
    right_layout->addLayout(sliders_and_names, 100);
    right_layout->addWidget(close);
    right_layout->setSpacing(def_h);
    right_layout->addStretch();

    right_menu_lsys = new QWidget;
    right_menu_lsys->hide();
    lsys_layout = new QVBoxLayout(right_menu_lsys);

    presets_layout = new QVBoxLayout;
    presets_lbl = new QLabel("Select preset:");
    lsys_presets = new QComboBox;
    presets_layout->addWidget(presets_lbl);
    presets_layout->addWidget(lsys_presets);
    lsys_presets->addItems(QStringList({"User Defined", "Dragon curve",
                                        "Sierpinski triangle",
                                        "Sierpinski triangle 2",
                                        "Hilbert curve",
                                        "Plant",
                                        "Tree",
                                        "Koch Snowflake",
                                        "Gosper curve",
                                        "Binary tree",
                                        "Quadratic Koch Island"}));

    vars_layout = new QVBoxLayout;
    vars_lbl = new QLabel("Enter Rules:");
    vars_layout->addWidget(vars_lbl);
    vars = new QTextEdit;
    vars_layout->addWidget(vars);
    rules = new QGroupBox("Actions");
    rules_layout = new QFormLayout(rules);

    axiom_layout = new QVBoxLayout;
    axiom = new QLineEdit();
    axiom_lbl = new QLabel("Enter axiom:");
    axiom_layout->addWidget(axiom_lbl);
    axiom_layout->addWidget(axiom);

    ang_layout = new QHBoxLayout;
    ang = new QLineEdit();
    ang_lbl = new QLabel("Angle (degrees):");
    ang_layout->addWidget(ang_lbl);
    ang_layout->addWidget(ang);

    iter_layout = new QHBoxLayout;
    iter = new QLineEdit();
    iter_lbl = new QLabel("Iterations:");
    iter_layout->addWidget(iter_lbl);
    iter_layout->addWidget(iter);
    angle_slider = new QSlider(Qt::Horizontal);
    angle_slider->setMinimum(-1800);
    angle_slider->setMaximum(1800);
    //angle_slider->setInt

    lsys_layout->addLayout(presets_layout, 1);
    lsys_layout->addLayout(vars_layout, 6);
    lsys_layout->addLayout(axiom_layout, 1);
    lsys_layout->addWidget(rules, 14);
    lsys_layout->addLayout(ang_layout, 1);
    lsys_layout->addWidget(angle_slider, 1);
    lsys_layout->addLayout(iter_layout, 1);

    right_menu_ifs = new QWidget;
    right_menu_ifs->hide();
    ifs_layout = new QVBoxLayout(right_menu_ifs);

    ifs_presets_layout = new QVBoxLayout;
    ifs_presets_lbl = new QLabel("Select preset:");
    ifs_presets = new QComboBox;
    ifs_presets_layout->addWidget(ifs_presets_lbl);
    ifs_presets_layout->addWidget(ifs_presets);
    ifs_presets->addItems(QStringList({"User Defined", "Sierpinski Triangle", "Barnsley Fern", "Square", "Pentagon", "Hexagon"}));

    points_layout = new QVBoxLayout;
    points_lbl = new QLabel("Points:");
    points_layout->addWidget(points_lbl);
    points = new QTextEdit;
    points_layout->addWidget(points);

    fcount_layout = new QHBoxLayout;
    fcount = new QLineEdit();
    fcount->setMaximumWidth(40);
    fcount_lbl = new QLabel("Number of functions:");
    start_lbl = new QLabel("Starting point:");
    start = new QLineEdit;
    start->setMaximumWidth(40);
    fcount_layout->addWidget(fcount_lbl);
    fcount_layout->addWidget(fcount);
    fcount_layout->addStretch();
    fcount_layout->addWidget(start_lbl);
    fcount_layout->addWidget(start);

    functions = new QScrollArea;
    func_widget = new QWidget;
    func_layout = new QVBoxLayout;
    func_vector = {};
    func_widget->setLayout(func_layout);
    functions->setWidget(func_widget);
    functions->setWidgetResizable(true);

    ifs_iter_layout = new QHBoxLayout;
    ifs_iter = new QLineEdit();
    ifs_iter_lbl = new QLabel("Iterations:");
    ifs_iter_layout->addWidget(ifs_iter_lbl);
    ifs_iter_layout->addWidget(ifs_iter);

    ifs_layout->addLayout(ifs_presets_layout);
    ifs_layout->addLayout(points_layout);
    ifs_layout->addLayout(fcount_layout);
    ifs_layout->addWidget(functions);
    ifs_layout->addLayout(ifs_iter_layout);
    ifs_layout->setStretch(0, 1);
    ifs_layout->setStretch(1, 3);
    ifs_layout->setStretch(2, 1);
    ifs_layout->setStretch(3, 14);
    ifs_layout->setStretch(4, 1);

    main_layout = new QHBoxLayout;

    right = new QVBoxLayout;
    right->addWidget(right_menu_thread);
    right->addWidget(right_menu_lsys);
    right->addWidget(right_menu_ifs);
    right->addWidget(ok);
    right->addWidget(close);
    main_layout->addWidget(view);
    main_layout->addLayout(right);

    main_layout->setStretch(0, 50);
    main_layout->setStretch(1, 6);

    main->setLayout(main_layout);
    setCentralWidget(main);
    setWindowTitle("Fractal Designer");

    connect(scene, &MainScene::started, this, [=](int max){progress_bar->setMaximum(max);});
    connect(scene, &MainScene::nextLine, this, [=](int val){progress_bar->setValue(val);});
    connect(scene, &MainScene::format, this, [=](QString f){progress_bar->setFormat(f);});
    connect(colormaps_combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index){ scene->current_colormap = index; scene->updateImage();});
    connect(newton_type_box, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int flag){ scene->thread->calc_flag = flag; ok->click();});
    connect(close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ok, SIGNAL(clicked()), this, SLOT(OkClicked()));
    connect(palette_offset_slider, SIGNAL(valueChanged(int)), this, SLOT(paletteOffsetChanged(int)));
    connect(palette_length_slider, SIGNAL(valueChanged(int)), this, SLOT(paletteLengthChanged(int)));

    connect(vars, SIGNAL(textChanged()), this, SLOT(varTextChanged()));
    connect(axiom, SIGNAL(textChanged(QString)), this, SLOT(axiomTextChanged(QString)));

    connect(lsys_presets, SIGNAL(activated(int)), SLOT(presetChanged(int)));

    connect(angle_slider, SIGNAL(valueChanged(int)), this, SLOT(angleChanged(int)));
    connect(ang, SIGNAL(textChanged(QString)), this, SLOT(angleTextChanged(QString)));

    connect(ifs_presets, SIGNAL(activated(int)), SLOT(ifsPresetChanged(int)));
    connect(fcount, SIGNAL(textChanged(QString)), this, SLOT(fcountChanged(QString)));

    new QShortcut(QKeySequence(Qt::Key_Return), this, SLOT(OkClicked()));
    new QShortcut(QKeySequence(Qt::Key_E), this, SLOT(zoomIn()));
    new QShortcut(QKeySequence(Qt::Key_Q), this, SLOT(zoomOut()));
}

void MainWindow::zoomIn() {
    view->wheelEvent(new QWheelEvent({0, 0}, {0, 0}, {0, 0}, {0, 1}, Qt::NoButton, Qt::NoModifier, Qt::NoScrollPhase, 0));
}

void MainWindow::zoomOut() {
    view->wheelEvent(new QWheelEvent({0, 0}, {0, 0}, {0, 0}, {0, -1}, Qt::NoButton, Qt::NoModifier, Qt::NoScrollPhase, 0));
}

void MainWindow::fcountChanged(QString s) {
    int n = s.toInt();
    if (n == 0 || n > 100) return;
    int old_size = func_vector.size();
    if (n < old_size) {
        for (int i = n; i < old_size; ++i) {
            func_layout->removeWidget(func_vector[i]);
            delete func_vector[i];
        }
        func_vector.resize(n);
    } else {
        for (int i = old_size; i < n; ++i) {
            func_vector.push_back(new IFSFunction(QString("Function %0").arg(i + 1)));
            func_layout->addWidget(func_vector.last());
        }
    }
}

void MainWindow::ifsPresetChanged(int val) {
    if (val == 0) {
        points->setText("");
        fcount->setText("1");
        start->setText("");
        func_vector[0]->setFunc(0, {"", "", "", "", "", ""});
    }
    if (val == 1) {
        points->setText("A=(-100,58)\nB=(100,58)\nC=(0,-115.5)\nS=(0,0)\nr=0.5\np=0.333333");
        fcount->setText("3");
        start->setText("S");
        func_vector[0]->setFunc(0, {"p", "r", "0", "0", "r", "A"});
        func_vector[1]->setFunc(0, {"p", "r", "0", "0", "r", "B"});
        func_vector[2]->setFunc(0, {"p", "r", "0", "0", "r", "C"});
        ifs_iter->setText("5000");
    }
    if (val == 2) {
        points->setText("A=(0,-100)\nS=(0,0)");
        fcount->setText("4");
        start->setText("S");
        func_vector[0]->setFunc(0, {"0.01", "0", "0", "0", "0.16", ""});
        func_vector[1]->setFunc(0, {"0.85", "0.85", "0.04", "-0.04", "0.85", "A"});
        func_vector[2]->setFunc(0, {"0.07", "0.2", "-0.26", "0.23", "0.22", "A"});
        func_vector[3]->setFunc(0, {"0.07", "-0.15", "0.28", "0.26", "0.24", "A"});
        ifs_iter->setText("12500");
    }
    if (val == 3) {
        points->setText("A=(-100,-100)\nB=(100,-100)\nC=(-100,100)\nD=(100,100)\nS=(0,0)\nr=0.5\np=0.2");
        fcount->setText("5");
        start->setText("S");
        func_vector[0]->setFunc(0, {"p", "r", "0", "0", "r", "A"});
        func_vector[1]->setFunc(0, {"p", "r", "0", "0", "r", "B"});
        func_vector[2]->setFunc(0, {"p", "r", "0", "0", "r", "C"});
        func_vector[3]->setFunc(0, {"p", "r", "0", "0", "r", "D"});
        func_vector[4]->setFunc(1, {"p", "S", "75"});
        ifs_iter->setText("25000");
    }
    if (val == 4) {
        points->setText("A=(0,100)\nB=(-95.1,30.1)\nC=(-58.8,-80.9)\nD=(58.8,-80.9)\nE=(95.1,30.1)\nS=(0,0)\nr=0.4\np=0.2");
        fcount->setText("5");
        start->setText("S");
        func_vector[0]->setFunc(0, {"p", "r", "0", "0", "r", "A"});
        func_vector[1]->setFunc(0, {"p", "r", "0", "0", "r", "B"});
        func_vector[2]->setFunc(0, {"p", "r", "0", "0", "r", "C"});
        func_vector[3]->setFunc(0, {"p", "r", "0", "0", "r", "D"});
        func_vector[4]->setFunc(0, {"p", "r", "0", "0", "r", "E"});
        ifs_iter->setText("25000");
    }
    if (val == 5) {
        points->setText("A=(50,86.6)\nB=(-50,86.6)\nC=(-100,0)\nD=(-50,-86.6)\nE=(50,-86.6)\nF=(100,0)\nS=(0,0)\nr=0.3\np=0.16666");
        fcount->setText("6");
        start->setText("S");
        func_vector[0]->setFunc(0, {"p", "r", "0", "0", "r", "A"});
        func_vector[1]->setFunc(0, {"p", "r", "0", "0", "r", "B"});
        func_vector[2]->setFunc(0, {"p", "r", "0", "0", "r", "C"});
        func_vector[3]->setFunc(0, {"p", "r", "0", "0", "r", "D"});
        func_vector[4]->setFunc(0, {"p", "r", "0", "0", "r", "E"});
        func_vector[5]->setFunc(0, {"p", "r", "0", "0", "r", "F"});
        ifs_iter->setText("10000");
    }
    ok->click();
    view->resetTransform();
    view->centerOn(0, 0);

}

void MainWindow::collectIFS() {
    points_map.clear();
    const_map.clear();
    iterations = ifs_iter->text().toInt();
    QStringList list = points->toPlainText().split(QRegExp("[\\s+]"), Qt::SkipEmptyParts);
    for (QString pair : list) {
        QStringList p = pair.split(QRegExp("[=:]"));
        if (p.size() != 2) continue;
        QStringList coords = p[1].split(QRegExp("[(){};,]"), Qt::SkipEmptyParts);
        if (coords.size() == 1) {const_map[p[0]] = coords[0].toDouble();}
        if (coords.size() != 2) continue;
        points_map[p[0]] = {coords[0].toDouble(), coords[1].toDouble()};
    }
    start_point = points_map[start->text()];
    func_table.clear();
    for (IFSFunction* f : func_vector) {
        QVector<QString> r = f->getInfo();
        if (r.empty()) continue;
        QVector<double> vd = {};
        auto f0 = const_map.find(r[1]);
        if (f0 != const_map.end()) vd.append(f0.value());
        else vd.append(r[1].toDouble());
        if (r[0] == "0") {
            vd.append(0);
            for (int i = 2; i <=5; ++i) {
                auto f = const_map.find(r[i]);
                if (f != const_map.end()) vd.append(f.value());
                else vd.append(r[i].toDouble());
            }
            QPointF z = points_map[r[6]];
            vd.append(z.x());
            vd.append(z.y());

        } else {
            vd.append(1);
            QPointF p1 = points_map[r[2]];
            vd.append(p1.x());
            vd.append(p1.y());
            auto f = const_map.find(r[3]);
            if (f != const_map.end()) vd.append(f.value());
            else vd.append(r[3].toDouble());
        }
        func_table.append(vd);
    }
}

void MainWindow::TextChanged(QString str) {
    palette_offset_slider->setValue(std::stoi(str.toStdString()));
}

void MainWindow::angleChanged(int val) {
    QString f = QString::number((double)val / 10);
    if (f != ang->text()) {
        ang->setText(f);
    }
    ok->click();
}

void MainWindow::angleTextChanged(QString f) {
    int val = f.toDouble() * 10;
    disconnect(angle_slider, SIGNAL(valueChanged(int)), this, SLOT(angleChanged(int)));
    angle_slider->setSliderPosition(val);
    connect(angle_slider, SIGNAL(valueChanged(int)), this, SLOT(angleChanged(int)));
}

void MainWindow::presetChanged(int val) {
    if (val == 0) {
        vars->setText("");
        axiom->setText("");
    }
    if (val == 1) {
        vars->setText("X:X+YF+\n"
                      "Y:-FX-Y");
        axiom->setText("X");
        rule_lines['X']->setText("");
        rule_lines['Y']->setText("");
        rule_lines['F']->setText("F");
        ang->setText("90");
        iter->setText("12");
    }
    if (val == 2) {
        vars->setText("A:B-A-B\n"
                      "B:A+B+A");
        axiom->setText("A");
        rule_lines['A']->setText("F");
        rule_lines['B']->setText("F");
        ang->setText("60");
        iter->setText("7");
    }
    if (val == 3) {
        vars->setText("F:F-G+F+G-F\n"
                      "G:GG");
        axiom->setText("F-G-G");
        rule_lines['F']->setText("F");
        rule_lines['G']->setText("F");
        ang->setText("120");
        iter->setText("6");
    }
    if (val == 4) {
        vars->setText("A:+BF-AFA-FB+\n"
                      "B:-AF+BFB+FA-");
        axiom->setText("A");
        rule_lines['A']->setText("");
        rule_lines['B']->setText("");
        rule_lines['F']->setText("F");
        ang->setText("90");
        iter->setText("7");
    }
    if (val == 5) {
        vars->setText("X:F+[[X]-X]-F[-FX]+X\n"
                      "F:FF");
        axiom->setText("---X");
        rule_lines['F']->setText("F");
        ang->setText("25");
        iter->setText("7");
    }
    if (val == 6) {
        vars->setText("F:FF-[-F+F+F]+[+F-F-F]\n");
        axiom->setText("----F");
        rule_lines['F']->setText("F");
        ang->setText("22.5");
        iter->setText("5");
    }
    if (val == 7) {
        vars->setText("F:F+F--F+F\n");
        axiom->setText("+F--F--F");
        rule_lines['F']->setText("F");
        ang->setText("60");
        iter->setText("5");
    }
    if (val == 8) {
        vars->setText("A:A-B--B+A++AA+B-\n"
                      "B:+A-BB--B-A++A+B");
        axiom->setText("A");
        rule_lines['A']->setText("F");
        rule_lines['B']->setText("F");
        ang->setText("60");
        iter->setText("5");
    }
    if (val == 9) {
        vars->setText("A:B[-A]+A\n"
                      "B:BB");
        axiom->setText("A");
        rule_lines['A']->setText("F");
        rule_lines['B']->setText("F");
        ang->setText("45");
        iter->setText("5");
    }
    if (val == 10) {
            vars->setText("F:F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF\n"
                          "f:ffffff");
            axiom->setText("F+F+F+F");
            rule_lines['F']->setText("F");
            rule_lines['f']->setText("f");
            ang->setText("90");
            iter->setText("2");
        }
    ok->click();
    view->resetTransform();
    view->centerOn(0, 0);
}

void MainWindow::OkClicked() {
    switch (scene->thread->fractal_type) {
    case Fractals::Mandelbrot:
        scene->funcEnter.parse_two_vars();
        break;
    case Fractals::JuliaSet:
        scene->funcEnter.parse_one_var();
        break;
    case Fractals::Newton:
        if (newton_a_real->text().isEmpty()) {
            newton_a_real->setText("1");
        }
        if (newton_a_imag->text().isEmpty()) {
            newton_a_imag->setText("0");
        }
        if (newton_c_real->text().isEmpty()) {
            newton_c_real->setText("0");
        }
        if (newton_c_imag->text().isEmpty()) {
            newton_c_imag->setText("0");
        }
        if (tolerance_enter->text().isEmpty()) {
            tolerance_enter->setText("1e-6");
        }
        if (max_iter_enter->text().isEmpty()) {  // TODO: check if < 0?
            max_iter_enter->setText("100");
        }
        scene->thread->newton_a_real = static_cast<type>(newton_a_real->text().toDouble());
        scene->thread->newton_a_imag = static_cast<type>(newton_a_imag->text().toDouble());
        if (newton_c_real->text() == "z") {
            scene->thread->is_mandel_nova = true;
        } else {
            scene->thread->is_mandel_nova = false;
            scene->thread->newton_c_real = static_cast<type>(newton_c_real->text().toDouble());
            scene->thread->newton_c_imag = static_cast<type>(newton_c_imag->text().toDouble());
        }
        scene->thread->tolerance = static_cast<type>(tolerance_enter->text().toDouble());
        scene->thread->MaxIterations = max_iter_enter->text().toULong();
        scene->funcEnter.parse_one_var();
        scene->derivativeEnter->parse_one_var();
        break;
    case Fractals::Secant:
        scene->funcEnter.parse_one_var();
        if (tolerance_enter->text().isEmpty()) {
            tolerance_enter->setText("1e-6");
        }
        if (max_iter_enter->text().isEmpty()) {  // TODO: check if < 0?
            max_iter_enter->setText("100");
        }
        scene->thread->tolerance = tolerance_enter->text().toDouble();
        scene->thread->MaxIterations = max_iter_enter->text().toULong();
        break;
    case Fractals::IFS:
        scene->clear();
        collectIFS();
        scene->drawIFS(func_table, iterations, start_point);
        break;
    case Fractals::LSys:
        scene->clear();
        collect();
        scene->drawLSys(rules_map, axiom_str, actions_map, angle, iterations);
        return;
    }
    scene->reset();
    view->render();
}

void MainWindow::paletteOffsetChanged(int val) {
    scene->palette_offset = paletteOff(val);
    scene->updateImage();
}

void MainWindow::clearActions() {
    actions_map.clear();
    actions_map['+'] = "+";
    actions_map['-'] = "-";
}


void MainWindow::paletteLengthChanged(int val) {
    scene->palette_length = paletteLen(val);
    scene->updateImage();
}

void MainWindow::varTextChanged() {
    QString text = vars->toPlainText().simplified();
    for (auto it = text.cbegin(); it != text.cend(); ++it) {
        QChar c = *it;
        if (c.isLetter()) {
            auto t = rule_lines.find(c);
            if (t == rule_lines.end()) {
                rule_lines[c] = new QLineEdit;
                rules_layout->addRow(c, rule_lines[c]);
                if (c == 'F') {
                    rule_lines[c]->setText("F");
                }
            }
        }
    }
    std::list<std::pair<QChar,QLineEdit*>> lst;
    for (auto it = rule_lines.begin(); it != rule_lines.end(); ++it) {
        QChar c = it.key();
        if (!text.contains(c)) {
            lst.push_back({c, it.value()});
        }
    }
    for (auto elem : lst) {
        rules_layout->removeRow(elem.second);
        rule_lines.remove(elem.first);
    }
    QStringList r = text.split(QRegExp("\\s+"));
    rules_map.clear();
    for (QString s : r) {
        QStringList pair = s.split(':');
        if (pair.size() == 2) {
            rules_map[pair[0][0]] = pair[1];
        }
    }
}

void MainWindow::collect() {
    clearActions();
    for (auto it = rule_lines.begin(); it != rule_lines.end(); ++it) {
        QChar c = it.key();
        auto p = it.value();
        actions_map[c] = p->text();
    }
    angle = ang->text().toDouble();
    iterations = iter->text().toInt();
}

void MainWindow::axiomTextChanged(QString f) {
    axiom_str = f.simplified();
}

void MainWindow::ChangeToMandelbrot() {
    if (scene->thread->fractal_type != Fractals::Mandelbrot) {
        view->init();
        scene->clearScene();
        derivative_field->hide();
        newton_type_changer->hide();
        newton_args->hide();
        tolerance->hide();
        max_iter->hide();

        lbl_offset->show();
        palette_offset_slider->show();
        palette_length_slider->setOrientation(Qt::Vertical);
        length_slider_layout->setAlignment(palette_length_slider, Qt::AlignHCenter);

        disconnect(scene->thread, &MainThread::renderedImage,
                scene, &MainScene::setValueMatrix);
        delete scene->thread;

        scene->funcEnter.setText("z^2+c");
        scene->thread = new Mandelbrot_Julia_Thread;
        scene->thread->fractal_type = Fractals::Mandelbrot;
        scene->thread->fparser = &scene->funcEnter;
        scene->connectAll();
        palette_length_slider->setSliderPosition(start_length);
        paletteLengthChanged(start_length);
    }
    right_layout->setStretchFactor(sliders_and_names, 100);
    main_layout->setStretch(1, 6);
    right_menu_lsys->hide();
    right_menu_ifs->hide();
    right_menu_thread->show();
    ok->click();
}

void MainWindow::ChangeToJuliaSet() {
    if (scene->thread->fractal_type != Fractals::JuliaSet) {
        view->init();
        scene->clearScene();
        derivative_field->hide();
        newton_type_changer->hide();
        newton_args->hide();
        tolerance->hide();
        max_iter->hide();

        lbl_offset->show();
        palette_offset_slider->show();
        palette_length_slider->setOrientation(Qt::Vertical);
        length_slider_layout->setAlignment(palette_length_slider, Qt::AlignHCenter);

        disconnect(scene->thread, &MainThread::renderedImage,
                scene, &MainScene::setValueMatrix);
        delete scene->thread;

        scene->funcEnter.setText("z^2-0.4-0.59i");
        scene->thread = new Mandelbrot_Julia_Thread;
        scene->thread->fractal_type = Fractals::JuliaSet;
        scene->thread->fparser = &scene->funcEnter;
        scene->connectAll();
        palette_length_slider->setSliderPosition(start_length);
        paletteLengthChanged(start_length);
    }
    right_layout->setStretchFactor(sliders_and_names, 100);
    main_layout->setStretch(1, 6);
    right_menu_lsys->hide();
    right_menu_ifs->hide();
    right_menu_thread->show();
    ok->click();
}

void MainWindow::ChangeToNewton() {
    if (scene->thread->fractal_type != Fractals::Newton) {
        view->init();
        scene->clearScene();
        // create field for derivative
        derivative_field->show();
        newton_type_changer->show();
        newton_args->show();
        tolerance->show();
        tolerance_enter->setText("1e-6");
        max_iter->show();
        max_iter_enter->setText("100");

        // hide one slider
        lbl_offset->hide();
        palette_offset_slider->hide();
        palette_length_slider->setOrientation(Qt::Horizontal);
        length_slider_layout->setAlignment(palette_length_slider, Qt::AlignVCenter | Qt::AlignTop);

        disconnect(scene->thread, &MainThread::renderedImage,
                scene, &MainScene::setValueMatrix);
        delete scene->thread;
        scene->funcEnter.setText("z^3-1");
        scene->derivativeEnter->setText("3*z^2");
        scene->thread = new Newton_Thread;
        scene->thread->fractal_type = Fractals::Newton;
        scene->thread->fparser = &scene->funcEnter;
        scene->thread->fparser_derivative = scene->derivativeEnter;
        scene->connectAll();
        palette_length_slider->setSliderPosition(palette_length_slider->minimum() + 300);
        paletteLengthChanged(palette_length_slider->minimum() + 300);
    }
    right_layout->setStretchFactor(sliders_and_names, 0);
    main_layout->setStretch(1, 6);
    right_menu_lsys->hide();
    right_menu_ifs->hide();
    right_menu_thread->show();
    ok->click();
}

void MainWindow::ChangeToSecant() {
    if (scene->thread->fractal_type != Fractals::Secant) {
        view->init();
        scene->clearScene();
        // create field for derivative
        derivative_field->hide();
        newton_type_changer->show();
        newton_args->hide();
        tolerance->show();
        tolerance_enter->setText("1e-6");
        max_iter->show();
        max_iter_enter->setText("100");

        // hide one slider
        lbl_offset->hide();
        palette_offset_slider->hide();
        palette_length_slider->setOrientation(Qt::Horizontal);
        length_slider_layout->setAlignment(palette_length_slider, Qt::AlignVCenter | Qt::AlignTop);

        disconnect(scene->thread, &MainThread::renderedImage,
                scene, &MainScene::setValueMatrix);
        delete scene->thread;
        scene->funcEnter.setText("z^3-1");
        scene->thread = new Newton_Thread;
        scene->thread->fractal_type = Fractals::Secant;
        scene->thread->fparser = &scene->funcEnter;
        scene->connectAll();
        palette_length_slider->setSliderPosition(palette_length_slider->minimum() + 300);
        paletteLengthChanged(palette_length_slider->minimum() + 300);
    }
    right_layout->setStretchFactor(sliders_and_names, 0);
    main_layout->setStretch(1, 6);
    right_menu_lsys->hide();
    right_menu_ifs->hide();
    right_menu_thread->show();
    ok->click();
}

void MainWindow::ChangeToIFS() {
    if (scene->thread->fractal_type != Fractals::IFS) {
        scene->clearScene();
        derivative_field->hide();
        newton_type_changer->hide();
        newton_args->hide();
        tolerance->hide();

        view->use_thread = false;
        disconnect(scene->thread, &MainThread::renderedImage, scene, &MainScene::setValueMatrix);
        delete scene->thread;
        scene->thread = new IFS_Thread;
        scene->thread->fractal_type = Fractals::IFS;
        scene->thread->fparser = &scene->funcEnter;
        scene->thread->fparser_derivative = scene->derivativeEnter;
        scene->connectAll();
        ifs_presets->setCurrentIndex(1);
        ifsPresetChanged(1);
    }
    functions->setMinimumWidth(func_widget->minimumSizeHint().width() + functions->verticalScrollBar()->minimumSizeHint().width() + 20);
    main_layout->setStretch(1, 1);
    right_menu_lsys->hide();
    right_menu_ifs->show();
    right_menu_thread->hide();
    ok->click();
}

void MainWindow::ChangeToLSys() {
    if (scene->thread->fractal_type != Fractals::LSys) {
        scene->clearScene();
        derivative_field->hide();
        newton_type_changer->hide();
        newton_args->hide();
        tolerance->hide();
        max_iter->hide();

        lbl_offset->show();
        palette_offset_slider->show();
        palette_length_slider->setOrientation(Qt::Vertical);
        length_slider_layout->setAlignment(palette_length_slider, Qt::AlignHCenter);
        view->use_thread = false;

        disconnect(scene->thread, &MainThread::renderedImage,
                scene, &MainScene::setValueMatrix);
        delete scene->thread;

        scene->thread = new LSys_Thread;
        scene->thread->fractal_type = Fractals::LSys;
        scene->thread->fparser = &scene->funcEnter;
        scene->thread->fparser_derivative = scene->derivativeEnter;
        scene->connectAll();
        lsys_presets->setCurrentIndex(1);
        presetChanged(1);
    }
    main_layout->setStretch(1, 7);
    right_menu_lsys->show();
    right_menu_ifs->hide();
    right_menu_thread->hide();
    ok->click();
}

void MainWindow::setLog() {
    scene->method = 0;
    scene->updateImage();
}
void MainWindow::setPure() {
    scene->method = 1;
    scene->updateImage();
}

void MainWindow::OpenDocumentation() {
    QDesktopServices::openUrl(QUrl("https://github.com/VanoPekkar/fractal-designer"));
}

void MainWindow::SaveImage() {
    save_window = new SaveWindow(this,
                                 scene,
                                 &rules_map,
                                 &axiom_str,
                                 &actions_map,
                                 angle,
                                 iterations,
                                 &func_table,
                                 start_point);
}


