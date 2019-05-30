
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>

#include "main.h"
#include "mainwindow.h"
#include "patientform.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (!QSqlDatabase::drivers().contains("QSQLITE")) {
        QMessageBox::critical(
                    this,
                    "Unable to load database",
                    "This app needs the SQLITE driver"
                    );
        return;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("PatientaDB.db");
    if (!db.open()) {
        QMessageBox::critical(
                    this,
                    "Unable to open database",
                    "This app needs the PatientaDB.db database"
                    );
        return;
    }

    modelPatients = new QSqlQueryModel();
    modelPatients->setQuery("select * from TPatient");

    modelPatients->setHeaderData(
                Ui::TPatient::FIRST_NAME_INDEX,
                Qt::Horizontal,
                QObject::tr("Имя")
                );
    modelPatients->setHeaderData(
                Ui::TPatient::SECOND_NAME_INDEX,
                Qt::Horizontal,
                QObject::tr("Отчество")
                );
    modelPatients->setHeaderData(
                Ui::TPatient::LAST_NAME_INDEX,
                Qt::Horizontal,
                QObject::tr("Фамилия")
                );
    modelPatients->setHeaderData(
                Ui::TPatient::BIRTH_DATE_INDEX,
                Qt::Horizontal,
                QObject::tr("Дата Рождения")
                );

    ui->patientsList->setModel(modelPatients);

    ui->patientsList->setColumnHidden(Ui::TPatient::PATIENT_ID_INDEX, true);
    ui->patientsList->setColumnHidden(Ui::TPatient::STREET_INDEX, true);
    ui->patientsList->setColumnHidden(Ui::TPatient::BUILDING_INDEX, true);
    ui->patientsList->setColumnHidden(Ui::TPatient::BLOCK_INDEX, true);
    ui->patientsList->setColumnHidden(Ui::TPatient::APARTMENTS_INDEX, true);
    ui->patientsList->setColumnHidden(Ui::TPatient::PHONE_NUMBER_INDEX, true);
    ui->patientsList->setColumnHidden(Ui::TPatient::ANAMNESIS_INDEX, true);

    ui->patientsList->show();

    connect(
                ui->patientsList,
                SIGNAL(clicked(const QModelIndex &)),
                this,
                SLOT(onTableClicked(const QModelIndex &))
                );

    connect (
                ui->patientsList,
                SIGNAL(doubleClicked(const QModelIndex &)),
                this,
                SLOT(onTableDoubleClicked(const QModelIndex &))
                );

    connect(
                ui->createButton,
                SIGNAL (clicked()),
                this,
                SLOT (onCreateRecordButtonClicked())
                );

    connect(
                ui->deleteButton,
                SIGNAL (clicked()),
                this,
                SLOT (onDeleteRecordButtonClicked())
                );

    patientForm = new PatientForm;

    connect(
                patientForm,
                SIGNAL (updatePatientTable()),
                this,
                SLOT (onUpdatePatientTable())
                );

    connect (
                ui->searchFirstName,
                SIGNAL (textChanged(const QString &)),
                this,
                SLOT (searchOnFirstNameChanged(const QString &))
                );

    connect (
                ui->searchSecondName,
                SIGNAL (textChanged(const QString &)),
                this,
                SLOT (searchOnSecondNameChanged(const QString &))
                );

    connect (
                ui->searchLastName,
                SIGNAL (textChanged(const QString &)),
                this,
                SLOT (searchOnLastNameChanged(const QString &))
                );

    connect (
                ui->searchYear,
                SIGNAL (textChanged(const QString &)),
                this,
                SLOT (searchOnYearChanged(const QString &))
                );
}

void MainWindow::onTableClicked(const QModelIndex &index)
{
    if (index.isValid()) {

        QString lastNameText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::TPatient::LAST_NAME_INDEX
                        )
                    ).toString();
        ui->labelLastName->setText(lastNameText);

        QString firstNameText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::TPatient::FIRST_NAME_INDEX
                        )
                    ).toString();
        ui->labelFirstName->setText(firstNameText);

        QString secondNameText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::TPatient::SECOND_NAME_INDEX
                        )
                    ).toString();
        ui->labelSecondName->setText(secondNameText);

        QString birthDateString = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::TPatient::BIRTH_DATE_INDEX
                        )
                    ).toString();
        ui->labelBirthDate->setText(birthDateString);

        QString streetText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::TPatient::STREET_INDEX
                        )
                    ).toString();
        QString buildingText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::TPatient::BUILDING_INDEX
                        )
                    ).toString();
        QString blockText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::TPatient::BLOCK_INDEX
                        )
                    ).toString();
        QString appartmentsText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::TPatient::APARTMENTS_INDEX
                        )
                    ).toString();
        ui->labelAddress->setText(
                    streetText + ", " +
                    "д. " + buildingText + ", "  +
                    "к. " + blockText + ", " +
                    "кв. " + appartmentsText
                    );

        QString phoneNumberText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::TPatient::PHONE_NUMBER_INDEX
                        )
                    ).toString();
        ui->labelPhoneNumber->setText (phoneNumberText);
    }
}

void MainWindow::onTableDoubleClicked(const QModelIndex &index) {

    if (index.isValid()) {
        QSqlRecord patientRecord = modelPatients->record(index.row());
        patientForm->setData(patientRecord);
    }

    patientForm->show();
    patientForm->activateWindow();
    patientForm->raise();
}

void MainWindow::onCreateRecordButtonClicked() {

    patientForm->resetData();
    patientForm->show();
    patientForm->activateWindow();
    patientForm->raise();
}

void MainWindow::onDeleteRecordButtonClicked() {

    QMessageBox::StandardButton reply;

    QModelIndexList selection = ui->patientsList->selectionModel()->selectedRows();

    if (selection.size()) {

        QModelIndex index = selection.at (0);
        QSqlRecord record = modelPatients->record(index.row());

        reply = QMessageBox::question(
                    this,
                    "Question",
                    "Вы действительно хотите удалить " +
                        record.value(Ui::TPatient::FIRST_NAME_INDEX).toString() + " " +
                        record.value(Ui::TPatient::SECOND_NAME_INDEX).toString() + " " +
                        record.value(Ui::TPatient::LAST_NAME_INDEX).toString() + " из базы?",
                    QMessageBox::Yes|QMessageBox::No
                    );
        if (reply == QMessageBox::No)
            return;

        int patientId = record.value(
                    Ui::TPatient::PATIENT_ID_INDEX
                    ).toInt();

        QString queryString;
        queryString = "delete from TPatient where ";
        queryString += "Id='" + QString::number(patientId) + "'";

        QSqlQuery patientQuery;
        patientQuery.exec(queryString);

        queryString = "delete from TCheck where ";
        queryString += "Patient_Id='" + QString::number(patientId) + "'";
        patientQuery.exec(queryString);

        onUpdatePatientTable();

        patientForm->hide();

    } else {
        QMessageBox::critical(
                    this,
                    "Select to remove",
                    "Не выбран пациент для удаления"
                    );
    }
}

void MainWindow::onUpdatePatientTable() {

    modelPatients->setQuery("select * from TPatient");
}

void MainWindow::searchOnFirstNameChanged(const QString &text) {

    QString query = "select * from TPatient where ";
    query += "First_Name like '%" + text + "%' and ";
    query += "Second_Name like '%" + ui->searchSecondName->text() + "%' and ";
    query += "Last_Name like '%" + ui->searchLastName->text() + "%' and ";
    query += "Birth_Date like '%" + ui->searchYear->text() + "%'";

    modelPatients->setQuery(query);
}

void MainWindow::searchOnSecondNameChanged(const QString &text) {

    QString query = "select * from TPatient where ";
    query += "First_Name like '%" + ui->searchFirstName->text() + "%' and ";
    query += "Second_Name like '%" + text + "%' and ";
    query += "Last_Name like '%" + ui->searchLastName->text() + "%' and ";
    query += "Birth_Date like '%" + ui->searchYear->text() + "%'";

    modelPatients->setQuery(query);
}

void MainWindow::searchOnLastNameChanged(const QString &text) {

    QString query = "select * from TPatient where ";
    query += "First_Name like '%" + ui->searchFirstName->text() + "%' and ";
    query += "Second_Name like '%" + ui->searchSecondName->text() + "%' and ";
    query += "Last_Name like '%" + text + "%' and ";
    query += "Birth_Date like '%" + ui->searchYear->text() + "%'";

    modelPatients->setQuery(query);
}

void MainWindow::searchOnYearChanged(const QString &text) {

    QString query = "select * from TPatient where ";
    query += "First_Name like '%" + ui->searchFirstName->text() + "%' and ";
    query += "Second_Name like '%" + ui->searchSecondName->text() + "%' and ";
    query += "Last_Name like '%" + ui->searchLastName->text() + "%' and ";
    query += "Birth_Date like '%" + text + "%'";

    modelPatients->setQuery(query);
}

MainWindow::~MainWindow()
{
    delete ui;
    db.close();
    delete modelPatients;
    delete patientForm;
}
