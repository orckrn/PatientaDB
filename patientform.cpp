
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

    connect (
                ui->checkList,
                SIGNAL(doubleClicked(const QModelIndex &)),
                this,
                SLOT(onTableDoubleClicked(const QModelIndex &))
                );

    checkForm = new CheckForm;

    connect(
                checkForm,
                SIGNAL (updateCheckTable()),
                this,
                SLOT (onUpdateCheckTable())
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

    ui->checkList->setModel(modelChecks);
    ui->checkList->setColumnHidden(Ui::TCheck::CHECK_ID_INDEX, true);
    ui->checkList->setColumnHidden(Ui::TCheck::PATIENT_ID_INDEX, true);

    ui->checkList->show();

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
    ui->checkList->setModel(modelChecks);

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
        return;
    }

    switch (mode) {

        case Ui::Form_Mode::CREATE_RECORD_MODE: {

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
            queryString += "Phone_Number='" + ui->phoneEdit->text() + "'";

            patientQuery.exec(queryString);
            patientQuery.next();
            patientId = patientQuery.value(Ui::TPatient::PATIENT_ID_INDEX).toInt();

            mode = Ui::Form_Mode::EDIT_RECORD_MODE;

        } break;

        case Ui::Form_Mode::EDIT_RECORD_MODE: {

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

        } break;
    }
}

void PatientForm::onCreateCheckButtonClicked() {

    checkForm->resetData(patientId);
    checkForm->show();
    checkForm->activateWindow();
    checkForm->raise();
}

void PatientForm::onDeleteCheckButtonClicked() {

    QMessageBox::StandardButton reply;

    QModelIndexList selection = ui->checkList->selectionModel()->selectedRows();
    if (selection.size()) {

        QModelIndex index = selection.at (0);
        QSqlRecord record = modelChecks->record(index.row());

        reply = QMessageBox::question(
                    this,
                    "Question",
                    "Вы действительно хотите удалить " +
                        record.value(Ui::TCheck::TAG_INDEX).toString() + " из базы?",
                    QMessageBox::Yes|QMessageBox::No
                    );
        if (reply == QMessageBox::No)
            return;

        int checkId = record.value(
                    Ui::TCheck::CHECK_ID_INDEX
                    ).toInt();

        QString queryString;
        queryString = "delete from TCheck where ";
        queryString += "Id='" + QString::number(checkId) + "'";

        QSqlQuery patientQuery;
        patientQuery.exec(queryString);

        onUpdateCheckTable();

        checkForm->hide();

    } else {

        QMessageBox::critical(
                    this,
                    "Select to remove",
                    "Не выбрана запись посещения для удаления"
                    );
    }
}

void PatientForm::onTableDoubleClicked(const QModelIndex &index) {

    if (index.isValid()) {
        QSqlRecord checkRecord = modelChecks->record(index.row());
        checkForm->setData(checkRecord);
    }

    checkForm->show();
    checkForm->activateWindow();
    checkForm->raise();
}

void PatientForm::onUpdateCheckTable() {

    QString queryString = "select * from TCheck where ";
    queryString += "Patient_Id='" + QString::number(patientId) + "'";
    modelChecks->setQuery(queryString);
}

PatientForm::~PatientForm()
{
    delete ui;
    delete modelChecks;
    delete checkForm;
}
