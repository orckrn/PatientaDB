
#include <QMessageBox>

#include "main.h"
#include "patientform.h"
#include "ui_patientform.h"

PatientForm::PatientForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientForm)
{
    ui->setupUi(this);

    modelChecks = new QSqlQueryModel();

    connect (
                ui->saveButton,
                SIGNAL (clicked()),
                this,
                SLOT (onSaveRecordButtonClicked())
                );

    connect (
                ui->createCheckButton,
                SIGNAL(clicked()),
                this,
                SLOT (onCreateCheckButtonClicked())
                );

    connect (
                ui->deleteCheckButton,
                SIGNAL(clicked()),
                this,
                SLOT (onDeleteCheckButtonClicked())
                );
}

void PatientForm::setData(QSqlRecord patientRecord) {

    modelChecks->setQuery(
                "select * from TCheck where Patient_Id = " +
                patientRecord.value(Ui::TPatient::PATIENT_ID_INDEX).toString()
                );

    modelChecks->setHeaderData(
                Ui::TCheck::TAG_INDEX,
                Qt::Horizontal,
                QObject::tr("Метка")
                );
    modelChecks->setHeaderData(
                Ui::TCheck::DATE_INDEX,
                Qt::Horizontal,
                QObject::tr("Дата")
                );
    modelChecks->setHeaderData(
                Ui::TCheck::RESOLUTION_INDEX,
                Qt::Horizontal,
                QObject::tr("Резолюция")
                );

    ui->patientChecks->setModel(modelChecks);
    ui->patientChecks->setColumnHidden(Ui::TCheck::CHECK_ID_INDEX, true);
    ui->patientChecks->setColumnHidden(Ui::TCheck::PATIENT_ID_INDEX, true);

    ui->patientChecks->show();

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

    ui->createCheckButton->show();
    ui->deleteCheckButton->show();

    patientId = patientRecord.value(Ui::TPatient::PATIENT_ID_INDEX).toInt();
    mode = Ui::Form_Mode::EDIT_RECORD_MODE;
}

void PatientForm::resetData() {

    modelChecks->setQuery("");
    ui->patientChecks->setModel(modelChecks);

    ui->lastNameEdit->setText("");

    ui->firstNameEdit->setText("");

    ui->secondNameEdit->setText("");

    ui->birthDateEdit->setDate(
                QDate::fromString("2000-01-01", Qt::ISODate)
                );

    ui->streetEdit->setText("");

    ui->buildingEdit->setValue(0);

    ui->blockEdit->setValue(0);

    ui->apartmentsEdit->setValue(0);

    ui->phoneEdit->setText("");

    ui->anamnesisEdit->setText("");

    ui->createCheckButton->hide();
    ui->deleteCheckButton->hide();

    mode = Ui::Form_Mode::CREATE_RECORD_MODE;
}

void PatientForm::onSaveRecordButtonClicked()
{
    QString errorMessage;
    QString queryString;
    QSqlQuery patientQuery;
    QMessageBox::StandardButton reply;

    switch (mode) {

        case Ui::Form_Mode::CREATE_RECORD_MODE: {

            if (ui->lastNameEdit->text().isEmpty()) {
                errorMessage += "Фамилия\n";
            }

            if (ui->firstNameEdit->text().isEmpty()) {
                errorMessage += "Имя\n";
            }

            if (ui->secondNameEdit->text().isEmpty()) {
                errorMessage += "Отчество\n";
            }

            if (!errorMessage.isEmpty()) {

                QMessageBox::critical(
                        this,
                        "Form fields must be defined",
                        errorMessage + "должн(ы) быть определен(ы)"
                        );

            } else {

                reply = QMessageBox::question(
                            this,
                            "Question",
                            "Вы действительно хотите добавить " +
                                ui->firstNameEdit->text() + " " +
                                ui->secondNameEdit->text() + " " +
                                ui->lastNameEdit->text() + " в базу?",
                            QMessageBox::Yes|QMessageBox::No
                            );
                if (reply == QMessageBox::No)
                    return;

                queryString = "insert into TPatient (";
                queryString += "First_Name, Second_Name, Last_Name, ";
                queryString += "Birth_Date, Street, Building, Block, ";
                queryString += "Apartments, Phone_Number, Anamnesis) ";
                queryString += "values (";
                queryString += "'" + ui->firstNameEdit->text() + "', ";
                queryString += "'" + ui->secondNameEdit->text() + "', ";
                queryString += "'" + ui->lastNameEdit->text() + "', ";
                queryString += "'" + ui->birthDateEdit->date().toString(Qt::ISODate) + "', ";
                queryString += "'" + ui->streetEdit->text() + "', ";
                queryString += "'" + ui->buildingEdit->text() + "', ";
                queryString += "'" + ui->blockEdit->text() + "', ";
                queryString += "'" + ui->apartmentsEdit->text() + "', ";
                queryString += "'" + ui->phoneEdit->text() + "', ";
                queryString += "'" + ui->anamnesisEdit->toPlainText() + "')";

                patientQuery.exec(queryString);

                ui->createCheckButton->show();
                ui->deleteCheckButton->show();

                emit updatePatientTable();

                queryString = "select * from TPatient where ";
                queryString += "First_Name='" + ui->firstNameEdit->text() + "' and ";
                queryString += "Second_Name='" + ui->secondNameEdit->text() + "' and ";
                queryString += "Last_Name='" + ui->lastNameEdit->text() + "' and ";
                queryString += "Birth_Date='" + ui->birthDateEdit->date().toString(Qt::ISODate) + "' and ";
                queryString += "Street='" + ui->streetEdit->text() + "' and ";
                queryString += "Building='" + ui->buildingEdit->text() + "' and ";
                queryString += "Block='" + ui->blockEdit->text() + "' and ";
                queryString += "Apartments='" + ui->apartmentsEdit->text() + "' and ";
                queryString += "Phone_Number='" + ui->phoneEdit->text() + "' and ";
                queryString += "Anamnesis='" + ui->anamnesisEdit->toPlainText() + "'";
                patientQuery.exec(queryString);
                patientQuery.next();
                patientId = patientQuery.value(Ui::TPatient::PATIENT_ID_INDEX).toInt();

                mode = Ui::Form_Mode::EDIT_RECORD_MODE;
            }

        } break;

        case Ui::Form_Mode::EDIT_RECORD_MODE: {

            if (ui->lastNameEdit->text().isEmpty()) {
                errorMessage += "Фамилия\n";
            }

            if (ui->firstNameEdit->text().isEmpty()) {
                errorMessage += "Имя\n";
            }

            if (ui->secondNameEdit->text().isEmpty()) {
                errorMessage += "Отчество\n";
            }

            if (!errorMessage.isEmpty()) {

                QMessageBox::critical(
                        this,
                        "Form fields must be defined",
                        errorMessage + "должн(ы) быть определен(ы)"
                        );

            } else {

                reply = QMessageBox::question(
                            this,
                            "Question",
                            "Вы действительно хотите обновить " +
                                ui->firstNameEdit->text() + " " +
                                ui->secondNameEdit->text() + " " +
                                ui->lastNameEdit->text() + " в базе?",
                            QMessageBox::Yes|QMessageBox::No
                            );
                if (reply == QMessageBox::No)
                    return;

                queryString = "update TPatient set ";
                queryString += "First_Name='" + ui->firstNameEdit->text() + "', ";
                queryString += "Second_Name='" + ui->secondNameEdit->text() + "', ";
                queryString += "Last_Name='" + ui->lastNameEdit->text() + "', ";
                queryString += "Birth_Date='" + ui->birthDateEdit->date().toString(Qt::ISODate) + "', ";
                queryString += "Street='" + ui->streetEdit->text() + "', ";
                queryString += "Building='" + ui->buildingEdit->text() + "', ";
                queryString += "Block='" + ui->blockEdit->text() + "', ";
                queryString += "Apartments='" + ui->apartmentsEdit->text() + "', ";
                queryString += "Phone_Number='" + ui->phoneEdit->text() + "', ";
                queryString += "Anamnesis='" + ui->anamnesisEdit->toPlainText() + "' ";
                queryString += "where Id='" + QString::number(patientId) + "'";

                patientQuery.exec(queryString);

                emit updatePatientTable();
            }

        } break;
    }
}

void PatientForm::onCreateCheckButtonClicked() {

    return; //  TBD
}

void PatientForm::onDeleteCheckButtonClicked() {

    return; //  TBD
}

PatientForm::~PatientForm()
{
    delete ui;
}
