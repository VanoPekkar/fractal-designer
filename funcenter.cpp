#include "funcenter.h"
#include <QDebug>
#include <QString>

FuncEnterLineEdit::FuncEnterLineEdit(QWidget* parent) : QLineEdit(parent) {
    _fparser.AddConstant("pi", std::complex<double>{3.14159265358979323846, 0});
    _fparser.AddConstant("e", std::complex<double>{2.71828182845904523536, 0});
}

std::complex<double> FuncEnterLineEdit::eval(std::complex<double>* params) {
    std::complex<double> result = _fparser.Eval(params);
    if (!_fparser.EvalError()) {
        return result;
    }
    throw std::runtime_error("Eval error. Last input: " +
                             std::to_string(params[0].real()) + "+" +
                             std::to_string(params[0].imag()) + "i" +
                             " and " +
                             std::to_string(params[1].real()) + "+" +
                             std::to_string(params[1].imag()) + "i"
                             );
}

void FuncEnterLineEdit::parse_two_vars() {
    std::string var = text().toStdString();
    if (QString::fromStdString(std::to_string(1.1)).contains(",")) var = text().replace(".", ",").toStdString();
    int success = _fparser.Parse(var, "z,c");
    if (success == -1) {
        return;
    }
    throw std::runtime_error("Parse_Mandel error!");
}


void FuncEnterLineEdit::parse_one_var() {
    std::string var = text().toStdString();
    if (QString::fromStdString(std::to_string(1.1)).contains(",")) var = text().replace(".", ",").toStdString();
    int success = _fparser.Parse(var, "z");
    if (success == -1) {
        return;
    }
    throw std::runtime_error("Parse_Julia error!");
}

FuncEnterLineEdit::FuncEnterLineEdit(const FuncEnterLineEdit* ptr) {
    this->setText(ptr->text());
}

