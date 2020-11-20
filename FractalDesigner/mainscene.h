#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>


class MainScene : public QGraphicsScene {

    Q_OBJECT

public:
    MainScene(QObject *parent = nullptr);
    QObject* pparentWidget;
    QPointF     previousPoint;
    long double x_coord;
    long double y_coord;
    long double width;
    long double colormap;
    long double scale;
    int x_picsize;
    int y_picsize;
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    QImage PlotMandel(int colormap = 5, long double xw0 = -2, long double yw0 = -2, long double cs = 4, unsigned int iter = 20);

};

#endif // MAINSCENE_H
