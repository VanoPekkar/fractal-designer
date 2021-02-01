#ifndef SAVEWINDOW_H
#define SAVEWINDOW_H

#include<QWidget>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QLineEdit>
#include<QComboBox>
#include<QLabel>
#include<QPushButton>
#include<QPainter>
#include<QCloseEvent>

#include"mainscene.h"

class SaveWindow : public QWidget {

    Q_OBJECT

public:
    SaveWindow(QWidget* parent, MainScene* scene);

private:
    QWidget* _parent;
    MainScene* _scene;
    QHBoxLayout* nameLayout;
    QHBoxLayout* formatLayout;
    QHBoxLayout* buttonLayout;
    QVBoxLayout* mainLayout;
    QLineEdit* name;
    QComboBox* box;
    QLabel* nameLabel;
    QLabel* formatLabel;
    QPushButton* save;
    QPushButton* cancel;

private slots:
    void Save();
    void Destroy();

protected:
    void closeEvent(QCloseEvent* event) override;

};

#endif // SAVEWINDOW_H
