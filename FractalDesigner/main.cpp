#include"mainwindow.h"

#include <QApplication>
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

    QMenu* pmnuSubMenu = new QMenu("&SubMenu", pmnu);
    pmnu->addMenu(pmnuSubMenu);
    pmnuSubMenu->addAction("&Test");

    QAction* pDisabledAction = pmnu->addAction("&DisabledItem");
    pDisabledAction->setEnabled(false);

    pmnu->addSeparator();

    pmnu->addAction("&Exit", &app, SLOT(quit()));

    QMenu*   fileMenu   = new QMenu("&File");
    fileMenu->addAction("&Save",
                        window,
                        SLOT(SaveImage()), // not implemented
                        Qt::CTRL + Qt::Key_S
                       );

    QMenu*   chooseMenu   = new QMenu("&Tools");

    window->menuBar()->addMenu(fileMenu);
    window->menuBar()->addMenu(chooseMenu);
    window->menuBar()->addMenu(pmnu);

    window->show();

    return app.exec();
}
