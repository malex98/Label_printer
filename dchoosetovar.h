#ifndef DCHOOSETOVAR_H
#define DCHOOSETOVAR_H

#include <QDialog>

namespace Ui {
class dChooseTovar;
}

class dChooseTovar : public QDialog
{
    Q_OBJECT

public:
    explicit dChooseTovar(QWidget *parent = 0);
    ~dChooseTovar();
    void addTovar(QString str, int i, QIcon icon);//добавляет строку в QCombobox, id, и иконку
    int count();
    int result();
    void clearcbNaim();

private:
    Ui::dChooseTovar *ui;
    QVector<int> ind;
};

#endif // DCHOOSETOVAR_H
