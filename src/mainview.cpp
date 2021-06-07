#include "mainview.h"
#include <QStyleFactory>

JuliaWindow::JuliaWindow(QWidget* parent,
                         long double x_center, long double y_center,
                         JuliaWindow** ptr,
                         MainScene* scene_ptr) : QMainWindow(parent) {
    this->resize(500, 500);
    this->setStyle(QStyleFactory::create("Fusion"));

    self_ptr = ptr;

    main = new QWidget;

    scene = new MainScene(this);
    scene->colormaps = scene_ptr->colormaps;
    scene->current_colormap = scene_ptr->current_colormap;
    scene->palette_offset = scene_ptr->palette_offset;
    scene->palette_length = scene_ptr->palette_length;
    scene->set_color = scene_ptr->set_color;
    scene->method = scene_ptr->method;
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
    scene->method = 0;
    show();
    setAttribute(Qt::WA_DeleteOnClose);  // calls destructor when x pressed
}

JuliaWindow::~JuliaWindow() {
    delete scene->thread;
    *self_ptr = nullptr;
}


MainView::MainView(MainScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {
    installEventFilter(this);
    scene_ptr = scene;
    use_thread = true;
    setDragMode(QGraphicsView::ScrollHandDrag);
    viewport()->setCursor(Qt::CrossCursor);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    installEventFilter(this);
    update_freq = 0;
    resize_timer = 0;
    show();
    QSize visibleSize(scene_ptr->visible_rect.width(), scene_ptr->visible_rect.height());
    bitMask = QImage(visibleSize, QImage::Format_MonoLSB);
    bitMask.setColor(0, 0);
    bitMask.setColor(1, 1);
    bitMask.fill(1);
    setSceneRect({-100000, -100000, 200000, 200000});
}

void MainView::scrollContentsBy(int x, int y) {
    QGraphicsView::scrollContentsBy(x, y);
}

void MainView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastDragPos = event->pos();
    } else if (event->button() == Qt::RightButton &&
               scene_ptr->thread->fractal_type == Fractals::Mandelbrot) {
        long double half_y = scene_ptr->visible_rect.height() / 2;
        long double half_x = scene_ptr->visible_rect.width() / 2;
        long double y = scene_ptr->y_coord + ((event->y() - half_y) * scene_ptr->scale);
        long double x = scene_ptr->x_coord + ((event->x() - half_x) * scene_ptr->scale);
        if (juliawindow) {
            delete juliawindow;
        }
        juliawindow = new JuliaWindow(this, x, -y, &juliawindow, scene_ptr);
    }
    QGraphicsView::mousePressEvent(event);
}

void MainView::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton)) {
        pixmapOffset += event->pos() - lastDragPos;
        lastDragPos = event->pos();
    }
    if (mouse_position != nullptr) {
        char str[40];
        double half_y = scene_ptr->visible_rect.height() / 2;
        double half_x = scene_ptr->visible_rect.width() / 2;
        double y = scene_ptr->y_coord + ((event->y() - half_y) * scene_ptr->scale);
        double x = scene_ptr->x_coord + ((event->x() - half_x) * scene_ptr->scale);
        sprintf(str, "Real: %.4lf  Imag: %.4lf", x, -y);
        mouse_position->setText(QString::fromStdString(
                                    std::string(str)
                                    ));
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MainView::mouseReleaseEvent(QMouseEvent *event) {
    if ((event->button() == Qt::LeftButton)) {
        pixmapOffset += event->pos() - lastDragPos;
        lastDragPos = QPoint();


        scene_ptr->x_coord -= pixmapOffset.x() * scene_ptr->scale;
        scene_ptr->y_coord -= pixmapOffset.y() * scene_ptr->scale;
        scene_ptr->last_offset += pixmapOffset;
        pixmapOffset = QPoint();
        render_extra();
    }
    QGraphicsView::mouseReleaseEvent(event);
    viewport()->setCursor(Qt::CrossCursor);
}

void MainView::wheelEvent(QWheelEvent * event) {
    if (!use_thread) {
        scene_ptr->visible_rect = QRectF(mapToScene(0,0), mapToScene(width(), height()));
        if (event->angleDelta().y() > 0) {
            scale(1.3, 1.3);
        } else {
            scale(0.7, 0.7);
        }
        QGraphicsView::wheelEvent(event);
        return;
    }
    scene_ptr->empty = true;
    scene_ptr->visible_rect = QRectF(mapToScene(0,0), mapToScene(width(), height()));
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
    scene_ptr->visible_rect = QRectF(mapToScene(0,0), mapToScene(width(), height()));
    QSize visibleSize(scene_ptr->visible_rect.width(), scene_ptr->visible_rect.height());
    bitMask = QImage(visibleSize, QImage::Format_MonoLSB);
    bitMask.setColor(0, 0);
    bitMask.setColor(1, 1);
    bitMask.fill(1);
    render();
}

void MainView::render() {
    scene_ptr->visible_rect = QRectF(mapToScene(0,0), mapToScene(width(), height()));
    QSize visible_size(scene_ptr->visible_rect.width(), scene_ptr->visible_rect.height());
    //QImage tempMask = bitMask;
    if (bitMask.size() != visible_size || scene_ptr->empty) {
        //qDebug() << "here";
        bitMask = QImage(visible_size, QImage::Format_MonoLSB);
        bitMask.setColor(0, 0);
        bitMask.setColor(1, 1);
        bitMask.fill(1);
    }
    scene_ptr->thread->render(scene_ptr->x_coord, scene_ptr->y_coord, scene_ptr->scale, visible_size, bitMask ,1);
}

bool MainView::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::GraphicsSceneMouseDoubleClick) {
        return true;
    } else {
        // call standard signal
        return QObject::eventFilter(obj, event);
    }
}

void MainView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    if (use_thread) {
        diff += event->size() - event->oldSize();
        if (resize_timer) {
                killTimer(resize_timer);
                resize_timer = 0;
         }
         resize_timer = startTimer(100);
    } else {
        int old_x = scene_ptr->visible_rect.x(), old_y = scene_ptr->visible_rect.y();
        scene_ptr->visible_rect = QRectF(mapToScene(0,0), mapToScene(width(), height()));
        int x = scene_ptr->visible_rect.x(), y = scene_ptr->visible_rect.y();
        scene_ptr->last_offset = {old_x - x, old_y - y};
        diff = {0, 0};
    }
}

void MainView::timerEvent(QTimerEvent *te) {
    int old_x = scene_ptr->visible_rect.x(), old_y = scene_ptr->visible_rect.y();
    scene_ptr->visible_rect = QRectF(mapToScene(0,0), mapToScene(width(), height()));
    int x = scene_ptr->visible_rect.x(), y = scene_ptr->visible_rect.y();
    scene_ptr->last_offset = {old_x - x, old_y - y};
    render_extra();
    diff = {0, 0};
    killTimer(te->timerId());
    resize_timer = 0;
}

void MainView::render_extra() {
    scene_ptr->visible_rect = QRectF(mapToScene(0,0), mapToScene(width(), height()));
    if (use_thread) {
        QRectF source = scene_ptr->visible_rect;
        QRectF target(0, 0, source.width(), source.height());
        QImage image = QImage(target.width(), target.height(), QImage::Format_ARGB32);
        image.fill(QColor(Qt::transparent));
        QPainter painter;
        painter.begin(&image);
        scene_ptr->render(&painter, target, source);
        painter.end();
        bitMask = image.createAlphaMask();
        //qDebug() << "al" << bitMask.pixel(0, 0);
        render();
        bitMask.setColor(0, 0);
        bitMask.setColor(1, 1);
        bitMask.fill(0);
    }
}

void MainView::init() {
    use_thread = true;
    resetTransform();
    scene_ptr->visible_rect = QRectF(mapToScene(0,0), mapToScene(width(), height()));
    QSize visibleSize(scene_ptr->visible_rect.width(), scene_ptr->visible_rect.height());
    bitMask = QImage(visibleSize, QImage::Format_MonoLSB);
    bitMask.setColor(0, 0);
    bitMask.setColor(1, 1);
    bitMask.fill(1);
}

void MainView::mouseDoubleClickEvent(QMouseEvent *event) {
    mousePressEvent(event);
    return;
}



