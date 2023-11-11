#include "homegm.h"
#include "ui_homegm.h"
#include "waitscreengm.h"
#include <time.h>


HomeGm::HomeGm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomeGm),
    level_select(false)
{
    ui->setupUi(this);
    generate_code();
    ui->code->setText(QString::number(code));
}

HomeGm::~HomeGm()
{
    delete ui;
}

void HomeGm::on_criarsala_clicked()
{
    QString num_players=ui->num_players->toPlainText();
    QString num_round=ui->num_round->toPlainText();
    QString player_name=(ui->player_name->toPlainText());
    QString time=ui->time->toPlainText();

//    create_game(A);
    //interessante: definir aqui a matriz!!!
//    MainWindow *w=new(MainWindow(num_players,num_round,player_name,time,code));
    if(num_players!=QString("") && num_round!=QString("") && player_name!=QString("") && time!=QString("") && num_players.toInt()<=7 && level_select){
        WaitScreenGm *w=new WaitScreenGm(num_players.toInt(),num_round.toInt(),0,player_name,time.toInt(),code,level);
        w->show();
        close();
    }
}

void HomeGm::generate_code(){
    srand(time(nullptr));
    code=rand()%(9000)+1000;
}


void HomeGm::on_level_0_clicked()
{
    level_select=true;
    level=0;
    ui->level_0->setStyleSheet("QPushButton { background-color: #ffffcc; border: 1px solid black; border-radius: 5px; color: black; }");
    ui->level_1->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; border-radius: 5px; color: black; }");
    ui->level_2->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; border-radius: 5px; color: black; }");
}


void HomeGm::on_level_1_clicked()
{
    level_select=true;
    level=1;
    ui->level_0->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; border-radius: 5px; color: black; }");
    ui->level_1->setStyleSheet("QPushButton { background-color: #ffffcc; border: 1px solid black; border-radius: 5px; color: black; }");
    ui->level_2->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; border-radius: 5px; color: black; }");
}


void HomeGm::on_level_2_clicked()
{
    level_select=true;
    level=2;
    ui->level_0->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; border-radius: 5px; color: black; }");
    ui->level_1->setStyleSheet("QPushButton { background-color: white; border: 1px solid black; border-radius: 5px; color: black; }");
    ui->level_2->setStyleSheet("QPushButton { background-color: #ffffcc; border: 1px solid black; border-radius: 5px; color: black; }");
}

