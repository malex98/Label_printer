#include "dchoosetovar.h"
#include "ui_dchoosetovar.h"
#include <QDebug>

dChooseTovar::dChooseTovar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dChooseTovar)
{
    ui->setupUi(this);
    clearcbNaim();
}

dChooseTovar::~dChooseTovar()
{
    delete ui;
}

void dChooseTovar::addTovar(QString str, int i, QIcon icon)
{
    ind.append(i);
    ui->cbName->addItem(icon, str);
    ui->cbName->setCurrentIndex(i);
    qDebug()<<"da"<<i<<str;
}

int dChooseTovar::count()
{
    return ind.count();
}

int dChooseTovar::result()
{
    if(ui->cbName->count() == 0)
        return -1;
    if(ui->cbName->currentIndex()<0)    return ind.at(0);
    return ind.at(ui->cbName->currentIndex());
}

void dChooseTovar::clearcbNaim()
{
    ui->cbName->clear();
    ind.clear();
}
