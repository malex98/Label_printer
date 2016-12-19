#include "tovar.h"
#include <QDebug>

Tovar::Tovar()
{

}

QString Tovar::getString(int i)
{
    if((i >= 0)&&(i < strs.count()))
        return strs.at(i);
    qDebug()<<"Incorrect index in getString: "<<i;
    return "";
}

int Tovar::getInteger(int i)
{
    if((i >= 0)&&(i < ints.count()))
        return ints.at(i);
    qDebug()<<"Incorrect index in getInteger: "<<i;
    return 0;
}

QDate Tovar::getDate(int i)
{
    QString date = getString(i);
    int y = date.left(4).toInt();
    int d = date.right(2).toInt();
    int m = date.mid(5,2).toInt();

    return QDate(y,m,d);
}

QString Tovar::username()
{
    return getString(0);
}

QString Tovar::barcode_1ps()
{
    return getString(1);
}

QString Tovar::barcode_2kg()
{
    return getString(2);
}

QString Tovar::barcode_3kg()
{
    return getString(3);
}

QString Tovar::line1()
{
    return getString(4);
}

QString Tovar::line2()
{
    return getString(5);
}

QString Tovar::conditions()
{
    return getString(6);
}

QString Tovar::boxline1()
{
    return getString(7);
}

QString Tovar::boxline2()
{
    return getString(8);
}

QDate Tovar::date_made()
{
    return getDate(9);
}

QDate Tovar::date_pack()
{
    return getDate(10);
}

QDate Tovar::date_expire()
{
    return date_made().addDays(storage_life());
}

int Tovar::nomer()
{
    return getInteger(0);
}

int Tovar::weight_1ps()
{
    return getInteger(1);
}

int Tovar::pack_2kg()
{
    return getInteger(2);
}

int Tovar::pack_3kg()
{
    return getInteger(3);
}

int Tovar::storage_life()
{
    return getInteger(4);
}

int Tovar::box_format()
{
    return getInteger(5);
}

int Tovar::unit_kol()
{
    return getInteger(6);
}


