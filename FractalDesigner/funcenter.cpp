#include "funcenter.h"


FuncEnterLineEdit::FuncEnterLineEdit(QWidget* parent) : QLineEdit(parent) {
    _fparser.AddConstant("pi", std::complex<double>{3.14159265358979323846, 0});
    _fparser.AddConstant("e", std::complex<double>{2.71828182845904523536, 0});
}

std::complex<double> FuncEnterLineEdit::eval(std::complex<double>* params) {
    std::complex<double> result = _fparser.Eval(params);
    if (!_fparser.EvalError()) {
        return result;
    }
    // TODO: think what returns here?
    // throw 1;
    return _fparser.EvalError();
}

void FuncEnterLineEdit::parse_func() {
    std::string var = text().toStdString();
    int success = _fparser.Parse(text().toStdString(), "z,c");
    if (success == -1) {
        return;
    }
    //  TODO: what here
    // throw 1;
}
