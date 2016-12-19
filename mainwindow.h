#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <tovar.h>
#include <state.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btExit_clicked();

    void on_btPrint_clicked();

    void on_edScaner_returnPressed();

    void on_cbName_currentIndexChanged(int index);

    void on_pbRefresh_clicked();

private:
    void print();
    void refreshMessage(int index);
    Ui::MainWindow *ui;
    QVector<Tovar> tovars;
    QVector<State> states;

    QSqlDatabase db;
    QString pack_format,unit_format, picture;
    int box_format;
    QMap<QChar,QIcon> icons;
    QString box_printer, unit_printer;
};

#endif // MAINWINDOW_H
