#ifndef FUNCENTER_H
#define FUNCENTER_H

#include <QLineEdit>
#include "Lib/fparser4.5.2/fparser.hh"


class FuncEnterLineEdit : public QLineEdit {

    Q_OBJECT

public:
    FuncEnterLineEdit(QWidget* parent = nullptr);
    std::complex<double> eval(std::complex<double>* params);
    void parse_two_vars();
    void parse_one_var();

public:
    FunctionParser_cd _fparser;
};


#endif // FUNCENTER_H
