
#include <QMessageBox>

#include "main.h"
#include "checkform.h"
#include "ui_checkform.h"

CheckForm::CheckForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckForm)
{
    ui->setupUi(this);
}

void CheckForm::setData() {
    //  TBD
}

void CheckForm::resetData() {
    //  TBD
}

CheckForm::~CheckForm()
{
    delete ui;
}
