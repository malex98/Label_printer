#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dchoosetovar.h"
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <qsettings.h>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings("HKEY_CURRENT_USER\\Software\\L.Soft\\Reports2",
                       QSettings::NativeFormat);

    box_printer = settings.value("box_printer").toString();
    unit_printer = settings.value("unit_printer").toString();
    db = QSqlDatabase::addDatabase("QIBASE");
    db.setDatabaseName(settings.value("RepLSdb").toString());

    db.setUserName(settings.value("RepLSuser").toString().mid(10));
    db.setPassword(settings.value("RepLSpass").toString().mid(9));

    if(!db.open()) {
        QMessageBox::warning(0,"1Warning", db.lastError().text());
        qDebug() << db.lastError().text();
        return;
    }
    QSqlQuery a_query;
    if(!a_query.exec("select val_blob from forest where id_priznak=51")) {
        QMessageBox::warning(0,"2Warning", db.lastError().text());
        return;
    }
    ui->cbName->clear();
    QSqlRecord rec = a_query.record();
    if(a_query.next()) {
        QJsonDocument document = QJsonDocument::fromJson(a_query.value(rec.indexOf("val_blob")).toByteArray());//создаём объект Json Document, считав в него все данные из ответа
        QJsonObject jo = document.object();// Забираем из документа корневой объект
        pack_format = jo.value("pack_format").toString();
        unit_format = jo.value("unit_format").toString();
        picture     = jo.value("picture").toString();
        QJsonValue jv = jo.value("icons");
        if(jv.isArray()){
            QJsonArray ja = jv.toArray();
            for(int i = 0; i < ja.count(); i++) {
                QJsonObject jo2 = ja.at(i).toObject();
                if(jo2.contains("file_ch")&&jo2.contains("file")) {
                    icons[jo2.value("file_ch").toString().at(0)]=QIcon(":/images/images/"+jo2.value("file").toString());
                }}
            }
    }
    a_query.clear();
    on_pbRefresh_clicked();
    ui->cbName->setCurrentIndex(-1);
    ui->infText->setText("");
    ui->btPrint->setEnabled(false);
    ui->pbRefresh->setStyleSheet("QPushButton  { border-width: 0px; }");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btExit_clicked()
{
    QApplication::exit();
}

void MainWindow::on_btPrint_clicked()
{
    ui->btPrint->setEnabled(false);
    print();
    ui->btPrint->setEnabled(true);
}

void MainWindow::on_edScaner_returnPressed()
{
    QString status="";
    bool ok ;
    qulonglong scancode = ui->edScaner->text().toLongLong(&ok);
    if(!ok) return;
    switch(scancode) {
    case 29900000000053:    on_btExit_clicked();        //Выключить АРМ
    case 29900000000046:    status = "Отменить ящик";   break;
    case 29900000000015:    status = "Режим +";         break;
    case 29900000000022:    status = "Режим -";         break;
    case 29900000000039:    if(ui->btPrint->isEnabled())//Печать
                                on_btPrint_clicked();
    case 29900000000060:    status = "Повторить последнюю печать";break;
    }
    if(tovars.count()<1)    return;
    dChooseTovar* dialog=new dChooseTovar;
    dialog->clearcbNaim();
    int i;
    Tovar t;
    for(i = 0; i < tovars.count(); i++) {
        t = tovars.at(i);
        qulonglong code = t.barcode_1ps().toLongLong(&ok);
        if(ok && (code==scancode))
            dialog->addTovar(t.date_pack().toString("MM.dd")+"|"+
                             t.date_made().toString("MM.dd")+"|"+
                             QString::number(t.kol_vsego)+" "+
                             t.username(),i,QIcon(icons[t.icon]));
    }
    ui->edScaner->setText("");
    if(dialog->count() >1)
        if(dialog->exec() != QDialog::Accepted) return;//в окне выбрали отменить
    i = dialog->result();
    ui->cbName->setCurrentIndex(i);
    if(i < 0)   {//не выбрали в диалоге наименование или не совпал ШК
        ui->infText->setText("");
        return;
    }
    t = tovars.at(i);
    box_format = t.box_format();
    refreshMessage(i);
    int j = t.kol_vsego/((box_format==2)?t.pack_2kg():t.pack_3kg());
    if(j <=400) ui->sbQuantity->setValue(j);
        else    ui->sbQuantity->setValue(400);
    ui->sbQuantity->setFocus();
}

void MainWindow::print()
{
    Tovar t = tovars.at(ui->cbName->currentIndex());
    QString script;
    QString print_kol = QString::number(ui->sbQuantity->value());
    QString nomer_partii = QString::number(QDate(2015,1,1).daysTo(t.date_made())*50+7+t.nomer());
    double f = t.weight_1ps();  f /= 1000;
    switch (box_format) {
    case 1://батоны (шт)
       script = unit_format.arg(t.line1()).arg(t.line2()).arg(t.date_made().toString("dd.MM.yy.")).arg(t.date_pack().toString("dd.MM.yy.")).arg(t.date_expire().toString("dd.MM.yy.")).arg(f).arg(t.barcode_1ps()).arg(print_kol);
       break;
    case 2://шт в ящики по 2кг
       script = pack_format.arg(t.boxline1()).arg(t.boxline2()).arg(t.date_made().toString("dd.MM.yy.")).arg(t.date_pack().toString("dd.MM.yy.")).arg(t.date_expire().toString("dd.MM.yy.")).arg(f*t.pack_2kg()).arg(t.barcode_2kg()).arg(print_kol).arg(t.conditions()).arg(nomer_partii).arg(picture).arg(t.pack_2kg());
       break;
    case 3://шт в ящики по 3кг
       script = pack_format.arg(t.boxline1()).arg(t.boxline2()).arg(t.date_made().toString("dd.MM.yy.")).arg(t.date_pack().toString("dd.MM.yy.")).arg(t.date_expire().toString("dd.MM.yy.")).arg(f*t.pack_3kg()).arg(t.barcode_3kg()).arg(print_kol).arg(t.conditions()).arg(nomer_partii).arg(picture).arg(t.pack_3kg());
       break;
    default:
       break;
    }
    //qDebug()<<box_format<<script;
    //bool bOk;QString str = QInputDialog::getText( 0,"Input","Name:",QLineEdit::Normal,script,&bOk);//QString script = "SIZE 54 mm, 30 mm\nDIRECTION 1\nREFERENCE 0,0\nOFFSET 0 mm\nSET PEEL OFF\nSET CUTTER OFF\nSET PARTIAL_CUTTER OFF\nSET TEAR ON\nGAP 1.8 mm,0 mm\nCLS\nTEXT 10,10,\"2\",0,1,1,\"Human readable alignment\"\nBARCODE 10,50,\"EAN13+5\",100,1,0,1.5,1,\"97858459083905109\"\nPRINT 1\n";

    QByteArray u8scr= script.toUtf8();
    QString printer;
    if(box_format==1)   printer = unit_printer;
            else        printer = box_printer;
    QFile pr(printer);//for linux QFile pr("/dev/usblp0");   // Device need 0666 permission to work.
    if (pr.open(QFile::WriteOnly)) {
        QTextStream o(&pr);
        o.setCodec("UTF-8");
        o.setGenerateByteOrderMark(true);
        o << u8scr;
        o.flush();
    } else {
        QMessageBox::warning(0,"Ошибка печати","Не найден расшаренный принтер "+printer);
        return;
    }
    ui->edScaner->setFocus();

    QSqlQuery a_query;
    if(!a_query.exec("insert into heap_t(id_product,id_type,kol,val_str) values("+
QString::number(t.id_product)+","+((box_format==1)?"49":"50")+","+QString::number(ui->sbQuantity->value())+",'{\"id_heap\":"+QString::number(t.id_heap)+"}');")) {
        QMessageBox::warning(0,"4Warning", db.lastError().text());
        return;
    }
    a_query.clear();
}

void MainWindow::refreshMessage(int index)
{
    if(index<0) {
        ui->infText->setText("");
        ui->btPrint->setEnabled(false);
        return;
    }
    ui->btPrint->setEnabled(true);
    Tovar t = tovars.at(index);
    float fves = t.weight_1ps();
    QString l1=t.boxline1();
    QString l2=t.boxline2();
    switch(box_format) {
    case 1:
        l1=t.line1();
        l2=t.line2();
        break;
    case 2:
        fves *= t.pack_2kg();
        break;
    case 3:
        fves *= t.pack_3kg();
        break;
    default:
        fves = 0;
    }
    QString ves = QString::number(fves/1000);
    QString kol = (box_format==1)?"":("Кол-во:   "+QString::number(fves/t.weight_1ps())+" шт""<br><br>Условия хранения:    "+t.conditions());
    ui->infText->setText(l1+"<br>"+l2+
                         "<br><br>Изготовлено: "+t.date_made().toString("dd.MM.yy.")+
                         "<br>Упаковано:   "+t.date_pack().toString("dd.MM.yy.")+
                         "<br>Годен до:    "+t.date_expire().toString("dd.MM.yy.")+
                         "<br><br>Масса нетто: "+ves+" кг<br><br>"+kol);
}

void MainWindow::on_cbName_currentIndexChanged(int index)
{
    if((index < 0)|(index >= tovars.count()))    return;
    Tovar t = tovars.at(index);
    box_format = 1;
    refreshMessage(index);
    if(t.kol_vsego > 400)   ui->sbQuantity->setValue(400);
        else                ui->sbQuantity->setValue(t.kol_vsego);
    ui->sbQuantity->setFocus();
}

void MainWindow::on_pbRefresh_clicked()
{
    QSqlQuery a_query;
    if(!a_query.exec("select h.id_heap,h.kol,p.naim,h.id_product,p.val_blob,h.val_str from heap_t h, forest p where h.id_product=p.id_priznak and h.id_type=48 order by h.val_str,p.naim")) {
        QMessageBox::warning(0,"3Warning", db.lastError().text());
        return;
    }
    ui->cbName->clear();
    QSqlRecord rec = a_query.record();
    while (a_query.next()) {
        Tovar tovar = Tovar();
        QJsonDocument document = QJsonDocument::fromJson(a_query.value(rec.indexOf("val_blob")).toByteArray());// То создаём объект Json Document, считав в него все данные из ответа
        QJsonObject root = document.object();
        QString str = root.value("product").toString();
        tovar.strs.append(str);
        QJsonValue jv = root.value("strings");
        if(jv.isArray()){// Если значение является массивом, то забираем массив из данного свойства
            QJsonArray ja = jv.toArray();
            for(int i = 0; i < ja.count(); i++)// Перебирая все элементы массива ...
                tovar.strs.append(ja.at(i).toString());
        }
        jv = root.value("numbers");
        if(jv.isArray()){
            QJsonArray ja = jv.toArray();
            for(int i = 0; i < ja.count(); i++)
                tovar.ints.append(ja.at(i).toInt());
        }
        document = QJsonDocument::fromJson(a_query.value(rec.indexOf("val_str")).toByteArray());// То создаём объект Json Document, считав в него все данные из ответа
        root = document.object();
        tovar.strs.append(root.value("Made").toString());
        tovar.strs.append(root.value("Pack").toString());
        tovar.ints.append(root.value("Box").toInt());
        tovar.kol_vsego = a_query.value(rec.indexOf("kol")).toInt();
        tovar.id_product= a_query.value(rec.indexOf("id_product")).toInt();
        tovar.id_heap=    a_query.value(rec.indexOf("id_heap")).toInt();
        tovar.icon=(icons.contains(str.at(0)))?str.at(0):' ';
        QString cbline =    tovar.date_pack().toString("MM.dd")+"|"+
                            tovar.date_made().toString("MM.dd")+"|"+
                            QString::number(tovar.kol_vsego)+" "+
                            str.mid(1);
        if(tovar.icon==' ') ui->cbName->addItem(cbline);
            else ui->cbName->addItem(icons[tovar.icon],cbline);
        tovars.append(tovar);
    }
    a_query.clear();
}

