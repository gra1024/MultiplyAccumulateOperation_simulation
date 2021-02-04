#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QPixmap>
#include "config.h"
#include "asip.h"

#define DISPLAY 0

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_start_clicked();

private:
    Ui::MainWindow *ui;
    void disp(data_asip);
    void ui_disp(data_asip);
    int pushButton_start_chenge = 0;
    asip* a;
    data_asip d;

    QPixmap out_0,out_0_90,out_1,out_1_90,out_1_180;
    QPixmap sel_c,sel_l,sel_r;

    QPalette red;

};
#endif // MAINWINDOW_H
