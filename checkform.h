#ifndef CHECKFORM_H
#define CHECKFORM_H

#include <QtSql>
#include <QWidget>

#include "main.h"

namespace Ui {

    class CheckForm;
}

class CheckForm : public QWidget
{
    Q_OBJECT

public:
    explicit CheckForm(QWidget *parent = nullptr);
    ~CheckForm();
    void setData(QSqlRecord);
    void resetData(int);

private:
    Ui::CheckForm *ui;
    Ui::Form_Mode mode;

private:
    void fillPatientData(int);

private slots:
    void onSaveRecordButtonClicked();
};

#endif // CHECKFORM_H
