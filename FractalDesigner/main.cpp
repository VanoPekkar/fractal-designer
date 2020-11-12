//  read about static build !!!!!!!!!!!!!!!


#include<QApplication>
#include"mainwindow.h"
//#include<QPushButton>
//#include<QSlider>
//#include<QSpinBox>
//#include<QVBoxLayout>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow* window = new MainWindow();
    window->show();


    /*QPushButton* btn = new QPushButton("Bye!");
    btn->show();

    QSlider* sl = new QSlider(Qt::Horizontal);
    sl->setMaximum(100);

    QWidget* window = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;

    QSpinBox* sb = new QSpinBox;
    sb->setMaximum(100);

    layout->addWidget(sl);
    layout->addWidget(btn);
    layout->addWidget(sb);

    window->setLayout(layout);

    window->show();

    QObject::connect(btn, SIGNAL(clicked()), &app, SLOT(quit()));
    QObject::connect(sb, SIGNAL(valueChanged(int)), sl, SLOT(setValue(int)));
    QObject::connect(sl, SIGNAL(valueChanged(int)), sb, SLOT(setValue(int)));*/

    return app.exec();
}
