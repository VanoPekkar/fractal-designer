#include "mainscene.h"
#include "mainwindow.h"

#include <iostream>

MainScene::MainScene(QObject* parent,
                     long double x_coord, long double y_coord,
                     long double scale,
                     long double scale_factor,
                     const QString formula)
    : QGraphicsScene(parent)
    , x_coord(x_coord)
    , y_coord(y_coord)
    , scale(scale)
    , scale_factor(scale_factor) {
    pparentWidget = parent;
    width = 4;
    palette_length = 1;
    palette_offset = 0;
    current_colormap = 0;
    method = 1;
    colormaps = {{
                     {qRgb(0, 0, 255), qRgb(0, 255, 255), qRgb(0, 255, 0), qRgb(255, 255, 0), qRgb(255, 0, 0), qRgb(255, 0, 255), qRgb(0, 0, 255)},
                     {0, 0.125, 0.175, 0.35, 0.515, 0.85, 1},
                     7
                 },
                 {
                     {qRgb(0, 0, 0), qRgb(255, 255, 255), qRgb(0, 0, 0)},
                     {0, 0.5, 1},
                     3
                 }, {
                     {qRgb(255, 0, 0), qRgb(255, 255, 0), qRgb(255, 255, 255), qRgb(0, 0, 0), qRgb(200, 0, 200), qRgb(255, 0, 0)},
                     {0, 0.35, 0.7, 0.8, 0.9, 1},
                     5
                 }, {
                     {qRgb(255, 255, 255), qRgb(20, 0, 60), qRgb(255, 0, 0), qRgb(255, 255, 255), qRgb(20, 0, 60), qRgb(255, 255, 255)},
                     {0, 0.15, 0.35, 0.65, 0.8, 1},
                     6
                 }, {
                     {qRgb(0, 0, 0), qRgb(200, 100, 0), qRgb(0, 255, 50), qRgb(255, 155, 25), qRgb(120, 255, 80), qRgb(70, 130, 60), qRgb(0, 0, 0)},
                     {0, 0.15, 0.35, 0.5, 0.65, 0.8, 1},
                     7
                 }, {
                     {qRgb(60, 50, 200), qRgb(50, 90, 200), qRgb(180, 150, 70), qRgb(230, 220, 150), qRgb(140, 110, 200), qRgb(60, 0, 190), qRgb(60, 50, 200)},
                     {0, 0.10, 0.35, 0.5, 0.80, 0.9, 1},
                     7
                 }};
    set_color = qRgba(0, 0, 0, 255);
    funcEnter.setText(formula);
    funcEnter.parse_two_vars();
    qRegisterMetaType<QVector<QVector<type>>>("QVector<QVector<type>>");
    thread = new Mandelbrot_Julia_Thread;
    thread->fractal_type = Fractals::Mandelbrot;
    thread->fparser = &funcEnter;
    connectAll();
}

MainScene::MainScene(const MainScene* ptr)
    : pparentWidget(nullptr)
    , previousPoint(ptr->previousPoint)
    , x_coord(ptr->x_coord), y_coord(ptr->y_coord)
    , width(ptr->width)
    , scale(ptr->scale)
    , scale_factor(ptr->scale_factor)
    , x_picsize(ptr->x_picsize), y_picsize(ptr->y_picsize)
    , funcEnter(&ptr->funcEnter)
    , derivativeEnter(new FuncEnterLineEdit(ptr->derivativeEnter))
    , pixmap1(ptr->pixmap1)
    , colormaps(ptr->colormaps)
    , current_colormap(ptr->current_colormap)
    , last_offset(ptr->last_offset)
    , offsets(ptr->offsets)
    , palette_offset(ptr->palette_offset)
    , palette_length(ptr->palette_length)
    , set_color(ptr->set_color)
    , method(ptr->method)
    , thread(nullptr) {}

void MainScene::connectAll() {
    connect(thread, &MainThread::renderedImage, this, &MainScene::setValueMatrix);
    connect(thread, &MainThread::started, this, &MainScene::started);
    connect(thread, &MainThread::nextLine, this, &MainScene::nextLine);
    connect(thread, &MainThread::format, this, &MainScene::format);
}

void MainScene::keyPressEvent(QKeyEvent * event) {
    if (event->key() == Qt::Key_Q) {

    } else if (event->key() == Qt::Key_E) {
        palette_offset += 2;
    }
}

void MainScene::setValueMatrix(const QVector<QVector<type>>& res) {
    if (empty) {
        clear();
    }
    empty = false;
    int di = -last_offset.y(), dj = -last_offset.x();
    last_offset = {0, 0};
    int h = res.size(), w = res[0].size();
    if (values.size() == 0) { // || h != values.last().size() || w != values.last()[0].size()) {
        values = {res};
        offsets = {{di, dj}};
        drawImage();
        return;
    }
    if (di == 0 && dj == 0) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                type r = res[i][j];
                if (r != TRANSP) {
                    values.last()[i][j] = r;
                }
            }
        }
        drawImage();
        return;
    }
    values_new = QVector<QVector<type>>(h, QVector<type>(w));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int ni = i + di, nj = j + dj;
            type r = res[i][j];
            int s = values.size() - 1;
            while (s >= 0 && r == TRANSP) {
                if (0 <= ni && ni < values[s].size() && 0 <= nj && nj < values[s][0].size() && values[s][ni][nj] != TRANSP) {
                    r = values[s][ni][nj];
                } else {
                    ni += offsets[s].x();
                    nj += offsets[s].y();
                    --s;
                }

            }
            values_new[i][j] = r;
        }
    }
    values.push_back(values_new);
    offsets.push_back({di, dj});
    drawImage();
}


void MainScene::updateImage() {
    if (values.empty()) return;
    createImage();
    pixmap1->setPixmap(QPixmap::fromImage(image));
    for (auto* x : items()) {
        if (x != pixmap1) {
            removeItem(x);
        }
    }
    values = {values.last()};
    offsets = {offsets.last()};
}

void MainScene::clearScene() {
    for (auto* x : items()) {
          removeItem(x);
    }
    values = {};
    offsets = {{0, 0}};
}

void MainScene::createImage() {
    QSize resultSize(values.last()[0].size(), values.last().size());
    image = QImage(resultSize, QImage::Format_ARGB32);
    uint* scanLine = reinterpret_cast<uint *>(image.scanLine(0));
    uint transparent = qRgba(0, 0, 0, 0);
    float l = palette_length, o = palette_offset;
    double add, t;
    unsigned int c1, c2, op = 0xff000000;
    uint* colors = colormaps[current_colormap].colors;
    float* intervals = colormaps[current_colormap].intervals;
    int count = colormaps[current_colormap].count;
    uint r, g, b, k;
    for (auto a : values.last()) {
        for (type mu : a) {
            if (mu == TRANSP) {
                *scanLine++ = transparent;
                continue;
            }
            if (mu == 0 || mu != mu) {
                *scanLine++ = set_color;
                continue;
            }
            if (method == 0) {
                if (mu > 0) mu = log10(mu + 1);
                else mu = -log10(-mu + 1);
            }
            add =  mu / l + o;
            add -= floor(add);
            k = 0;
            for (int i = 0; i < count; ++i) {
                if ((intervals[i] < add) && (add <= intervals[i + 1])) {
                    c1 = colors[i];
                    c2 = colors[i + 1];
                    t = (add - intervals[i]) / (intervals[i + 1] - intervals[i]);
                    r = (((c1 & 0xff0000) >> 16) * (1 - t) + ((c2 & 0xff0000) >> 16) * (t));
                    g = ((c1 & 0xff00) >> 8) * (1 - t) + ((c2 & 0xff00) >> 8) * (t);
                    b = (c1 & 0xff) * (1 - t) + (c2 & 0xff) * (t);
                    *scanLine++ = op + (r << 16) + (g << 8) + b;
                    k = 1;
                    break;
                }
                if (k) break;
            }
        }
    }
}

void MainScene::drawImage() {
    createImage();
    pixmap1 = new GridItem(QPixmap::fromImage(image), nullptr, visible_rect.x(), visible_rect.y());
    addItem(pixmap1);
}

void MainScene::reset() {
    x_coord = 0;
    y_coord = 0;
    scale = 0.005;
}

void MainScene::drawLSys(QMap<QChar, QString> rules, QString axiom, QMap<QChar, QString> actions, double ang, int it) {
    QString axm_tmp = "";
    int r = 50;
    double angle = ang / 180 * M_PI;
    double cur_angle = 0;
    QPainterPath path;
    for (int i = 0; i < it; ++i) {
        for (QChar c : axiom) {
            auto f = rules.find(c.cell());
            if (f == rules.end()) {
                rules[c.cell()] = c.cell();
            }
            axm_tmp += rules[c.cell()];
        }
        axiom = axm_tmp;
        axm_tmp = "";
    }
    QStack<QPointF> positions;
    QStack<double> angles;
    for (QChar x : axiom) {
        if (actions.find(x) == actions.end()) {
            actions[x] = x;
        }
    }
    for (QChar x : axiom) {
        for (QChar c : actions[x]) {
            if (c == '+') {
                cur_angle += angle;
            }
            if (c == '-') {
                cur_angle -= angle;
            }
            if (c == 'F') {
                QPointF delta(r * cos(cur_angle), r * sin(cur_angle));
                path.lineTo(path.currentPosition() + delta);
            }
            if (c == 'f') {
                QPointF delta(r * cos(cur_angle), r * sin(cur_angle));
                path.moveTo(path.currentPosition() + delta);
            }
            if (c == '[') {
                positions.push(path.currentPosition());
                angles.push(cur_angle);
            }
            if (c == ']') {
                path.moveTo(positions.pop());
                cur_angle = angles.pop();
            }
        }
    }
    addPath(path, QPen(QColor(255, 60, 0), 7, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
}

void MainScene::drawIFS(QVector<QVector<double>> functions, int it, QPointF start) {
    QColor col = QColor(0, 0, 0);
    double p, x, y, ps = 0;
    int n = functions.size();
    QVector<double> probs = {0};
    for (int i = 0; i < n - 1; ++i) {
        ps += functions[i][0];
        probs.append(ps);
    }
    probs.append(1);
    QVector<double> func;
    QPointF cur = start;
    for (int i = 0; i < it; ++i) {
        x = cur.x(), y = cur.y();
        p = QRandomGenerator::global()->generateDouble();
        int j;
        for (j = 0; j < n; ++j) {
            if (probs[j] <= p && p < probs[j + 1]) break;
        }
        func = functions[j];
        if (func[1] == 0) {
            cur = {func[2] * x + func[3] * y + func[6], func[4] * x + func[5] * y + func[7]};
        } else {
            double x0 = func[2], y0 = func[3], r = func[4];
            double denom = (x - x0) * (x - x0) + (y - y0) * (y - y0);
            if (denom) cur = {x0 + r * r * (x - x0) / denom, y0 + r * r * (y - y0) / denom};
        }
        addEllipse(cur.x() - 0.25, cur.y() - 0.25, 0.5, 0.5, QPen(col), QBrush(col));
    }
}
