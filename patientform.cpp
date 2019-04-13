
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
                patientRecord.value(Ui::TPatient::LAST_NAME_INDEX).toString()
                );

    ui->firstNameEdit->setText(
                patientRecord.value(Ui::TPatient::FIRST_NAME_INDEX).toString()
                );

    ui->secondNameEdit->setText(
                patientRecord.value(Ui::TPatient::SECOND_NAME_INDEX).toString()
                );

    ui->birthDateEdit->setDate(
                patientRecord.value(Ui::TPatient::BIRTH_DATE_INDEX).toDate()
                );

    ui->streetEdit->setText(
                patientRecord.value(Ui::TPatient::STREET_INDEX).toString()
                );

    ui->buildingEdit->setValue(
                patientRecord.value(Ui::TPatient::BUILDING_INDEX).toInt()
                );

    ui->blockEdit->setValue(
                patientRecord.value(Ui::TPatient::BLOCK_INDEX).toInt()
                );

    ui->apartmentsEdit->setValue(
                patientRecord.value(Ui::TPatient::APARTMENTS_INDEX).toInt()
                );

    ui->phoneEdit->setText(
                patientRecord.value(Ui::TPatient::PHONE_NUMBER_INDEX).toString()
                );

    ui->anamnesisEdit->setText(
                patientRecord.value(Ui::TPatient::ANAMNESIS_INDEX).toString()
                );

    mode = Ui::Form_Mode::EDIT_RECORD_MODE;
}

void PatientForm::resetData() {

    ui->lastNameEdit->setText("");

    ui->firstNameEdit->setText("");

    ui->secondNameEdit->setText("");

    ui->birthDateEdit->setDate(
                QDate::fromString("0000-01-01",Qt::ISODate)
                );

    ui->streetEdit->setText("");

    ui->buildingEdit->setValue(0);

    ui->blockEdit->setValue(0);

    ui->apartmentsEdit->setValue(0);

    ui->phoneEdit->setText("");

    ui->anamnesisEdit->setText("");

    mode = Ui::Form_Mode::CREATE_RECORD_MODE;
}

PatientForm::~PatientForm()
{
    delete ui;
}
