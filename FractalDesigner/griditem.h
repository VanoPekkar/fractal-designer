#ifndef GRIDITEM_H
#define GRIDITEM_H

#include <QGraphicsPixmapItem>


class GridItem : public QGraphicsPixmapItem {

    //Q_OBJECT

public:
    GridItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr, int x = 0, int y = 0);
    int x_pixel;
    int y_pixel;
    long double x_coord;
    long double y_coord;
    bool is_top;
    bool is_bottom;
    bool is_left;
    bool is_right;
    void setPos(qreal x, qreal y);
};

#endif // GRIDITEM_H
