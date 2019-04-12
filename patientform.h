#ifndef PATIENTFORM_H
#define PATIENTFORM_H

#include <QtSql>
#include <QWidget>

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

private:
    Ui::PatientForm *ui;
};

#endif // PATIENTFORM_H
