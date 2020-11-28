#include "mainview.h"

MainView::MainView(MainScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {
    installEventFilter(this);
    scene_ptr = scene;
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //KeyPressEater *keyPressEater = new KeyPressEater();
    installEventFilter(this);
    //setCacheMode(QGraphicsView::CacheNone);
    show();
}

void MainView::wheelEvent(QWheelEvent *event) {
//    auto scene = scene_ptr;
//    scene->PlotMandel(scene->colormap, -1, 0, scene->width, 50);
//    viewport()->update();
//    scene->PlotMandel(scene->colormap, -1.5, -1, scene->width / 2, 50);
//    viewport()->update();
    //scene->update();
    //scene->PlotMandel(scene->colormap, -1.2, 0.1, scene->width, 10);
    //viewport()->update();
    //scene->update();
    //scene->PlotMandel(scene->colormap, 0, -0.2, scene->width, 15);
    //viewport()->update();

    //scene->update();
    //scale(2, 2);
    //scene_ptr->clear();
    //viewport()->update();
//    if (event->angleDelta().y() > 0) {
//        scale(4, 4);
//    } else {
//        scale(0.25, 0.25);
//    }
    QGraphicsView::wheelEvent(event);
//    QImage img(700, 700, QImage::Format_RGB32);
//    img.fill(qRgb(0,23,100));
//    scene()->addPixmap(QPixmap::fromImage(img));
}

void MainView::scrollContentsBy(int x, int y)
{
//    if ((float)verticalScrollBar()->value() / verticalScrollBar()->maximum() < 0.2) {
//        scene()->addRect(0, -800, 800, 800);
//    }
//    if ((float)verticalScrollBar()->value() / verticalScrollBar()->maximum() > 0.8) {
//        scene()->addRect(0, 1600, 800, 800);
//    }
//    if ((float)horizontalScrollBar()->value() / horizontalScrollBar()->maximum() < 0.2) {
//        scene()->addRect(0, 1600, 800, 800);
//    }
//    if ((float)horizontalScrollBar()->value() / horizontalScrollBar()->maximum() > 0.8) {
//        scene()->addRect(0, 1600, 800, 800);
//    }
    for (auto elem : scene()->items( QRectF(mapToScene(0,0), mapToScene(width(), height())))) {
        GridItem* grid_elem = static_cast<GridItem*>(elem);
        if (grid_elem->is_top && grid_elem->is_left) {
            grid_elem->is_top = false;
            grid_elem->is_left = false;
            scene_ptr->addTop(grid_elem->x_pixel, grid_elem->y_pixel);
            scene_ptr->addLeft(grid_elem->x_pixel, grid_elem->y_pixel);
            scene_ptr->addTopLeft(grid_elem->x_pixel, grid_elem->y_pixel);
        } else if (grid_elem->is_top && grid_elem->is_right) {
            grid_elem->is_top = false;
            grid_elem->is_right = false;
            scene_ptr->addTop(grid_elem->x_pixel, grid_elem->y_pixel);
            scene_ptr->addRight(grid_elem->x_pixel, grid_elem->y_pixel);
            scene_ptr->addTopRight(grid_elem->x_pixel, grid_elem->y_pixel);
        } else if (grid_elem->is_bottom && grid_elem->is_left) {
            grid_elem->is_bottom = false;
            grid_elem->is_left = false;
            scene_ptr->addBottom(grid_elem->x_pixel, grid_elem->y_pixel);
            scene_ptr->addLeft(grid_elem->x_pixel, grid_elem->y_pixel);
            scene_ptr->addBottomLeft(grid_elem->x_pixel, grid_elem->y_pixel);
        } else if (grid_elem->is_bottom && grid_elem->is_right) {
            grid_elem->is_bottom = false;
            grid_elem->is_right = false;
            scene_ptr->addBottom(grid_elem->x_pixel, grid_elem->y_pixel);
            scene_ptr->addRight(grid_elem->x_pixel, grid_elem->y_pixel);
            scene_ptr->addBottomRight(grid_elem->x_pixel, grid_elem->y_pixel);
        } else if (grid_elem->is_top) {
            grid_elem->is_top = false;
            scene_ptr->addTop(grid_elem->x_pixel, grid_elem->y_pixel);
        } else if (grid_elem->is_right) {
            grid_elem->is_right = false;
            scene_ptr->addRight(grid_elem->x_pixel, grid_elem->y_pixel);
        } else if (grid_elem->is_left) {
            grid_elem->is_left = false;
            scene_ptr->addLeft(grid_elem->x_pixel, grid_elem->y_pixel);
        } else if (grid_elem->is_bottom) {
            grid_elem->is_bottom = false;
            scene_ptr->addBottom(grid_elem->x_pixel, grid_elem->y_pixel);
        }
    }
    QGraphicsView::scrollContentsBy(x, y);
}

bool MainView::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::Wheel) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //qDebug()<<"Прокрутка";
        //QImage img(700, 700, QImage::Format_RGB32);
        //img.fill(qRgb(0,23,100));
        //scene()->addPixmap(QPixmap::fromImage(img));
        return true;
    } else {
        // вызов стандартного сигнала
        return QObject::eventFilter(obj, event);
    }
}
