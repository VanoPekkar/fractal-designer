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
#include "funcenter.h"


#include "renderthread.h"
#include <QThread>
#include <QDebug>

typedef unsigned (*Colormap)(int);

class MainScene : public QGraphicsScene {

    Q_OBJECT

public:
    MainScene(QObject *parent = nullptr,
              long double x_coord = 0, long double y_coord = 0,
              long double scale = 0.005,
              long double scale_factor = 1.25,
              const QString formula = "z^2+c");
    QObject* pparentWidget;
    QPointF previousPoint;
    long double x_coord;
    long double y_coord;
    long double width;
    enum { ColormapSize = 256 };
    uint colormap[ColormapSize];
    long double scale;
    long double scale_factor;
    int x_picsize;
    int y_picsize;
    int temp;
    // Function enter line
    FuncEnterLineEdit funcEnter;
    FuncEnterLineEdit derivativeEnter;
    //GridItem testitem;
    GridItem* pixmap1;
    QImage image;
    void zoomOut();
    void zoomIn();
    unsigned int iterCount();
    QRectF visible_rect;
    QPainter painter;
    QVector<QVector<QVector<type>>> values;
    QVector<QVector<type>> values_new;
    QVector<QPixmap> pixmap_vector;
    RenderThread* thread;
    void addGridItem(int x, int y);
    void reset();
    void setValueMatrix(const QVector<QVector<type>>& res);
    void updateColormap();
    void createImage();
    void drawImage();
    QGraphicsItemGroup group;
    QVector<Colormap> colormaps;
    size_t current_colormap;
    QPoint last_offset;
    QVector<QPoint> offsets;


public slots:
    //void wheelEvent(QGraphicsSceneWheelEvent *event);
    void keyPressEvent(QKeyEvent * event);

};

#endif // MAINSCENE_H
