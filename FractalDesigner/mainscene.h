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


private:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

};

#endif // MAINSCENE_H
