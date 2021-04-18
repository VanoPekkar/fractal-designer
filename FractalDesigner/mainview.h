#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QGraphicsView>
#include <QTime>
#include <QHBoxLayout>
#include <QWheelEvent>
#include <QScrollBar>
#include <QMainWindow>
#include "mainscene.h"

class MainView;

class JuliaWindow : public QMainWindow {

    Q_OBJECT

public:
    JuliaWindow(QWidget* parent,
                long double x_center = 0, long double y_center = 0);
    ~JuliaWindow();

private:
    MainScene* scene;
    MainView* view;
    QHBoxLayout* MainLayout;
    QWidget* main;

};

class MainView : public QGraphicsView {

    Q_OBJECT

public:
    MainView(MainScene* scene = nullptr, QWidget* parent = nullptr);
    MainScene* scene_ptr;
    bool eventFilter(QObject *obj, QEvent *event) override;
    int update_freq;

    QPoint pixmapOffset;
    QPoint lastDragPos;

    QImage bitMask;

    JuliaWindow* juliawindow = nullptr;

public slots:
    //void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void wheelEvent(QWheelEvent *event) override;
    void scrollContentsBy(int x, int y) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
//    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void render();
    void render_extra();
};

#endif // MAINVIEW_H
