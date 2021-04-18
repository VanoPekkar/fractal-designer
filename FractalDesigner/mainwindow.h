#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "mainview.h"
#include "mainscene.h"

#include<QDialog>
#include<QLabel>
#include<QLineEdit>
#include<QCheckBox>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QMainWindow>
#include<QMessageBox>
#include<QImage>
#include<QPixmap>
#include<QColor>
#include <QSlider>
#include <QGraphicsView>

#include <complex>
#include <cmath>

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    QWidget* main;
    QLabel* lbl;
    QLabel* lbl_funcEnter;
    QLabel* lbl_derivative;
    QLineEdit* line;
    QCheckBox* cb1;
    QCheckBox* cb2;
    QPushButton* ok;
    QPushButton* close;
    QPushButton* reset;
    MainView* view;
    MainScene* scene;
    QSlider* palette_slider;
    QHBoxLayout* layout_derivEnter;
    QVBoxLayout* right_layout;


    //void mousePressEvent(QMouseEvent * event);

private slots:  // own slots
    void OkClicked();
    void TextChanged(QString str);
//    void resetClicked();
    void sliderMoved(int val);
    void ChangeToMandelbrot();
    void ChangeToJuliaSet();
    void ChangeToNewton();

signals:  // own signals
    void UpperCase(QString str);
    void Inverse(QString str);
    void Simple(QString str);

};

#endif // MYWINDOW_H
