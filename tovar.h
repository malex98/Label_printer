#ifndef TOVAR_H
#define TOVAR_H

#include <QObject>
#include <QVector>
#include <QDate>

class Tovar
{
public:
    Tovar();

    QVector<QString> strs;

    QString username();     //0 только для QCombobox (в БД 1 сивол означает сеть)
    QString barcode_1ps();  //1 ШК на батон
    QString barcode_2kg();  //2 ШК на 2кг короб
    QString barcode_3kg();  //3 ШК на 3кг короб
    QString line1();        //4 1 строка наименования на батоне
    QString line2();        //5 2 строка наименования на батоне
    QString conditions();   //6 условия хранения
    QString boxline1();     //7 1 строка наименования на коробке
    QString boxline2();     //8 2 строка наименования на коробке
    QDate date_made();      //9 дата изготовления
    QDate date_pack();      //10 дата упаковки
    QDate date_expire();    //  дата "годен до"

    QVector<int> ints;

    int nomer();            //0 номер для партии
    int weight_1ps();       //1 вес одного батона в граммах
    int pack_2kg();         //2 кол-во в коробке 2кг
    int pack_3kg();         //3 кол-во в коробке 3кг
    int storage_life();     //4 срок годности в днях
    int box_format();       //5 1-батон, 2-2кг коробка, 3-3кг коробка
    int unit_kol();         //6 кол-во этикеток на 1 батон
    int kol_vsego;          //  всего штук в заказе для box_format()< 4
    int id_product,id_heap;
    QChar icon;

private:
    QString getString(int i);
    int getInteger(int i);
    QDate getDate(int i);

signals:

public slots:
};

#endif // TOVAR_H
