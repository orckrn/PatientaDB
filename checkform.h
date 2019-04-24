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
    void setData();
    void resetData();

private:
    Ui::CheckForm *ui;
    Ui::Form_Mode mode;
};

#endif // CHECKFORM_H
