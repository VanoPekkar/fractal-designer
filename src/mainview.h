#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QGraphicsView>
#include <QTime>
#include <QHBoxLayout>
#include <QWheelEvent>
#include <QScrollBar>
#include <QMainWindow>
#include <QLabel>
#include "mainscene.h"

class MainView;

class JuliaWindow : public QMainWindow {

    Q_OBJECT

public:
    JuliaWindow(QWidget* parent,
                long double x_center = 0, long double y_center = 0,
                JuliaWindow** ptr = nullptr,
                MainScene* scene_ptr = nullptr);
    ~JuliaWindow();

private:
    MainScene* scene;
    MainView* view;
    QHBoxLayout* MainLayout;
    QWidget* main;
    JuliaWindow** self_ptr;

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
    QSize diff;

    QImage bitMask;
    int resize_timer;
    JuliaWindow* juliawindow = nullptr;
    bool use_thread;

    QLabel* mouse_position = nullptr;

public slots:
    void wheelEvent(QWheelEvent *event) override;
    void scrollContentsBy(int x, int y) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void timerEvent(QTimerEvent *te) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void render();
    void render_extra();
    void init();
};

#endif // MAINVIEW_H
