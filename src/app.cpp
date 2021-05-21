#include "app.h"
#include "QDebug"

App::App(int& argc, char**argv) : QApplication(argc, argv) {};

bool App::notify(QObject *receiver, QEvent *event) {
    //qDebug() << event->type();
    return QApplication::notify(receiver, event);
}
