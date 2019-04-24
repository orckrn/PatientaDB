#ifndef PATIENTFORM_H
#define PATIENTFORM_H

#include <QtSql>
#include <QWidget>

#include "main.h"
#include "checkform.h"

namespace Ui {

    class PatientForm;
}

class PatientForm : public QWidget
{
    Q_OBJECT

public:
    explicit PatientForm(QWidget *parent = nullptr);
    ~PatientForm();
    void setData(QSqlRecord);
    void resetData();

private:
    Ui::PatientForm *ui;
    Ui::Form_Mode mode;
    QSqlQueryModel *modelChecks;
    int patientId;
    CheckForm *checkForm;

signals:
    void updatePatientTable();

private slots:
    void onSaveRecordButtonClicked();
    void onCreateCheckButtonClicked();
    void onDeleteCheckButtonClicked();
    void onTableDoubleClicked(const QModelIndex &);
};

#endif // PATIENTFORM_H
