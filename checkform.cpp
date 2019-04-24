
#include <QMessageBox>

#include "main.h"
#include "checkform.h"
#include "ui_checkform.h"

CheckForm::CheckForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckForm)
{
    ui->setupUi(this);

    connect (
                ui->saveButton,
                SIGNAL (clicked()),
                this,
                SLOT (onSaveRecordButtonClicked())
                );
}

void CheckForm::setData(QSqlRecord checkRecord) {

    int patientId;
    patientId = checkRecord.value(Ui::TCheck::PATIENT_ID_INDEX).toInt();

    fillPatientData(patientId);

    ui->tagEdit->setText(
                checkRecord.value(Ui::TCheck::TAG_INDEX).toString()
                );

    ui->dateEdit->setDate(
                checkRecord.value(Ui::TCheck::DATE_INDEX).toDate()
                );

    ui->resolutionEdit->setText(
                checkRecord.value(Ui::TCheck::RESOLUTION_INDEX).toString()
                );

    mode = Ui::Form_Mode::EDIT_RECORD_MODE;
}

void CheckForm::resetData(int patientId) {

    fillPatientData(patientId);

    ui->tagEdit->setText("");

    ui->dateEdit->setDate(QDate::fromString("2000-01-01", Qt::ISODate));

    ui->resolutionEdit->setText("");

    mode = Ui::Form_Mode::CREATE_RECORD_MODE;
}

void CheckForm::fillPatientData(int patientId) {

    QSqlQuery query;
    QString queryString;

    queryString = "select * from TPatient where ";
    queryString += "Id='" + QString::number(patientId) + "'";
    query.exec(queryString);

    query.next();

    ui->labelLastName->setText(
                query.value(Ui::TPatient::LAST_NAME_INDEX).toString()
                );

    ui->labelFirstName->setText(
                query.value(Ui::TPatient::FIRST_NAME_INDEX).toString()
                );

    ui->labelSecondName->setText(
                query.value(Ui::TPatient::SECOND_NAME_INDEX).toString()
                );

    ui->labelBirthDate->setText(
                query.value(Ui::TPatient::BIRTH_DATE_INDEX).toString()
                );
}

void CheckForm::onSaveRecordButtonClicked() {

    QString errorMessage;

    if (ui->tagEdit->text().isEmpty()) {
        errorMessage += "Метка посещения\n";
    }

    if (ui->resolutionEdit->toPlainText().isEmpty()) {
        errorMessage += "Резолюция\n";
    }

    if (!errorMessage.isEmpty()) {

        QMessageBox::critical(
                this,
                "Form fields must be defined",
                errorMessage + "должн(ы) быть определен(ы)"
                );
        return;
    }

    switch (mode) {

        case Ui::Form_Mode::CREATE_RECORD_MODE: {

        } break;

        case Ui::Form_Mode::EDIT_RECORD_MODE: {
        } break;
    }
}

CheckForm::~CheckForm()
{
    delete ui;
}
