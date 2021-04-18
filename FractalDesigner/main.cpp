#include"mainwindow.h"

#include<QApplication>
#include <QtGui>
#include <QMenu>
#include <QMenuBar>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow* window = new MainWindow();

    QMenu*   pmnu   = new QMenu("&Menu");

    pmnu->addAction("&About Qt",
                    &app,
                    SLOT(aboutQt()),
                    Qt::CTRL + Qt::Key_Q
                   );

    pmnu->addSeparator();

    QAction* pCheckableAction = pmnu->addAction("&CheckableItem");
    pCheckableAction->setCheckable(true);
    pCheckableAction->setChecked(true);

    QMenu* fractalMenu = new QMenu("Fractals", pmnu);
    pmnu->addMenu(fractalMenu);
    fractalMenu->addAction("Mandelbrot",
                           window,
                           SLOT(ChangeToMandelbrot()));
    fractalMenu->addAction("Julia set",
                           window,
                           SLOT(ChangeToJuliaSet()));
    fractalMenu->addAction("Newton",
                           window,
                           SLOT(ChangeToNewton()));

    QAction* pDisabledAction = pmnu->addAction("&DisabledItem");
    pDisabledAction->setEnabled(false);

    pmnu->addSeparator();

    pmnu->addAction("&Exit", &app, SLOT(quit()));

    QMenu*   fileMenu   = new QMenu("&File");
    fileMenu->addAction("&Save",
                              &app,
                              SLOT(aboutQt()), // TODO: not implemented
                              Qt::CTRL + Qt::Key_S
                             );

    QMenu*   chooseMenu   = new QMenu("&Tools");

    window->menuBar()->addMenu(fileMenu);
    window->menuBar()->addMenu(chooseMenu);
    window->menuBar()->addMenu(pmnu);

    window->show();

    return app.exec();
}
