
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

    ui->patientsList->setFocusPolicy(Qt::NoFocus);
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

    patientForm = new PatientForm;

    connect(
                patientForm,
                SIGNAL (updatePatientTable()),
                this,
                SLOT (onUpdatePatientTable())
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

void MainWindow::onUpdatePatientTable() {

    //  ???
    modelPatients->setQuery("select * from TPatient");
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}
