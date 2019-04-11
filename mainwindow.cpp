
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>

#include "mainwindow.h"
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

    modelPatients->setHeaderData(1, Qt::Horizontal, QObject::tr("Имя"));
    modelPatients->setHeaderData(2, Qt::Horizontal, QObject::tr("Отчество"));
    modelPatients->setHeaderData(3, Qt::Horizontal, QObject::tr("Фамилия"));
    modelPatients->setHeaderData(4, Qt::Horizontal, QObject::tr("Дата Рождения"));

    ui->patientsList->setModel(modelPatients);

    ui->patientsList->setColumnHidden(0, true);
    ui->patientsList->setColumnHidden(5, true);
    ui->patientsList->setColumnHidden(6, true);
    ui->patientsList->setColumnHidden(7, true);
    ui->patientsList->setColumnHidden(8, true);
    ui->patientsList->setColumnHidden(9, true);
    ui->patientsList->setColumnHidden(10, true);

    connect(
                ui->patientsList,
                SIGNAL(clicked(const QModelIndex &)),
                this,
                SLOT(onTableClicked(const QModelIndex &))
                );

    ui->patientsList->show();
}

void MainWindow::onTableClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString cellText = index.data().toString();
        return;
    }
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}
