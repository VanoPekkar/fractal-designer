#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};
#endif // MAINWIDGET_H
