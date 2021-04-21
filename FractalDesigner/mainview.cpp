#include "mainview.h"
#include <iostream>

JuliaWindow::JuliaWindow(QWidget* parent,
                         long double x_center, long double y_center) : QMainWindow(parent) {
//    parent->setEnabled(false);
    this->resize(500, 500);  // TODO: necessary?

    main = new QWidget;

    scene = new MainScene(this);
    scene->thread->fractal_type = Fractals::JuliaSet;
    std::string formula = "z^2 + " + std::to_string(x_center) +
                          " + " + std::to_string(y_center) + "i";
    scene->funcEnter.setText(formula.c_str());
    scene->funcEnter.parse_one_var();

    view = new MainView(scene);

    MainLayout = new QHBoxLayout;
    MainLayout->addWidget(view);

    main->setLayout(MainLayout);
    setCentralWidget(main);

    formula = "Julia set for " + std::to_string(x_center) + " " + std::to_string(y_center) + "i";
    setWindowTitle(formula.c_str());
    show();
    //setAttribute(Qt::WA_DeleteOnClose);  // calls destructor when x pressed
}

JuliaWindow::~JuliaWindow() {
//    parentWidget()->setEnabled(true);
}


MainView::MainView(MainScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {
    installEventFilter(this);
    scene_ptr = scene;
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setRenderHint(QPainter::HighQualityAntialiasing);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    //KeyPressEater *keyPressEater = new KeyPressEater();
    installEventFilter(this);
    update_freq = 0;

    //setCacheMode(QGraphicsView::CacheNone);
    show();
    QSize visibleSize(scene_ptr->visible_rect.width(), scene_ptr->visible_rect.height());
    bitMask = QImage(visibleSize, QImage::Format_MonoLSB);
    bitMask.fill(1);
    render();
    setSceneRect({-100000, -100000, 200000, 200000});
}

void MainView::scrollContentsBy(int x, int y) {
    QGraphicsView::scrollContentsBy(x, y);
}

void MainView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastDragPos = event->pos();
    } else if (event->button() == Qt::RightButton) {
        long double half_y = scene_ptr->visible_rect.height() / 2;
        long double half_x = scene_ptr->visible_rect.width() / 2;
        long double y = scene_ptr->y_coord + ((event->y() - half_y) * scene_ptr->scale);
        long double x = scene_ptr->y_coord + ((event->x() - half_x) * scene_ptr->scale);
        if (juliawindow) {
            delete juliawindow;
        }
        juliawindow = new JuliaWindow(this, x, y);
    }
    QGraphicsView::mousePressEvent(event);
}

void MainView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        pixmapOffset += event->pos() - lastDragPos;
        lastDragPos = event->pos();
        //update();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MainView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pixmapOffset += event->pos() - lastDragPos;
        lastDragPos = QPoint();


        scene_ptr->x_coord -= pixmapOffset.x() * scene_ptr->scale;
        scene_ptr->y_coord -= pixmapOffset.y() * scene_ptr->scale;
        scene_ptr->last_offset = pixmapOffset;
        pixmapOffset = QPoint();

        render_extra();
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void MainView::wheelEvent(QWheelEvent * event) {
    bitMask.fill(0);

    //scene_ptr->pixmap1->setScale(scene_ptr->scale * 10);
    //qDebug() << scene_ptr->items().count();


    QRectF source = scene_ptr->visible_rect;
    QRectF target(0, 0, source.width(), source.height());
    QImage image = QImage(target.width(), target.height(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QSize scaled_size(source.width(), source.height());
    QPoint add;
    if (event->angleDelta().y() > 0) {
        scene_ptr->scale /= scene_ptr->scale_factor;
        scaled_size *= scene_ptr->scale_factor;
    } else {
        scene_ptr->scale *= scene_ptr->scale_factor;
        scaled_size /= scene_ptr->scale_factor;
        add.setX(source.width() * (1 - 1 / scene_ptr->scale_factor) / 2);
        add.setY(source.height() * (1 - 1 / scene_ptr->scale_factor) / 2);
    }

    QPainter painter;
    painter.begin(&image);
    scene_ptr->render(&painter, target, source);
    painter.end();



//    QVector<QRgb> ctable;
//    for(int i = 0; i < 256; ++i) {
//        ctable.append(qRgb(i, i, i));
//    }
    //image.setColorCount(256);
    //image.setColorTable(ctable);s
    image = image.scaled(scaled_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if (event->angleDelta().y() > 0) {
        QSize c = image.size() / scene_ptr->scale_factor;
        int dx = (image.width() - c.width()) / 2;
        int dy = (image.height() - c.height()) / 2;
        QRect cropped(dx, dy, c.width(), c.height());
        image = image.copy(cropped);
        add = {0, 0};
    }
    auto* newi = scene_ptr->addPixmap(QPixmap::fromImage(image));

    newi->setPos(source.topLeft() + add);

    for (auto* x : scene_ptr->items()) {
        if (x != newi) {
            scene_ptr->removeItem(x);
        }
    }
    scene_ptr->values.clear();



    //QGraphicsItemGroup *group = scene_ptr->createItemGroup(scene_ptr->items());

    render();
    //scene_ptr->destroyItemGroup(group);
    //QGraphicsScene::wheelEvent(event);
}

void MainView::render() {
    QSize visible_size(scene_ptr->visible_rect.width(), scene_ptr->visible_rect.height());
    QImage tempMask = bitMask;
    scene_ptr->thread->render(scene_ptr->x_coord, scene_ptr->y_coord, scene_ptr->scale, visible_size, tempMask ,1);
}

bool MainView::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::Wheel) {
        //QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        //QImage img(700, 700, QImage::Format_RGB32);
        //img.fill(qRgb(0,23,100));
        //scene()->addPixmap(QPixmap::fromImage(img));
        return true;
    } else {
        // вызов стандартного сигнала
        return QObject::eventFilter(obj, event);
    }
}

void MainView::resizeEvent(QResizeEvent *event) {
    qDebug() << "resize" << scene_ptr->visible_rect;
    QGraphicsView::resizeEvent(event);
    render_extra();
}

void MainView::render_extra() {
    scene_ptr->visible_rect = QRectF(mapToScene(0,0), mapToScene(width(), height()));
    QRectF source = scene_ptr->visible_rect;
    QRectF target(0, 0, source.width(), source.height());
    QImage image = QImage(target.width(), target.height(), QImage::Format_ARGB32);
    image.fill(QColor(Qt::transparent));
    QPainter painter;
    painter.begin(&image);
    scene_ptr->render(&painter, target, source);
    painter.end();
    bitMask = image.createAlphaMask();
    render();
    bitMask.fill(0);
}

//void MainView::mouseDoubleClickEvent(QMouseEvent *event) {
//    // show Julia Window
//    long double half_y = scene_ptr->visible_rect.height() / 2;
//    long double half_x = scene_ptr->visible_rect.width() / 2;
//    long double y = scene_ptr->y_coord + ((event->y() - half_y) * scene_ptr->scale);
//    long double x = scene_ptr->y_coord + ((event->x() - half_x) * scene_ptr->scale);
//    juliawindow = new JuliaWindow(this, x, y);
//    QGraphicsView::mouseDoubleClickEvent(event);
//}



