
#include "main.h"
#include "patientform.h"
#include "ui_patientform.h"

PatientForm::PatientForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientForm)
{
    ui->setupUi(this);
}

void PatientForm::setData(QSqlRecord patientRecord) {

    ui->lastNameEdit->setText(
                patientRecord.value(Ui::LAST_NAME_INDEX).toString()
                );

    ui->firstNameEdit->setText(
                patientRecord.value(Ui::FIRST_NAME_INDEX).toString()
                );

    ui->secondNameEdit->setText(
                patientRecord.value(Ui::SECOND_NAME_INDEX).toString()
                );
}

PatientForm::~PatientForm()
{
    delete ui;
}
