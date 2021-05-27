#include "ifsfunc.h"

IFSFunction::IFSFunction(QString name) : QGroupBox(name) {
    ftype = 0;
    type_combo = new QComboBox;
    type_combo->addItems({"Linear", "Inverse"});
    type_combo->setMaximumWidth(type_combo->minimumSizeHint().width());
    layout = new QVBoxLayout;
    setLayout(layout);
    data = new QHBoxLayout;
    prob_lbl = new QLabel("Probabilty:");
    prob = new QLineEdit;
    int w = prob->font().pointSize();
    prob->setMaximumWidth(w * 7);
    data->addWidget(type_combo);
    data->addWidget(prob_lbl);
    data->addWidget(prob);

    data->addStretch();
    layout->addLayout(data);
    connect(type_combo, SIGNAL(activated(int)), SLOT(typeChanged(int)));
    t1 = new QWidget;
    lin_layout = new QHBoxLayout;
    t1->setLayout(lin_layout);
    x_x = new QLineEdit;
    x_y = new QLineEdit;
    z = new QLineEdit;
    y_x = new QLineEdit;
    y_y = new QLineEdit;
    x_x->setMaximumWidth(w * 5);
    x_y->setMaximumWidth(w * 5);
    z->setMaximumWidth(w * 3);
    y_x->setMaximumWidth(w * 5);
    y_y->setMaximumWidth(w * 5);
    pl = new QLabel("f(X) =");
    ml = new QLabel("X +");
    mat = new QGridLayout;
    mat->addWidget(x_x, 0, 0);
    mat->addWidget(x_y, 0, 1);
    mat->addWidget(y_x, 1, 0);
    mat->addWidget(y_y, 1, 1);
    lin_layout->addWidget(pl);
    lin_layout->addLayout(mat);
    lin_layout->addWidget(ml);
    lin_layout->addWidget(z);
    lin_layout->addStretch();


    t2 = new QWidget;
    t2->hide();
    il_layout = new QHBoxLayout;
    t2->setLayout(il_layout);
    p1 = new QLineEdit;
    p2 = new QLineEdit;
    p1->setMaximumWidth(w * 3);
    p2->setMaximumWidth(w * 6);
    p1l = new QLabel("Center:");
    p2l = new QLabel(", Radius:");
    il_layout->addWidget(p1l);
    il_layout->addWidget(p1);
    il_layout->addWidget(p2l);
    il_layout->addWidget(p2);
    il_layout->addStretch();

    layout->addWidget(t1);
    layout->addWidget(t2);
    layout->addStretch();
}

void IFSFunction::typeChanged(int val) {
    ftype = val;
    if (val == 0) {
        t1->show();
        t2->hide();
    }
    if (val == 1) {
        t2->show();
        t1->hide();
    }
}

QVector<QString> IFSFunction::getInfo() {
    QString t;
    t.setNum(ftype);
    QVector<QString> res = {t, prob->text()};
    if (ftype == 0) {
        res.append(x_x->text());
        res.append(x_y->text());
        res.append(y_x->text());
        res.append(y_y->text());
        res.append(z->text());
    }
    if (ftype == 1) {
        res.append(p1->text());
        res.append(p2->text());
    }
    return res;
}

void IFSFunction::setFunc(int t, QVector<QString> v) {
    type_combo->setCurrentIndex(t);
    typeChanged(t);
    prob->setText(v[0]);
    if (t == 0) {
        x_x->setText(v[1]);
        x_y->setText(v[2]);
        y_x->setText(v[3]);
        y_y->setText(v[4]);
        z->setText(v[5]);
    }
    if (t == 1) {
        p1->setText(v[1]);
        p2->setText(v[2]);
    }
}

IFSFunction::~IFSFunction() {};
