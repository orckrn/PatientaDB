#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>

#include "patientform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQueryModel *modelPatients;
    PatientForm *patientForm;

    const int PATIENT_ID_INDEX = 0;
    const int FIRST_NAME_INDEX = 1;
    const int SECOND_NAME_INDEX = 2;
    const int LAST_NAME_INDEX = 3;
    const int BIRTH_DATE_INDEX = 4;
    const int STREET_INDEX = 5;
    const int BUILDING_INDEX = 6;
    const int BLOCK_INDEX = 7;
    const int APARTMENTS_INDEX = 8;
    const int PHONE_NUMBER_INDEX = 9;
    const int ANAMNESIS_INDEX = 10;

private slots:
    void onTableClicked(const QModelIndex &index);
    void onTableDoubleClicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
