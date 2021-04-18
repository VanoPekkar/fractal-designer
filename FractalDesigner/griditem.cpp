#include "griditem.h"

#include <QKeyEvent>


GridItem::GridItem (const QPixmap &pixmap, QGraphicsItem *parent, int x, int y) : QGraphicsPixmapItem(pixmap, parent) {
    is_bottom = false;
    is_left = false;
    is_right = false;
    is_top = false;
    setPos(x, y);
}

void GridItem::setPos(qreal x, qreal y) {
    x_pixel = x;
    y_pixel = y;
    QGraphicsItem::setPos(x, y);
}

void GridItem::updatePixmap(const QImage &image) {
    setPixmap(QPixmap::fromImage(image));
    update();
}


