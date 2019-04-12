
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

    //  birth date

    ui->streetEdit->setText(
                patientRecord.value(Ui::STREET_INDEX).toString()
                );

    //  building, block, apartments

    ui->phoneEdit->setText(
                patientRecord.value(Ui::PHONE_NUMBER_INDEX).toString()
                );

    ui->anamnesisEdit->setText(
                patientRecord.value(Ui::ANAMNESIS_INDEX).toString()
                );

    //  getting checks from TCheck table
}

PatientForm::~PatientForm()
{
    delete ui;
}
