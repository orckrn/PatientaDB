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

private slots:
    void onTableClicked(const QModelIndex &index);
    void onTableDoubleClicked(const QModelIndex &index);
    void onCreateRecordButtonClicked();
    void onDeleteRecordButtonClicked();
    void onUpdatePatientTable();
    void searchOnFirstNameChanged(const QString &text);
    void searchOnSecondNameChanged(const QString &text);
    void searchOnLastNameChanged(const QString &text);
    void searchOnYearChanged(const QString &text);
};

#endif // MAINWINDOW_H
