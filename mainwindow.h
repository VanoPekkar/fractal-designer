#ifndef MYWINDOW_H
#define MYWINDOW_H

#include<QDialog>
#include<QLabel>
#include<QLineEdit>
#include<QCheckBox>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QMainWindow>

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

private slots:  // own slots
    void OkClicked();
    void TextChanged(QString str);
    void ClearClicked();

signals:  // own signals
    void UpperCase(QString str);
    void Inverse(QString str);
    void Simple(QString str);

};

#endif // MYWINDOW_H
