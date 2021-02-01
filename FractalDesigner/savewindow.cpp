#include"savewindow.h"

SaveWindow::SaveWindow(QWidget* parent, MainScene* scene) : QWidget() {
    _parent = parent;
    _scene = scene;

    _parent->setEnabled(false);

    // available formats
    box = new QComboBox;
    box->addItem("png");
    box->addItem("bmp");
    box->addItem("jpg");
    box->addItem("jpeg");

    mainLayout = new QVBoxLayout;
    formatLayout = new QHBoxLayout;
    nameLayout = new QHBoxLayout;
    buttonLayout = new QHBoxLayout;
    name = new QLineEdit;
    name->setText("pic");
    save = new QPushButton("&save");
    cancel = new QPushButton("&cancel");
    nameLabel = new QLabel("Enter file's name:");
    formatLabel = new QLabel("Enter file's extention:");

    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(name);

    formatLayout->addWidget(formatLabel);
    formatLayout->addWidget(box);

    buttonLayout->addWidget(cancel);
    buttonLayout->addWidget(save);

    mainLayout->addLayout(nameLayout);
    mainLayout->addLayout(formatLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle("Save Parameters");

    connect(save, SIGNAL(clicked()), this, SLOT(Save()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(Destroy()));
}

void SaveWindow::Save() {
    QString filename = this->name->text();
    if (filename.isEmpty()) {
        return;
    }
    QImage image(_scene->x_picsize, _scene->y_picsize, QImage::Format_ARGB32);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    _scene->render(&painter);
    painter.end();
    QString convertTemp = this->box->currentText();
    filename = filename + "." + convertTemp;
    const char* fmt(convertTemp.toStdString().c_str());
    image.save(filename, fmt, 100);
    this->Destroy();
}

void SaveWindow::closeEvent(QCloseEvent* event) {
    QWidget::closeEvent(event);
    Destroy();
}

void SaveWindow::Destroy() {
    _parent->setEnabled(true);
    delete this;
}
