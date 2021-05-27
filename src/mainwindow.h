#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "mainview.h"
#include "mainscene.h"
#include "savewindow.h"
#include "ifsfunc.h"

#include<QDialog>
#include<QLabel>
#include<QLineEdit>
#include<QCheckBox>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QMainWindow>
#include<QMessageBox>
#include<QImage>
#include<QPixmap>
#include<QColor>
#include <QSlider>
#include <QGraphicsView>
#include <QComboBox>
#include <QProgressBar>
#include <QGroupBox>
#include <QTextEdit>
#include <QFormLayout>
#include <QStack>
#include <QString>
#include <QLocale>
#include <QShortcut>
#include <QWheelEvent>

#include<QDesktopServices>
#include<QFileInfo>

#include <complex>
#include <cmath>



class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    QWidget* main;
    QHBoxLayout* main_layout;
    QVBoxLayout* right;

    QLabel* mouse_position;

    QWidget* max_iter;
    QHBoxLayout* max_iter_layout;
    QLabel* max_iter_lbl;
    QLineEdit* max_iter_enter;

    QWidget* right_menu_thread;
    QWidget* right_menu_lsys;

    QWidget* derivative_field;

    QWidget* newton_type_changer;
    QLabel* newton_type_lbl;
    QHBoxLayout* newton_type_layout;
    QComboBox* newton_type_box;
    QWidget* newton_args;
    QVBoxLayout* newton_args_layout;
    QHBoxLayout* newton_c_layout;
    QLabel* newton_c_lbl1;
    QLineEdit* newton_c_real;
    QLabel* newton_c_lbl2;
    QLineEdit* newton_c_imag;
    QLabel* newton_c_lbl3;
    QHBoxLayout* newton_a_layout;
    QLabel* newton_a_lbl1;
    QLineEdit* newton_a_real;
    QLabel* newton_a_lbl2;
    QLineEdit* newton_a_imag;
    QLabel* newton_a_lbl3;

    QWidget* tolerance;
    QHBoxLayout* tolerance_layout;
    QLabel* tolerance_lbl;
    QLineEdit* tolerance_enter;

    QWidget* right_menu_ifs;
    QVBoxLayout* ifs_layout;
    QVBoxLayout* ifs_presets_layout;
    QLabel* ifs_presets_lbl;
    QComboBox* ifs_presets;
    QVBoxLayout* points_layout;
    QLabel* points_lbl;
    QTextEdit* points;
    QHBoxLayout* fcount_layout;
    QLabel* fcount_lbl;
    QWidget* func_widget;
    QVBoxLayout* func_layout;
    QLineEdit* fcount;
    QScrollArea* functions;
    QHBoxLayout* ifs_iter_layout;
    QLabel* ifs_iter_lbl;
    QLineEdit* ifs_iter;
    QLabel* start_lbl;
    QLineEdit* start;
    QPointF start_point;

    QVBoxLayout* presets_layout;
    QLabel* presets_lbl;
    QComboBox* lsys_presets;
    QVBoxLayout* axiom_layout;
    QLabel* axiom_lbl;
    QLineEdit* axiom;
    QGroupBox* rules;
    QFormLayout* rules_layout;
    QVBoxLayout* vars_layout;
    QLabel* vars_lbl;
    QTextEdit* vars;
    QHBoxLayout* ang_layout;
    QLabel* ang_lbl;
    QLineEdit* ang;
    QHBoxLayout* iter_layout;
    QLabel* iter_lbl;
    QLineEdit* iter;
    QSlider* angle_slider;
    QComboBox* colormaps_combo;
    QLabel* lbl_colormap;
    QLabel* lbl_offset;
    QLabel* lbl_length;
    QLabel* lbl_funcEnter;
    QLabel* lbl_derivative;
    QLineEdit* rule;
    QCheckBox* cb2;
    QPushButton* ok;
    QPushButton* close;
    QPushButton* reset;
    MainView* view;
    MainScene* scene;
    QSlider* palette_offset_slider;
    QSlider* palette_length_slider;
    QHBoxLayout* layout_derivEnter;

    QVBoxLayout* offset_slider_layout;
    QVBoxLayout* length_slider_layout;
    QHBoxLayout* sliders_and_names;

    QVBoxLayout* right_layout;
    QVBoxLayout* lsys_layout;
    QProgressBar* progress_bar;
    int start_length, start_offset;
    QMap<QChar, QLineEdit*> rule_lines;
    QMap<QChar, QString> rules_map;
    QMap<QChar, QString> actions_map;
    QVector<IFSFunction*> func_vector;
    QVector<QVector<double>> func_table;
    QMap<QString, QPointF> points_map;
    QMap<QString, double> const_map;
    QString axiom_str;
    double angle;
    int iterations;

    QGroupBox* t;

    SaveWindow* save_window = nullptr;

public slots:  // own slots
    void OkClicked();
    void TextChanged(QString str);
    void paletteOffsetChanged(int val);
    void paletteLengthChanged(int val);
    void varTextChanged();
    void axiomTextChanged(QString f);
    void angleTextChanged(QString f);
    void presetChanged(int val);
    void ChangeToMandelbrot();
    void ChangeToJuliaSet();
    void ChangeToNewton();
    void ChangeToSecant();
    void ChangeToIFS();
    void ChangeToLSys();
    void setLog();
    void setPure();
    void collect();
    void collectIFS();
    void clearActions();
    void angleChanged(int val);
    void ifsPresetChanged(int val);
    void fcountChanged(QString s);
    void OpenDocumentation();
    void zoomIn();
    void zoomOut();
    void SaveImage();

signals:  // own signals
    void UpperCase(QString str);
    void Inverse(QString str);
    void Simple(QString str);

};

#endif // MYWINDOW_H
