#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QGraphicsView>
#include <QTimer>
#include <QWheelEvent>
#include <QScrollBar>

#include "mainscene.h"


class MainView : public QGraphicsView {

    Q_OBJECT

public:
    MainView(MainScene* scene = nullptr, QWidget* parent = nullptr);
    MainScene* scene_ptr;
    bool eventFilter(QObject *obj, QEvent *event);
    QRectF* test;

protected slots:
    //void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void wheelEvent(QWheelEvent *event);
    void scrollContentsBy(int x, int y);
};

#endif // MAINVIEW_H
