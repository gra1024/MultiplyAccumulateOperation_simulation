#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <unistd.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    a = new asip();

    //なぜこのリンクでOKなのかわからない
    //Mac関係性？
    //Qt Createrのビルドからなら成功、"beild/architecture.app"からだと失敗
    /*
    //3行目　#include <unistd.h>のコメントを外す
    char dir[255];
    getcwd(dir,255);
    cout<<"Current Directory : "<<dir<<endl;
    ui->label_build_directry->setText(dir);
    */

    QPixmap pix = QPixmap("../../../../image/background.png");
    pix = pix.scaled(800,600);
    ui->label_background->setPixmap(pix);

    out_0 = QPixmap("../../../../image/out_0.png");
    out_0 = out_0.scaled(40,20);
    out_0_90 = QPixmap("../../../../image/out_0_90.png");
    out_0_90 = out_0_90.scaled(20,40);
    out_1 = QPixmap("../../../../image/out_1.png");
    out_1 = out_1.scaled(40,20);
    out_1_90 = QPixmap("../../../../image/out_1_90.png");
    out_1_90 = out_1_90.scaled(20,40);
    out_1_180 = QPixmap("../../../../image/out_1_180.png");
    out_1_180 = out_1_180.scaled(40,20);

    sel_c = QPixmap("../../../../image/sel_c.png");
    sel_c = sel_c.scaled(80,20);
    sel_l = QPixmap("../../../../image/sel_l.png");
    sel_l = sel_l.scaled(80,20);
    sel_r = QPixmap("../../../../image/sel_r.png");
    sel_r = sel_r.scaled(80,20);

    ui_disp(d);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_start_clicked()
{
    if (pushButton_start_chenge == 0){
        ui->pushButton_start->setText("Next");
        pushButton_start_chenge = 1;
    }

    if(pushButton_start_chenge == 2){
        qApp->closeAllWindows();
    }

    d = a->culc(DISPLAY);

    //error
    if(d.error == 1){
        cout << d.errorM << endl;
        qApp->closeAllWindows();
    }

    disp(d);

    if(d.mip.stp == 1){
        ui->pushButton_start->setText("Finish");
        cout << "finish" << endl;
        pushButton_start_chenge = 2;
    }

    ui_disp(d);

}

void  MainWindow::ui_disp(data_asip d){

    ui->lcdNumber_instruction->display(d.relative_pc);
    ui->lcdNumber_reg_A->display(d.a_reg);
    ui->lcdNumber_reg_MA1->display(d.ma1_reg);
    ui->lcdNumber_reg_MA2->display(d.ma2_reg);
    ui->lcdNumber_reg_MD1->display(d.md1_reg);
    ui->lcdNumber_reg_MD2->display(d.md2_reg);
    ui->lcdNumber_reg_loop->display(d.loop_reg);
    ui->lcdNumber_ALU->display(d.C);
    ui->lcdNumber_MPY->display(d.m_out);

    if(d.mip.MA1i==0) ui->label_sel_MA1i->setPixmap(sel_r);
    if(d.mip.MA1i==1) ui->label_sel_MA1i->setPixmap(sel_l);

    if(d.mip.MD1i==0) ui->label_sel_MD1i->setPixmap(sel_r);
    if(d.mip.MD1i==1) ui->label_sel_MD1i->setPixmap(sel_c);
    if(d.mip.MD1i==2) ui->label_sel_MD1i->setPixmap(sel_l);

    if(d.mip.MD2i==0) ui->label_sel_MD2i->setPixmap(sel_l);
    if(d.mip.MD2i==1) ui->label_sel_MD2i->setPixmap(sel_r);


    if(d.mip.Ao==0) ui->label_out_Ao->setPixmap(out_0);
    if(d.mip.Ao==1) ui->label_out_Ao->setPixmap(out_1);

    if(d.mip.MA1o==0) ui->label_out_MA1o->setPixmap(out_0);
    if(d.mip.MA1o==1) ui->label_out_MA1o->setPixmap(out_1);

    if(d.mip.MMD1o==0) ui->label_out_MMD1o->setPixmap(out_0);
    if(d.mip.MMD1o==1) ui->label_out_MMD1o->setPixmap(out_1);

    if(d.mip.MMD2o==0) ui->label_out_MMD2o->setPixmap(out_0);
    if(d.mip.MMD2o==1) ui->label_out_MMD2o->setPixmap(out_1);

    if(d.mip.MD1o==0) ui->label_out_MD1o->setPixmap(out_0);
    if(d.mip.MD1o==1) ui->label_out_MD1o->setPixmap(out_1);

    if(d.mip.Mo==0) ui->label_out_Mo->setPixmap(out_0);
    if(d.mip.Mo==1) ui->label_out_Mo->setPixmap(out_1);

    if(d.mip.ADeo==0) ui->label_out_ADeo->setPixmap(out_0_90);
    if(d.mip.ADeo==1) ui->label_out_ADeo->setPixmap(out_1_90);

    if(d.mip.Beo==0) ui->label_out_Beo->setPixmap(out_0_90);
    if(d.mip.Beo==1) ui->label_out_Beo->setPixmap(out_1_90);

    if(d.mip.Byo==0) ui->label_out_Byo->setPixmap(out_0);
    if(d.mip.Byo==1) ui->label_out_Byo->setPixmap(out_1_180);

    if(d.mip.AI1==0) ui->label_inc_AI1->clear();
    if(d.mip.AI1==1) ui->label_inc_AI1->setText("++");

    if(d.mip.AI2==0) ui->label_inc_AI2->clear();
    if(d.mip.AI2==1) ui->label_inc_AI2->setText("++");

    if(d.mip.Lpc==0) ui->label_loop_Lpc->clear();
    if(d.mip.Lpc==1) ui->label_loop_Lpc->setText("Lpc");

    QString str;
    str ="r_w   : ";
    str+=QString::number(d.mip.r_w);
    str+="\n";
    str+="Memo  : ";
    str+=QString::number(d.mip.Memo);
    str+="\n";
    str+="ALU   : ";
    str+=QString::number(d.mip.alu);
    str+="\n";
    str+="MPY   : ";
    str+=QString::number(d.mip.MPY);
    str+="\n";
    str+="stp   : ";
    str+=QString::number(d.mip.stp);
    str+="\n";
    str+="emit  : ";
    str+=QString::number(d.mip.emit_field);
    str+="\n";
    ui->textBrowser_otherParameters->setText(str);

    QString str2;
    for (int i = 0; i < nmax; i++){
        int digit=0;
        int a=d.mem[i];
        if(a==0) digit++;
        else{
            while(a!=0){
                a /= 10;
                digit++;
            }
        }
        for(int j=0; j< (6-digit);j++){
            str2+=" ";
        }
        str2+=QString::number(d.mem[i]);
        str2+=" ";
        if(i%8 == 7) str2+="\n";
    }
    ui->textBrowser_dataMemory->setText(str2);
    cout << str2.toCFString() << endl;

    if(d.mip.Al == 0) ui->label_text_A_reg->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.Al == 1) ui->label_text_A_reg->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.MA1l == 0) ui->label_text_MA1_reg->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.MA1l == 1) ui->label_text_MA1_reg->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.MD1l == 0) ui->label_text_MD1_reg->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.MD1l == 1) ui->label_text_MD1_reg->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.MD2l == 0) ui->label_text_MD2_reg->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.MD2l == 1) ui->label_text_MD2_reg->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.MA2l == 0) ui->label_text_MA2_reg->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.MA2l == 1) ui->label_text_MA2_reg->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.Lpl == 0) ui->label_text_loop_reg->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.Lpl == 1) ui->label_text_loop_reg->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.Ao == 1) ui->label_text_Ao->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.Ao == 0) ui->label_text_Ao->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.MA1o == 1) ui->label_text_MA1o->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.MA1o == 0) ui->label_text_MA1o->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.MD1o == 1) ui->label_text_MD1o->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.MD1o == 0) ui->label_text_MD1o->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.MMD1o == 1) ui->label_text_MMD1o->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.MMD1o == 0) ui->label_text_MMD1o->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.MMD2o == 1) ui->label_text_MMD2o->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.MMD2o == 0) ui->label_text_MMD2o->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.Mo == 1) ui->label_text_Mo->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.Mo == 0) ui->label_text_Mo->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.Byo == 1) ui->label_text_Byo->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.Byo == 0) ui->label_text_Byo->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.ADeo == 1) ui->label_text_Adeo->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.ADeo == 0) ui->label_text_Adeo->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");

    if(d.mip.Beo == 1) ui->label_text_Beo->setStyleSheet("color: rgb(0, 0, 0);background-color: rgba(255, 255, 255, 0);");
    if(d.mip.Beo == 0) ui->label_text_Beo->setStyleSheet("color: rgb(255, 0, 0);background-color: rgba(255, 255, 255, 0);");
}



void MainWindow::disp(data_asip d){
    cout << d.relative_pc << " th instruction" << endl;

    cout << "Al   =" << setw(1) << d.mip.Al << " ";
    cout << "Ao   =" << setw(1) << d.mip.Ao << " ";
    cout << "MA1i =" << setw(1) << d.mip.MA1i << " ";
    cout << "MA1o =" << setw(1) << d.mip.MA1o << " ";
    cout << "MA2l =" << setw(1) << d.mip.MA2l << " ";
    cout << "ADeo =" << setw(1) << d.mip.ADeo  << " " << endl;

    cout << "MD1i =" << setw(1) << d.mip.MD1i << " ";
    cout << "MD1l =" << setw(1) << d.mip.MD1l << " ";
    cout << "MD1o =" << setw(1) << d.mip.MD1o << " ";
    cout << "MD2i =" << setw(1) << d.mip.MD2i << " ";
    cout << "MD2l =" << setw(1) << d.mip.MD2l << " ";
    cout << "MPY  =" << setw(1) << d.mip.MPY << " ";
    cout << "Mo   =" << setw(1) << d.mip.Mo << " ";
    cout << "Beo  =" << setw(1) << d.mip.Beo  << " " << endl;

    cout << "Byo  =" << setw(1) << d.mip.Byo << " ";
    cout << "AI1  =" << setw(1) << d.mip.AI1 << " ";
    cout << "AI2  =" << setw(1) << d.mip.AI2 << " ";
    cout << "Lpl  =" << setw(1) << d.mip.Lpl << " ";
    cout << "Lpc  =" << setw(1) << d.mip.Lpc  << " " << endl;

    cout << "r_w  =" << setw(1) << d.mip.r_w << " ";
    cout << "MMD1o=" << setw(1) << d.mip.MMD1o << " ";
    cout << "MMD2o=" << setw(1) << d.mip.MMD2o << " ";
    cout << "Memo =" << setw(1) << d.mip.Memo  << " " << endl;

    cout << "alu  =" << setw(1) << d.mip.alu  << " "<< endl;

    cout << "ncld =" << setw(1) << d.mip.ncld << " ";
    cout << "ovf  =" << setw(1) << d.mip.ovf << " ";
    cout << "zf   =" << setw(1) << d.mip.zf << " ";
    cout << "mf   =" << setw(1) << d.mip.mf << " ";
    cout << "stp  =" << setw(1) << d.mip.stp << " " << endl;

    cout << "emit =" << setw(7) << d.mip.emit_field << " " << endl;


    //registers and memory contents
    cout << "intermediate results of registers and memory contents:" << endl;

    cout << "a_reg   =" << setw(6) << d.a_reg << " ";
    cout << "ma1_reg =" << setw(6) << d.ma1_reg << " ";
    cout << "ma2_reg =" << setw(6) << d.ma2_reg << " " << endl;

    cout << "md1_reg =" << setw(6) << d.md1_reg << " ";
    cout << "md2_reg =" << setw(6) << d.md2_reg << " ";
    cout << "loop_reg=" << setw(6) << d.loop_reg << " " << endl;

    cout << "multipliction result=" << setw(6) << d.m_out <<"     ";
    cout << "ov=" << setw(1) << d.pre_ov << "  ";
    cout << "z=" << setw(1) << d.pre_z << "  ";
    cout << "m=" << setw(1) << d.pre_m << "  "<< endl;

    cout << "memory contents:" << endl;

    for (int i = 0; i < nmax; i++){
        cout << setw(6) << d.mem[i] << " ";
        if(i%8 == 7) cout << endl;
    }
}

