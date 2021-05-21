#ifndef IFSFUNC_H
#define IFSFUNC_H


#include <QGroupBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QVector>
#include <QDebug>
#include <QSizePolicy>

class IFSFunction : public QGroupBox {

    Q_OBJECT

public:
    IFSFunction(QString name);
    ~IFSFunction();
    QComboBox* type_combo;
    QLabel* prob_lbl;
    QLineEdit* prob;
    QVBoxLayout* layout;
    QHBoxLayout* data;
    QWidget* t1;
    QWidget* t2;

    QLineEdit* x_x;
    QLineEdit* x_y;
    QLineEdit* z;
    QLineEdit* y_x;
    QLineEdit* y_y;
    QHBoxLayout* lin_layout;
    QGridLayout* mat;
    QLabel* pl;
    QLabel* ml;

    QHBoxLayout* il_layout;
    QLabel* p1l;
    QLabel* p2l;
    QLineEdit* p1;
    QLineEdit* p2;

    int ftype;


public slots:
    void typeChanged(int val);
    QVector<QString> getInfo();
    void setFunc(int t, QVector<QString>);

};

#endif // IFSFUNC_H
