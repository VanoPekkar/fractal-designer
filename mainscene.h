#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>
#include <QPainter>
#include <QGraphicsItemGroup>
#include <QVector>

#include "griditem.h"


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
    int temp;
    //GridItem testitem;
    GridItem* pixmap1;
    QImage* pic;
    void PlotMandel(int colormap = 5, long double xw0 = -2, long double yw0 = -2, long double cs = 4, unsigned int iter = 20);
    void PlotM(long double xw0 = -2, long double yw0 = -2, unsigned int iter = 20, QGraphicsPixmapItem* pixmap = nullptr);
    QPixmap PlotMOn(long double xw0 = -2, long double yw0 = -2, unsigned int iter = 20);
    void zoomOut();
    void zoomIn();
    unsigned int iterCount();
    QRectF* test;
    QPainter* painter;
    QGraphicsItemGroup grid;
    QVector<GridItem*> grid_vector;
    QVector<QPixmap> pixmap_vector;
    void addTop(int x, int y);
    void addBottom(int x, int y);
    void addLeft(int x, int y);
    void addRight(int x, int y);
    void addTopLeft(int x, int y);
    void addBottomLeft(int x, int y);
    void addBottomRight(int x, int y);
    void addTopRight(int x, int y);
    void addGridItem(int x, int y);
    void reset();

public slots:
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void keyPressEvent(QKeyEvent * event);

};

#endif // MAINSCENE_H
