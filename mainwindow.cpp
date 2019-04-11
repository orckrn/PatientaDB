
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>

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
                FIRST_NAME_INDEX,
                Qt::Horizontal,
                QObject::tr("Имя")
                );
    modelPatients->setHeaderData(
                SECOND_NAME_INDEX,
                Qt::Horizontal,
                QObject::tr("Отчество")
                );
    modelPatients->setHeaderData(
                LAST_NAME_INDEX,
                Qt::Horizontal,
                QObject::tr("Фамилия")
                );
    modelPatients->setHeaderData(BIRTH_DATE_INDEX, Qt::Horizontal, QObject::tr("Дата Рождения"));

    ui->patientsList->setModel(modelPatients);

    ui->patientsList->setColumnHidden(PATIENT_ID_INDEX, true);
    ui->patientsList->setColumnHidden(STREET_INDEX, true);
    ui->patientsList->setColumnHidden(BUILDING_INDEX, true);
    ui->patientsList->setColumnHidden(BLOCK_INDEX, true);
    ui->patientsList->setColumnHidden(APARTMENTS_INDEX, true);
    ui->patientsList->setColumnHidden(PHONE_NUMBER_INDEX, true);
    ui->patientsList->setColumnHidden(ANAMNESIS_INDEX, true);

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
                        LAST_NAME_INDEX
                        )
                    ).toString();
        ui->labelLastName->setText(lastNameText);

        QString firstNameText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        FIRST_NAME_INDEX
                        )
                    ).toString();
        ui->labelFirstName->setText(firstNameText);

        QString secondNameText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        SECOND_NAME_INDEX
                        )
                    ).toString();
        ui->labelSecondName->setText(secondNameText);

        QString birthDateString = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        BIRTH_DATE_INDEX
                        )
                    ).toString();
        ui->labelBirthDate->setText(birthDateString);

        QString streetText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        STREET_INDEX
                        )
                    ).toString();
        QString buildingText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        BUILDING_INDEX
                        )
                    ).toString();
        QString blockText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        BLOCK_INDEX
                        )
                    ).toString();
        QString appartmentsText = modelPatients->data(
                    modelPatients->index(
                        index.row(),
                        APARTMENTS_INDEX
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
                        PHONE_NUMBER_INDEX
                        )
                    ).toString();
        ui->labelPhoneNumber->setText (phoneNumberText);
    }
}

void MainWindow::onTableDoubleClicked(const QModelIndex &index) {

    //  TBD
    patientForm->show();
    patientForm->activateWindow();
    patientForm->raise();
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}
