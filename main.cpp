#include"mainwindow.h"
#include "app.h"

#include<QApplication>
#include <QtGui>
#include <QMenu>
#include <QMenuBar>
#include <QScreen>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow* window = new MainWindow();

    QMenu* pmnu = new QMenu("&Menu");

    pmnu->addSeparator();

    QMenu* fractalMenu = new QMenu("Fractals", pmnu);

    QMenu* it_menu = new QMenu("Coloring method", pmnu);
    QActionGroup* it_group = new QActionGroup(it_menu);
    it_group->setExclusive(true);
    QAction* iter_log = it_menu->addAction("Logarithmic", window, SLOT(setLog()));
    iter_log->setCheckable(true);
    QAction* iter_pure = it_menu->addAction("Pure iterations", window, SLOT(setPure()));
    iter_pure->setCheckable(true);
    it_group->addAction(iter_log);
    it_group->addAction(iter_pure);
    iter_log->setChecked(true);


    // choices of fractals
    QActionGroup* fractals_group = new QActionGroup(fractalMenu);
    fractals_group->setExclusive(true);
    QAction* mandelbrot = fractalMenu->addAction("Mandelbrot", window, SLOT(ChangeToMandelbrot()));
    QAction* julia = fractalMenu->addAction("Julia set", window, SLOT(ChangeToJuliaSet()));
    QAction* newton = fractalMenu->addAction("Newton", window, SLOT(ChangeToNewton()));
    QAction* secant = fractalMenu->addAction("Secant", window, SLOT(ChangeToSecant()));
    QAction* ifs = fractalMenu->addAction("IFS", window, SLOT(ChangeToIFS()));
    QAction* lsys = fractalMenu->addAction("L-system", window, SLOT(ChangeToLSys()));
    fractals_group->addAction(mandelbrot);
    fractals_group->addAction(julia);
    fractals_group->addAction(newton);
    fractals_group->addAction(secant);
    fractals_group->addAction(ifs);
    fractals_group->addAction(lsys);
    mandelbrot->setCheckable(true);
    mandelbrot->setChecked(true);
    julia->setCheckable(true);
    newton->setCheckable(true);
    secant->setCheckable(true);
    ifs->setCheckable(true);
    lsys->setCheckable(true);

    pmnu->addSeparator();

    QMenu* fileMenu   = new QMenu("&File");
    fileMenu->addAction("&Save",
                        window,
                        SLOT(SaveImage()),
                        Qt::CTRL + Qt::Key_S
                        );
    fileMenu->addAction("&Documentation",
                        window,
                        SLOT(OpenDocumentation()),
                        Qt::CTRL + Qt::Key_D
                        );


    window->menuBar()->addMenu(fileMenu);
    window->menuBar()->addMenu(fractalMenu);
    window->menuBar()->addMenu(it_menu);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    QSize default_size = {screenGeometry.width() * 2 / 3, screenGeometry.height() * 2 / 3};
    int pos_x = (screenGeometry.width() - default_size.width()) / 2;
    int pos_y = (screenGeometry.height() - default_size.height()) / 2;
    window->move(pos_x, pos_y);
    window->resize(default_size);
    window->show();

    return app.exec();
}
