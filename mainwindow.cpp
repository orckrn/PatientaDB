
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
                Ui::FIRST_NAME_INDEX,
                Qt::Horizontal,
                QObject::tr("Имя")
                );
    modelPatients->setHeaderData(
                Ui::SECOND_NAME_INDEX,
                Qt::Horizontal,
                QObject::tr("Отчество")
                );
    modelPatients->setHeaderData(
                Ui::LAST_NAME_INDEX,
                Qt::Horizontal,
                QObject::tr("Фамилия")
                );
    modelPatients->setHeaderData(Ui::BIRTH_DATE_INDEX, Qt::Horizontal, QObject::tr("Дата Рождения"));

    ui->patientsList->setModel(modelPatients);

    ui->patientsList->setColumnHidden(Ui::PATIENT_ID_INDEX, true);
    ui->patientsList->setColumnHidden(Ui::STREET_INDEX, true);
    ui->patientsList->setColumnHidden(Ui::BUILDING_INDEX, true);
    ui->patientsList->setColumnHidden(Ui::BLOCK_INDEX, true);
    ui->patientsList->setColumnHidden(Ui::APARTMENTS_INDEX, true);
    ui->patientsList->setColumnHidden(Ui::PHONE_NUMBER_INDEX, true);
    ui->patientsList->setColumnHidden(Ui::ANAMNESIS_INDEX, true);

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

    patientForm = new PatientForm;
}

void MainWindow::onTableClicked(const QModelIndex &index)
{
    if (index.isValid()) {

        QString lastNameText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::LAST_NAME_INDEX
                        )
                    ).toString();
        ui->labelLastName->setText(lastNameText);

        QString firstNameText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::FIRST_NAME_INDEX
                        )
                    ).toString();
        ui->labelFirstName->setText(firstNameText);

        QString secondNameText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::SECOND_NAME_INDEX
                        )
                    ).toString();
        ui->labelSecondName->setText(secondNameText);

        QString birthDateString = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::BIRTH_DATE_INDEX
                        )
                    ).toString();
        ui->labelBirthDate->setText(birthDateString);

        QString streetText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::STREET_INDEX
                        )
                    ).toString();
        QString buildingText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::BUILDING_INDEX
                        )
                    ).toString();
        QString blockText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::BLOCK_INDEX
                        )
                    ).toString();
        QString appartmentsText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        Ui::APARTMENTS_INDEX
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
                        Ui::PHONE_NUMBER_INDEX
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

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}
