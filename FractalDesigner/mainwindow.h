#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "mainscene.h"
#include "funcenter.h"

#include "savewindow.h"

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
    QLabel* pic;
    QLineEdit* line;
    QCheckBox* cb1;
    QCheckBox* cb2;
    QPushButton* ok;
    QPushButton* close;
    QPushButton* clear;
    QGraphicsView* view;
    QLabel* funcLabel;
    FuncEnterLineEdit* funcLineEdit;
    SaveWindow* saveWindow;
    MainScene* scene;

private slots:  // own slots
    void OkClicked();
    void TextChanged(QString str);
    void ClearClicked();
    void SaveImage();

signals:  // own signals
    void UpperCase(QString str);
    void Inverse(QString str);
    void Simple(QString str);

};

#endif // MYWINDOW_H
