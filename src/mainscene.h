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
#include <QStringList>

#include "griditem.h"
#include "funcenter.h"


#include "mainthread.h"
#include <QThread>
#include <QDebug>

#include<QImage>
#include<QPixmap>
#include<QColor>
#include <QPainter>
#include <Qt>
#include <QSize>
#include <QTime>
#include <QRandomGenerator>

#include <complex>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_map>

struct Palette {
    uint colors[10];
    float intervals[10];
    int count;
};

class MainScene : public QGraphicsScene {

    Q_OBJECT

public:
    MainScene(QObject *parent = nullptr,
              long double x_coord = 0, long double y_coord = 0,
              long double scale = 0.005,
              long double scale_factor = 1.25,
              const QString formula = "z^2+c");
    MainScene(const MainScene* ptr);  // copy constructor
    QObject* pparentWidget;
    QPointF previousPoint;
    long double x_coord;
    long double y_coord;
    long double width;
    long double scale;
    long double scale_factor;
    int x_picsize;
    int y_picsize;
    // Function enter line
    FuncEnterLineEdit funcEnter;
    FuncEnterLineEdit* derivativeEnter;
    //GridItem testitem;
    GridItem* pixmap1;
    QImage image;
    QRectF visible_rect;
    QPainter painter;
    QVector<QVector<QVector<type>>> values;
    QVector<QVector<type>> values_new;
    QVector<QPixmap> pixmap_vector;
    MainThread* thread;
    void addGridItem(int x, int y);
    void reset();
    void setValueMatrix(const QVector<QVector<type>>& res);
    void updateImage();
    void clearScene();
    void createImage();
    void drawImage();
    QGraphicsItemGroup group;
    QVector<Palette> colormaps;
    size_t current_colormap;
    QPoint last_offset;
    QVector<QPoint> offsets;
    double palette_offset;
    long double palette_length;
    uint set_color;
    uint color(double mu);
    void connectAll();
    int method;
    bool empty = true;
    void drawLSys(QMap<QChar, QString> rules, QString axiom, QMap<QChar, QString> actions, double ang, int it);
    void drawIFS(QVector<QVector<double>>  functions, int it, QPointF start);
signals:
    void started(int max);
    void nextLine(int val);
    void format(QString f);

public slots:
    void keyPressEvent(QKeyEvent * event) override;

};

#endif // MAINSCENE_H
