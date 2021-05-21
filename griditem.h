#ifndef GRIDITEM_H
#define GRIDITEM_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QDebug>

class GridItem : public QGraphicsPixmapItem {

public:
    GridItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr, int x = 0, int y = 0);
    int x_pixel;
    int y_pixel;
    long double x_coord;
    long double y_coord;
    void setPos(qreal x, qreal y);
    QGraphicsScene* scene;

public slots:
    void updatePixmap(const QImage &image);

};

#endif // GRIDITEM_H
