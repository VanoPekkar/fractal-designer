#ifndef SAVEWINDOW_H
#define SAVEWINDOW_H

#include<QMainWindow>
#include<QHBoxLayout>
#include<QFileDialog>
#include<QLabel>
#include<QVBoxLayout>
#include<QPushButton>
#include<QComboBox>
#include<QProgressBar>
#include<iostream>
#include<QGraphicsView>

#include "mainscene.h"

class SaveWindow : public QMainWindow {

    Q_OBJECT

public:
    SaveWindow(QWidget* parent = nullptr,
               const MainScene* scene_ptr = nullptr,
               QMap<QChar, QString>* rules_map = nullptr,
               QString* axiom_str = nullptr,
               QMap<QChar, QString>* actions_map = nullptr,
               double angle = 0,
               int iterations = 100,
               QVector<QVector<double>>* func_table = nullptr,
               QPointF start_point = {0, 0});
    ~SaveWindow();
    QWidget* parentW;
    QWidget* main;
    QVBoxLayout* main_layout;

    QProgressBar* progress_bar;

    QHBoxLayout* filename_layout;
    QHBoxLayout* resolution_layout;

    QLabel* filename_lbl;
    QPushButton* file_chooser;

    QString filename;

    QComboBox* resolution_chooser;

    QHBoxLayout* save_layout;
    QPushButton* save_file_btn;

    MainScene* scene;
    const MainScene* main_scene;

    QMap<QChar, QString>* rules_map;
    QString* axiom_str;
    QMap<QChar, QString>* actions_map;
    double angle;
    int iterations;
    QVector<QVector<double>>* func_table;
    QPointF start_point;


private slots:
    void SaveClicked();
    void FileChooserClicked();
    void ImageCreated();


};
#endif // SAVEWINDOW_H
